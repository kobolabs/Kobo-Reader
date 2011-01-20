#ifndef MAIN_H
#define MAIN_H

#include <iwlib.h>
#include <time.h>
#include <signal.h>
#include <string>

bool scanning;
bool monitoringLinkQuality;

std::string interface;
int fd;

int openSocket;
int scanCount;
char* scanBuffer;
size_t scanBufferSize;

struct iw_range _range;
struct iwreq _wrq;

/* Scanned network information that we will pipe to FIFO */
std::string networkName;
int networkMode;
std::string networkMacAddress;
std::string networkSecurityType;
float networkLinkQuality;

std::string escapeForShell(const std::string&);
void fireLinkQualityUpdate();

/* Functions that get all the info from iwlib */
float getLinkQuality();
int getBitRate();
std::string getBitRateDescription();
std::string getCurrentNetworkName();

/* Scanning functions */
bool initiateScan();
void processScan();
void continueScan();
void startScanningForNetworks();

/* Functions that deal with security types */
void printInformationElement(unsigned char* buffer, int length);
bool find8021XAuthenticationSuiteInIE(unsigned char* buffer);
bool find8021XAuthenticationSuite(unsigned char* buffer, int length);
unsigned char findWPAType(unsigned char *buffer, int length);

#endif // MAIN_H
