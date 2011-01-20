#include "BlackjackPlugin.h"
#include "BlackjackGame.h"

BlackjackPlugin::BlackjackPlugin()
{
}

BlackjackPlugin::~BlackjackPlugin()
{
}

QStringList BlackjackPlugin::mimeTypes()
{
	QStringList mimeTypes;
	mimeTypes << "application/x-games-Blackjack";

	return mimeTypes;
}

QWidget *BlackjackPlugin::reader(QWidget *parent)
{
	BlackjackGame *w = new BlackjackGame(parent);
	w->setGeometry(0, 0, 600, 734);
	return w;
}

ParserInterface *BlackjackPlugin::parser()
{
	return NULL;
}

Q_EXPORT_PLUGIN2(tictactoe, BlackjackPlugin)
