#include "Card.h"
#include <QtGui>

Card::Card(Rank rank, Suit suit) {
	_cardNo = (13 * (int)suit) + (int) rank;
}

Card::Card(int cardNo) {
	_cardNo = cardNo;
}

Suit Card::suit() const {
	
	return (Suit)((_cardNo - 1) / 13);
}

Rank Card::rank() const {
	int rank = _cardNo % 13;
	if (rank == 0) {
		return King;
	}
	return (Rank)rank;
}

QString Card::suitString() const {
	QString suitstr;
	switch(suit()) {
	case Clubs:
		suitstr = "clubs";
		break;
	case Diamonds:
		suitstr = "diamonds";
		break;
	case Hearts:
		suitstr = "hearts";
		break;
	case Spades:
	default:
		suitstr = "spades";
		break;
	}
	return suitstr;
}

QString Card::debugPrint() const {
	QString rankstr;
	switch (rank()) {
	case Ace:
		rankstr = "Ace";
		break;
	case Jack:
		rankstr = "Jack";
		break;
	case Queen:
		rankstr = "Queen";
		break;
	case King:
		rankstr = "King";
		break;
	default:
		rankstr = QString::number((int)rank());
		break;
	}
	return rankstr.append(" of " ).append(suitString());
}
