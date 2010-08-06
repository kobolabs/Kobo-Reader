#ifndef WINNINGSCALCULATOR_H
#define WINNINGSCALCULATOR_H

#include "Deck.h"

class WinningsCalculator {
public:
	static int calculateWinnings(Deck hand, int bet);
	
	static QString handName(Deck hand);
	
	static void testHand(int expected, Rank rank1, Suit suit1,
		Rank rank2, Suit suit2, Rank rank3, Suit suit3,
		Rank rank4, Suit suit4, Rank rank5, Suit suit5);
};

#endif // WINNINGSCALCULATOR_H
