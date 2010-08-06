#ifndef __VIDEO_POKER_PLUGIN_H__
#define __VIDEO_POKER_PLUGIN_H__

#include <QtGui>
#include "../PluginInterface.h"

class VideoPokerPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)

public:
	VideoPokerPlugin();
	~VideoPokerPlugin();

	QStringList mimeTypes();

	QWidget *reader(QWidget *parent = 0);
	ParserInterface *parser();

};

#endif
