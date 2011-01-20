#ifndef VIDEOPOKERGAME_H
#define VIDEOPOKERGAME_H

#include <QtGui>

namespace Ui
{
    class VideoPokerGame;
}

class Deck;

class VideoPokerGame: public QWidget
{
    Q_OBJECT

public:
    VideoPokerGame(QWidget *parent);
    ~VideoPokerGame();

protected:
	void keyPressEvent(QKeyEvent* e);
private:
    Ui::VideoPokerGame* ui;
    QWidget *lastFocusedHoldButton;
    Deck *_deck;
    Deck *_hand;
    QSet<int> _heldCards;
    int _money;
    bool _canDraw;
    
    void deal();
    void showCards();
    void updateScore();

private slots:
	void dealButtonPressed();
	void holdButtonPressed();
    void newGame();
};

#endif // VIDEOPOKERGAME_H
