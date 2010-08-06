#include "VideoPokerPlugin.h"
#include "VideoPokerGame.h"

VideoPokerPlugin::VideoPokerPlugin()
{
	qDebug() << __FUNCTION__;
}

VideoPokerPlugin::~VideoPokerPlugin()
{
	qDebug() << __FUNCTION__;
}

QStringList VideoPokerPlugin::mimeTypes()
{
	qDebug() << __FUNCTION__;

	QStringList mimeTypes;
	mimeTypes << "application/x-games-videopoker";

	return mimeTypes;
}

QWidget *VideoPokerPlugin::reader(QWidget *parent)
{
	qDebug() << __FUNCTION__;
	VideoPokerGame *w = new VideoPokerGame(parent);
	w->setGeometry(0, 0, 600, 734);
	return w;
}

ParserInterface *VideoPokerPlugin::parser()
{
	return NULL;
}

Q_EXPORT_PLUGIN2(tictactoe, VideoPokerPlugin)
