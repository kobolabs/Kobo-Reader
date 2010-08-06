#include "WinningsCalculator.h"
#include "PokerHands.h"
#include <cassert>

QList<Rank> ranksForCards(QList<Card> cards) {
	QList<Rank> ranks;
	for (int i = 0; i < cards.count(); i++) {
		ranks.push_back(cards.at(i).rank());
	}
	qSort(ranks);
	return ranks;
}

QList<int> tupleSizes(QList<Card> cards) {
	QList<int> tupleSizes;
	QList<Rank> ranks = ranksForCards(cards);
	while (!ranks.isEmpty()) {
		Rank rank = ranks.at(0);
		int tupleSize = ranks.removeAll(rank);
		tupleSizes.push_back(tupleSize);
	}
	return tupleSizes;
}

Card lowestCard(QList<Card> cards) {
	QList<Rank> ranks = ranksForCards(cards);
	return ranks.at(0);
}

Card highestCard(QList<Card> cards) {
	QList<Rank> ranks = ranksForCards(cards);
	return ranks.at(cards.count() - 1);
}

bool hasXOfAKind(QList<Card> cards, int x) {
	return tupleSizes(cards).contains(x);
}

bool hasFourOfAKind(QList<Card> cards) {
	return hasXOfAKind(cards, 4);
}

bool hasFlush(QList<Card> cards) {
	Suit firstSuit = cards.at(0).suit();
	for (int i = 0; i < cards.count(); i++) {
		Suit suit = cards.at(i).suit();
		if (suit != firstSuit) {
			return false;
		}
	}
	return true;
}

bool hasStraight(QList<Card> cards) {
	if (hasXOfAKind(cards, 2) || hasXOfAKind(cards, 3) || hasXOfAKind(cards, 4)) {
		return false;
	}
	QList<Rank> ranks = ranksForCards(cards);
	if (ranks.at(4) - ranks.at(0) == 4) {
		return true;
	}
	else if (ranks.at(0) == Ace && ranks.at(1) == Ten) {
		return true;
	}
	return false;
}

bool hasThreeOfAKind(QList<Card> cards) {
	return hasXOfAKind(cards, 3);
}

bool hasTwoPair(QList<Card> cards) {
	return tupleSizes(cards).count(2) == 2;
}

bool hasFullHouse(QList<Card> cards) {
	return tupleSizes(cards).contains(3) && tupleSizes(cards).contains(2);
}

bool hasStraightFlush(QList<Card> cards) {
	return hasFlush(cards) && hasStraight(cards);
}

bool hasRoyalFlush(QList<Card> cards) {
	if (!hasStraightFlush(cards)) {
		return false;
	}
	QList<Rank> ranks = ranksForCards(cards);
	if (ranks.at(1) == Ten)
		return true;
	return false;
}

Rank largestSet(QList<Card> cards) {
	int largestSetSize = 0;
	Rank largestSetRank = Ace;
	QList<Rank> ranks = ranksForCards(cards);
	while (!ranks.isEmpty()) {
		Rank currentRank = ranks.at(0);
		int numCommon = ranks.removeAll(currentRank);
		if (numCommon > largestSetSize) {
			largestSetRank = currentRank;
			largestSetSize = numCommon;
		}
	}
	return largestSetRank;
}

bool hasJacksOrBetter(QList<Card> cards) {
	Rank largestSetRank = largestSet(cards);
	return hasXOfAKind(cards, 2) && (largestSetRank == Ace || largestSetRank >= Jack); 
}

PokerHands pokerHand(Deck hand) {
	QList<Card> cards = hand.cards();
	PokerHands pokerHand = PokerHandRoyalFlush;
	if (hasRoyalFlush(cards)) {
		pokerHand = PokerHandRoyalFlush;
	}
	else if (hasStraightFlush(cards)) {
		pokerHand = PokerHandStraightFlush;
	}
	else if (hasFourOfAKind(cards)) {
		pokerHand = PokerHandFourOfAKind;
	}
	else if (hasFullHouse(cards)) {
		pokerHand = PokerHandFullHouse;
	}
	else if (hasFlush(cards)) {
		pokerHand = PokerHandFlush;
	}
	else if (hasStraight(cards)) {
		pokerHand = PokerHandStraight;
	}
	else if (hasThreeOfAKind(cards)) {
		pokerHand = PokerHandThreeOfAKind;
	}
	else if (hasTwoPair(cards)) {
		pokerHand = PokerHandTwoPair;
	}
	else if (hasJacksOrBetter(cards)) {
		pokerHand = PokerHandJacksOrBetter;
	}
	else {
		pokerHand = PokerHandGarbage;
	}
	return pokerHand;
}

int WinningsCalculator::calculateWinnings(Deck hand, int bet) {
	PokerHands winnings = pokerHand(hand);
	return bet * (int)winnings;
}

QString WinningsCalculator::handName(Deck hand) {
	PokerHands handType = pokerHand(hand);
	switch (handType) {
		case PokerHandRoyalFlush:
			return "Royal Flush!";
		case PokerHandStraightFlush:
			return "Straight Flush!";
		case PokerHandFourOfAKind:
			return "4 of a Kind!";
		case PokerHandFullHouse:
			return "Full House!";
		case PokerHandFlush:
			return "Flush!";
		case PokerHandStraight:
			return "Straight!";
		case PokerHandThreeOfAKind:
			return "3 of a Kind!";
		case PokerHandTwoPair:
			return "Two Pair!";
		case PokerHandJacksOrBetter:
			return "Pair!";
		case PokerHandGarbage:
		default:
			return "";
	}
}

void WinningsCalculator::testHand(int expected, Rank rank1, Suit suit1, Rank rank2, Suit suit2, Rank rank3, Suit suit3, Rank rank4, Suit suit4, Rank rank5, Suit suit5) {
	Card card1(rank1, suit1);
	Card card2(rank2, suit2);
	Card card3(rank3, suit3);
	Card card4(rank4, suit4);
	Card card5(rank5, suit5);
	card1.debugPrint();
	card2.debugPrint();
	card3.debugPrint();
	card4.debugPrint();
	card5.debugPrint();
	QList<Card> cards;
	cards.push_back(card1);
	cards.push_back(card2);
	cards.push_back(card3);
	cards.push_back(card4);
	cards.push_back(card5);
	Deck hand(cards);
	int winnings = WinningsCalculator::calculateWinnings(hand, 1);
	qDebug() << "Winnings: " << winnings << " Expected: " << expected;
	assert(winnings == expected);
}
