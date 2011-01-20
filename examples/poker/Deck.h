#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <QtCore>

class Deck {
public:
	Deck(QList<Card> cards);
	Deck();
	void shuffle();
	Card pop();
	QList<Card> cards() const;
private:
	QList<Card> _cards;
};

#endif // DECK_H
