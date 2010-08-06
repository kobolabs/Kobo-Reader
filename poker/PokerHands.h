#ifndef POKERHANDS_H
#define POKERHANDS_H

enum PokerHands {
	PokerHandGarbage = 0,
	PokerHandJacksOrBetter = 1,
	PokerHandTwoPair = 2,
	PokerHandThreeOfAKind = 3,
	PokerHandStraight = 4,
	PokerHandFlush = 6,
	PokerHandFullHouse = 10,
	PokerHandFourOfAKind = 25,
	PokerHandStraightFlush = 100,
	PokerHandRoyalFlush = 500
};

#endif // POKERHANDS_H
