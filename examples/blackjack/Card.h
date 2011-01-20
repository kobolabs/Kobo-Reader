#ifndef CARD_H
#define CARD_H

#include <QtGui>

enum Suit {
	Clubs = 0,
	Diamonds,
	Hearts,
	Spades
};

enum Rank {
	Ace = 1,
	Two = 2,
	Three = 3,
	Four = 4,
	Five = 5,
	Six = 6,
	Seven = 7,
	Eight = 8,
	Nine = 9,
	Ten = 10,
	Jack = 11,
	Queen = 12,
	King = 13
};

class Card {
public:
	Card(Rank rank, Suit suit);
	Card(int cardNo);
	
	Suit suit() const;
	Rank rank() const;
	QString suitString() const;
	QString debugPrint() const;
private:
	int _cardNo;
};

#endif // CARD_H
