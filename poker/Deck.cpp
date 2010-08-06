#include "Deck.h"


Deck::Deck() {
	for (int i = 0; i < 52; i++) {
		Card card(i);
		_cards.push_back(card);
	}
	shuffle();
}

Deck::Deck(QList<Card> cards) {
	_cards = cards;
}

void Deck::shuffle() {
	for (int n = _cards.count() - 1; n > 1; n--) {
		int k = rand() % n;
		_cards.swap(n, k);
	}
}

Card Deck::pop() {
	if (_cards.isEmpty()) {
		return Card(-1);
	}
	return _cards.takeFirst();
}

QList<Card> Deck::cards() const {
	return _cards;
}
