#include "main.h"

#include <string>
#include <netinet/ether.h>
#include <sstream>

using namespace std;

#define HARDWARE_STATUS_FIFO "/tmp/nickel-hardware-status"

static const int SIGNAL_STRENGTH_UPDATE_INTERVAL = 3000000;
static const int SCANNING_NETWORKS_WAIT_INTERVAL = 250000;

int main(int argc, char *argv[])
{
	pid_t pid;

	// Fork off!
	pid = fork();
	if (pid < 0)
	{
		exit(1);
	}
	if (pid > 0)
	{
		exit(0);
	}

	// Initialize variables
	openSocket = iw_sockets_open();
	interface = string(getenv("INTERFACE"));

	networkName = "";
	networkMode = 0;
	networkMacAddress = "";
	networkSecurityType = "";
	networkLinkQuality = 0;

	monitoringLinkQuality = false;
	scanning = false;

	// Open FIFO pipe.
	fd = open(HARDWARE_STATUS_FIFO, O_WRONLY);

	// Read the arguments passed to us.
	for(int i = 0; i != argc; ++i)
	{
		string argument(argv[i]);

		if(argument.substr(0, 4) == "scan")
		{
			startScanningForNetworks();
		}
		else if(argument.substr(0, 4) == "info")
		{
			string message = "echo network details:NAME:" + getCurrentNetworkName() + "BITRATE:" + getBitRateDescription()
							 + ">>" + HARDWARE_STATUS_FIFO;
			system(message.c_str());
		}
		else if(argument.substr(0, 11) == "linkquality")
		{
			monitoringLinkQuality = true;
		}
	}

	int intervalsSinceScan = 0;
	int intervalsSinceLinkQuality = 0;

	// Start our event loop.
	while(monitoringLinkQuality || scanning)
	{
		if (monitoringLinkQuality && intervalsSinceLinkQuality >= SIGNAL_STRENGTH_UPDATE_INTERVAL)
		{
			fireLinkQualityUpdate();
			intervalsSinceLinkQuality = 0;
		}

		if (scanning)
		{
			continueScan();
			intervalsSinceScan = 0;
			intervalsSinceScan += SCANNING_NETWORKS_WAIT_INTERVAL;

			if (!scanning) {
				string scanFinished = "echo scan finished >> " + string(HARDWARE_STATUS_FIFO);
				system(scanFinished.c_str());
			}
		}

		intervalsSinceLinkQuality += SCANNING_NETWORKS_WAIT_INTERVAL;
		usleep(SCANNING_NETWORKS_WAIT_INTERVAL);
	}
}


std::string escapeForShell(const std::string& s)
{
	std::string specialChars (" !#$%&'()*+,-./}~:;>=<?@[\\]^_`{|\""); /* might not need to escape ALL of these; taken as-is from dbck-2484 test case */
	std::string ret;
	for (int i = 0; i < s.length(); ++i)
	{
		char c = s.at(i);
		if (specialChars.find(c) != specialChars.npos)
		{
			ret += '\\';
		}
		ret += c;
	}
	return ret;
}


float linkQuality()
{
	float quality = -1;

	if (openSocket <= 0) {
		return -1;
	}

	iwstats stats;
	iwrange range;

	const char *iface = interface.c_str();

	if (iw_get_range_info(openSocket, iface, &range) >= 0) {
		if (iw_get_stats(openSocket, iface, &stats, &range, 1) >= 0) {
			signed char level = stats.qual.level;
			signed char noise = stats.qual.noise;
			signed char ratio = level - noise;
			if (ratio > 40) {
				quality = 0.8;
			}
			else if (ratio > 25) {
				quality = 0.6;
			}
			else if (ratio > 15) {
				quality = 0.4;
			}
			else if (ratio > 10) {
				quality = 0.2;
			}
			else {
				quality = 0.0;
			}
		}
	}

	return quality;
}

int getBitRate()
{
	int bitRate = -1;

	if (openSocket <= 0) {
		return -1;
	}

	struct iwreq wrq;
	memset(&wrq, 0, sizeof(struct iwreq));

	int err = iw_get_ext(openSocket, interface.c_str(), SIOCGIWRATE, &wrq);
	if (err >= 0) {
		bitRate = wrq.u.bitrate.value;
	}
	return bitRate;
}

std::string getBitRateDescription()
{
	int bitRate = getBitRate();
	if (bitRate == -1) {
		return "";
	} else {
		char buffer[32];
		iw_print_bitrate(buffer, sizeof(buffer), bitRate);
		return string(buffer);
	}
}

std::string getCurrentNetworkName()
{
	string name;
	char essid[4 * IW_ESSID_MAX_SIZE + 1];

	if (openSocket > 0)
	{
		struct iwreq wrq;
		memset(&wrq, 0x00, sizeof(struct iwreq));

		memset(essid, '\0', sizeof(essid));

		wrq.u.essid.pointer = (caddr_t) essid;
		wrq.u.essid.length = IW_ESSID_MAX_SIZE + 2;
		wrq.u.essid.flags = 0;

		if(iw_get_ext(openSocket,  interface.c_str(), SIOCGIWESSID, &wrq) >= 0) {
			name = essid;
		}
	}

	return name;
}

void printInformationElement(unsigned char* buffer, int length)
{
	printf("Dumping IE of length %d\n", length);

	int offset = 0;

	int part = 0;

	while (offset < (length - 2))
	{
		unsigned char partType   = buffer[offset + 0];
		unsigned char partLength = buffer[offset + 1];
		offset += 2;

		printf("	Part #%.2d of length %d\n", partType, partLength);

		//unsigned char* buffer = &buffer[offset];
		for (int i = 0; i < partLength; i++) {
			printf("    #%.4d 0x%.2x\n", i, buffer[offset++]);
		}

		++part;
	}
}


bool find8021XAuthenticationSuiteInIE(unsigned char* buffer)
{
	unsigned char type = buffer[0];
	unsigned char length = buffer[1];
	buffer += 2;

	bool found = false;

	// Only interested in IEs of type WPA or WPA2 (not sure what the official names for those types are)
	if (type == 0x30 || type == 0xdd)
	{
		// Only interested if the version, group cipher, pairwise ciphers are there. Otherwise it is PSK
		if ((type == 0x30 && length > 14) || (type == 0xdd && length > 17))
		{
			// WPA has the OUI first, so skip that
			if (type == 0xdd)
			{
				static unsigned char wpa1_oui[3] = {0x00, 0x50, 0xf2};

				// But not all IEs that start with 0xdd are WPA. So check that the OUI is valid.
				if (memcmp(&buffer[0], wpa1_oui, 3) != 0 || buffer[3] != 0x01) {
					return false;
				}

				buffer += 4;
			}

			// Skip the Version Number and the Group Cipher
			buffer += 2 + 4;

			// Skip the Pairwise Ciphers
			uint16_t count = buffer[0] | (buffer[1] << 8);
			buffer += 2 + (count * 4);

			// Loop over the Authentication Suites and see if 802.1X is in there
			count = buffer[0] | (buffer[1] << 8);
			buffer += 2;

			for (int i = 0; i < count; i++) {
				if (buffer[3] == IW_AUTH_KEY_MGMT_802_1X) {
					found = true;
					break;
				}
				buffer += 4;
			}
		}
	}

	return found;
}

bool find8021XAuthenticationSuite(unsigned char* buffer, int length)
{
	int offset = 0;

	while (offset < (length - 2))
	{
		unsigned char partType   = buffer[offset + 0];
		unsigned char partLength = buffer[offset + 1];

		if (partType == 0x30 || partType == 0xdd) {
			if (find8021XAuthenticationSuiteInIE(&buffer[offset])) {
				return true;
			}
		}

		offset += 2 + partLength;
	}

	return false;
}

unsigned char findWPAType(unsigned char *buffer, int length)
{
	static unsigned char wpa1_oui[3] = {0x00, 0x50, 0xf2};

	int offset = 0;

	// Scan the IEs until we find a WPA one. We assume that they are listed in order of
	// preference. If that is truly the case I do not know. Worst case is that we will
	// pick WPA1 over WPA2 while both are available. Should just work though.

	while (offset < (length - 2))
	{
		switch (buffer[offset])
		{
		case 0xdd:
			{
				// Yay. Magic. There is no API for this. We should probably buy the WPA spec.
				// Not all UEs that start with 0xdd are WPA. So check that the OUI is valid.
				// WPA1 IE looks like this: 0xdd 0xNN 0x00 0x50 0xf2 (NN is length .. 3?)

				if (memcmp(&buffer[offset+2], wpa1_oui, 3) == 0 && buffer[offset+5] == 0x01) {
					return 0xdd;
				}

				break;
			}

		case 0x30:
			{
				return 0x30;
				break;
			}
		}

		// Jump to the next IE.

		offset += buffer[offset+1] + 2;
	}

	return 0;
}

void processScan()
{
	if (_wrq.u.data.length > 0)
	{
		struct iw_event event;
		struct stream_descr stream;

		iw_init_event_stream(&stream, scanBuffer, _wrq.u.data.length);

		bool skipNetwork = false;
		bool foundWPAType = false;

		while (iw_extract_event_stream(&stream, &event, _range.we_version_compiled) > 0)
		{
			switch (event.cmd)
			{
			case SIOCGIWMODE:
				{
					if (event.u.mode < 8) {
						networkMode = event.u.mode;
					}
					break;
				}

			case SIOCGIWAP:
				{
					if (skipNetwork == false && networkMacAddress.length() > 0 && networkName.length() > 0) {
						string scannedNetwork;
						ostringstream o;
						o << networkLinkQuality;
						ostringstream p;
						p << networkMode;
						scannedNetwork = "echo network scanned:NAME:" + escapeForShell(networkName) + "MACADDRESS:" + networkMacAddress + "SECURITYTYPE:" + networkSecurityType + "LINKQUALITY:" + o.str() + "MODE:" + p.str() + " >> " + HARDWARE_STATUS_FIFO;

						system(scannedNetwork.c_str());
						
						networkName = "";
						networkMode = 0;
						networkMacAddress = "";
						networkSecurityType = "";
						networkLinkQuality = 0;
					}

					foundWPAType = false;
					skipNetwork = false;
					char buffer[32];
					networkMacAddress = string(ether_ntoa_r((const struct ether_addr *) &event.u.ap_addr.sa_data, buffer));
					break;
				}

			case SIOCGIWESSID:
				{
					char essid[4*IW_ESSID_MAX_SIZE+1];
					memset(essid, '\0', sizeof(essid));
					if((event.u.essid.pointer) && (event.u.essid.length)) {
						memcpy(essid, event.u.essid.pointer, event.u.essid.length);
					}

					networkName = essid;
					break;
				}

			case IWEVQUAL:
				{
					float linkQuality = 0.0;
					signed char level = event.u.qual.level;
					signed char noise = event.u.qual.noise;
					signed char ratio = level - noise;
					if (ratio > 40) {
						linkQuality = 0.8;
					}
					else if (ratio > 25) {
						linkQuality = 0.6;
					}
					else if (ratio > 15) {
						linkQuality = 0.4;
					}
					else if (ratio > 10) {
						linkQuality = 0.2;
					}
					else {
						linkQuality = 0.0;
					}

					networkLinkQuality = linkQuality;
					break;
				}

			case SIOCGIWENCODE:
				{
					// There is no explicit WEP identifier so we default to WEP if security is enabled. We
					// will change the security type to WPA or WPA2 when the IE events are coming in.

					if (event.u.data.flags & IW_ENCODE_DISABLED) {
						networkSecurityType = "NONE";
					}
					else {
						networkSecurityType = "WEP";
					}

					break;
				}

			case IWEVGENIE:
				{
					// We sometimes get multiple IE events. Let's only take the first correct one.
					if (foundWPAType) {
						break;
					}

					// Parse the IE to find out if this AP uses WPA or WPA2.

					unsigned char type = findWPAType((unsigned char*) event.u.data.pointer, event.u.data.length);

					switch (type) {
					case 0x30:  
						foundWPAType = true;
	 					networkSecurityType = "WPA2";
						break;
					case 0xdd:
						foundWPAType = true;
						networkSecurityType = "WPA";
						break;
					}

					// If this is WPA then parse the IE again to see if it is 802.1x

					if (type != 0x00) {
						skipNetwork = find8021XAuthenticationSuite((unsigned char*)event.u.data.pointer,event.u.data.length);
					}

					break;
				}
			}
		}

		// The stream just stops, there is no 'end of network info' event. So add the last network.
		if (skipNetwork == false && networkMacAddress.length()) {
			string scannedNetwork;
			ostringstream o;
			o << networkLinkQuality;
			ostringstream p;
			p << networkMode;
			scannedNetwork = "echo network scanned:NAME:" + escapeForShell(networkName) + "MACADDRESS:" + networkMacAddress +
							 "SECURITYTYPE:" + networkSecurityType + "LINKQUALITY:" + o.str() + "MODE:" + p.str() + " >> " + HARDWARE_STATUS_FIFO;

			system(scannedNetwork.c_str());

			foundWPAType = false;
			networkName = "";
			networkMode = 0;
			networkMacAddress = "";
			networkSecurityType = "";
			networkLinkQuality = 0;
		}

	}

	scanning = false;
}

void continueScan()
{
	// Setup a results buffer if we don't have one yet.
	if (scanBuffer == NULL) {
		scanBufferSize = 65535;
		scanBuffer = (char*) malloc(scanBufferSize);
		_wrq.u.data.pointer = scanBuffer;
		_wrq.u.data.flags = 0;
		_wrq.u.data.length = scanBufferSize;
	}

	// Try to read the results
	int ret = iw_get_ext(openSocket, interface.c_str(), SIOCGIWSCAN, &_wrq);
	if (ret < 0) {
		if (errno == EAGAIN) {
			// Not all results are available s wait for event loop to check back again...
		}
	} else {
		if (_wrq.u.data.length == 0) {
			scanCount++;
			if (scanCount == 3) {
				scanning = false;
				return;
			}
			if (initiateScan() == false) {
				scanning = false;
				return;
			}
		} else {
			processScan();
		}
	}
}

bool initiateScan()
{
	// Initiate a new scan
	if (scanBuffer != NULL) {
		free(scanBuffer);
		scanBuffer = NULL;
	}

	_wrq.u.data.pointer = NULL;
	_wrq.u.data.flags = 0;
	_wrq.u.data.length = 0;

	if (iw_set_ext(openSocket, interface.c_str(), SIOCSIWSCAN, &_wrq) < 0) {
		printf("(2) Interface does not support scanning (iw_set_ext failed)");
		return false;
	}

	// We will check back by the event loop...
	return true;
}


void startScanningForNetworks()
{
	if (openSocket == -1) {
		openSocket = iw_sockets_open();
	}

	if (openSocket == -1) {
		return;
	}

	// Get range stuff
	if (iw_get_range_info(openSocket, interface.c_str(), &_range) < 0) {
		printf("(1) Interface does not support scanning (iw_get_range_info failed)");
		return;
	}

	if (initiateScan()) {
		scanning = true;
		scanCount = 0;
	}
}

void fireLinkQualityUpdate()
{
	float quality = linkQuality();
	if (quality != -1) {

		ostringstream o;
		o << quality;

		string linkQualityUpdate = "echo link quality:" +o.str() + ">>" + HARDWARE_STATUS_FIFO;
		system(linkQualityUpdate.c_str());
	}
}

