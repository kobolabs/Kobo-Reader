#ifndef BlackjackGAME_H
#define BlackjackGAME_H

#include <QtGui>

namespace Ui
{
    class BlackjackGame;
}


enum HandOutcome {
	Outcome_Bust,
	Outcome_Win,
	Outcome_Push,
	Outcome_Lose,
	Outcome_Blackjack
};

class Deck;

class BlackjackGame: public QWidget
{
    Q_OBJECT

public:
    BlackjackGame(QWidget *parent);
    ~BlackjackGame();

protected:
	void keyPressEvent(QKeyEvent* e);
	void paintEvent(QPaintEvent *e);
	void drawHand(int x, int y, int width, Deck *deck, QPainter &painter, bool hideFirst);
private:
	Ui::BlackjackGame* ui;
	Deck *_deck;
	Deck *_dealerHand;
	Deck *_currentPlayerHand;
	QList<Deck *>_playerHands;
	int _money;
	bool _gameOver;
	bool _playersTurn;
	QList<int> _doublingHands;
	HandOutcome outcomeForDeck(Deck *deck);
	void updateNav();
	
signals:
	void footerEvent(QString const& title, int page, int numPages);

private slots:
	void hit();
	void stand();
	void doubleDown();
	void split();
    void newGame();
    void dealersTurn();
    void scoreGame();
    void onEntrySelected(const QString& option);
};

#endif // BlackjackGAME_H
