#include "VideoPokerPlugin.h"
#include "VideoPokerGame.h"

VideoPokerPlugin::VideoPokerPlugin()
{
}

VideoPokerPlugin::~VideoPokerPlugin()
{
}

QStringList VideoPokerPlugin::mimeTypes()
{
	QStringList mimeTypes;
	mimeTypes << "application/x-games-videopoker";

	return mimeTypes;
}

QWidget *VideoPokerPlugin::reader(QWidget *parent)
{
	VideoPokerGame *w = new VideoPokerGame(parent);
	w->setGeometry(0, 0, 600, 734);
	return w;
}

ParserInterface *VideoPokerPlugin::parser()
{
	return NULL;
}

Q_EXPORT_PLUGIN2(tictactoe, VideoPokerPlugin)
