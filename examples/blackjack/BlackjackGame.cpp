#include "BlackjackGame.h"
#include "ui_BlackjackGame.h"
#include <QString>
#include "Deck.h"

static const int BET_AMOUNT = 10;
static const int START_AMOUNT = 210;



BlackjackGame::BlackjackGame(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BlackjackGame)
{
	srand((unsigned)time(0));
	ui->setupUi(this);
	ui->nav->setFocus();
	
	QStringList options;
	options << "HIT ME!" << "STAND" << "DOUBLE" << "SPLIT";
	ui->nav->setIndexSections(options);
	ui->nav->setEnabledItems(options.toSet());
	connect(ui->nav, SIGNAL(alphabetEntrySelected(const QString&)), this, SLOT(onEntrySelected(const QString&)));
	_deck = NULL;
	_dealerHand = NULL;
	_currentPlayerHand = NULL;
	QSettings settings;
	_money = settings.value("Blackjack/money", START_AMOUNT).toInt();
	newGame();
}

static Deck *deckFromDeck(Deck *deck, int count, Deck *src = NULL) {
	QList<Card> cards;
	if (src != NULL)
		cards = src->cards();
	for (int i = 0; i < count; i++) {
		cards.push_back(deck->pop());
	}
	return new Deck(cards);
}

static int valueOfCard(Card card) {
	Rank rank = card.rank();
	if (rank == King || rank == Queen || rank == Jack)
		return 10;
	return (int)rank;
}

static Deck *dealToDeck(Deck *dest, Deck *src) {
	Deck *deck = deckFromDeck(src, 1, dest);
	delete dest;
	return deck;
}

static int valueForDeck(Deck *deck) {
	QList<Card> cards = deck->cards();
	bool hasAce = false;
	int total = 0;
	for (int i = 0; i < cards.count(); i++) {
		total += valueOfCard(cards.at(i));
		if (cards.at(i).rank() == Ace)
			hasAce = true;
	}
	if (total < 12 && hasAce)
		total += 10;
	return total;
}

static bool canSplit(Deck *deck) {
	if (deck->cards().count() != 2)
		return false;
	return deck->cards()[0].rank() == deck->cards()[1].rank();
}

void BlackjackGame::newGame() {
	emit footerEvent("", 0, 0);

	_gameOver = false;
	if (_money <= 0) {
		_money = START_AMOUNT;
	}
	delete _deck;
	_deck = new Deck();
	QList<Card> empty;
	delete _dealerHand;
	_dealerHand = deckFromDeck(_deck, 2);
	delete _currentPlayerHand;
	_playersTurn = true;
	_currentPlayerHand = deckFromDeck(_deck, 2);
	_playerHands.clear();
	_playerHands.push_back(_currentPlayerHand);
	_doublingHands.clear();
	if (valueForDeck(_dealerHand) == 21 || valueForDeck(_currentPlayerHand) == 21) {
		_gameOver = true;
		_playersTurn = false;
		scoreGame();
	}
	else {
		ui->nav->focusItem("HIT ME!");
	}
	updateNav();
	update();
}

static QString getFilenameForCard(const Card& card) {
		QString rankString = QString::number((int)card.rank());
		if (rankString.length() == 1) {
			rankString.prepend("0");
		}
		return QString::fromUtf8(":/images/%1%2.png").arg(rankString).arg(card.suitString().left(1));
}

void BlackjackGame::updateNav() {
	QSet<QString> enabledItems;
	if (_playersTurn && !_gameOver) {
		enabledItems << "HIT ME!" << "STAND";
		if (canSplit(_currentPlayerHand))
			enabledItems << "SPLIT";
		if (_currentPlayerHand->cards().count() == 2)
			enabledItems << "DOUBLE";
	}
	ui->nav->setEnabledItems(enabledItems);
}

void BlackjackGame::hit() {
	int handIndex = _playerHands.indexOf(_currentPlayerHand);
	_playerHands.removeOne(_currentPlayerHand);
	_currentPlayerHand = dealToDeck(_currentPlayerHand, _deck);
	_playerHands.insert(handIndex, _currentPlayerHand);
	qDebug() << "HIT" << valueForDeck(_currentPlayerHand);
	if (valueForDeck(_currentPlayerHand) > 21 || _doublingHands.contains(handIndex)) {
		stand();
	}
	updateNav();
	update();
}

HandOutcome BlackjackGame::outcomeForDeck(Deck *deck) {
	int playerTotal = valueForDeck(deck);
	int dealerTotal = valueForDeck(_dealerHand);
	if (playerTotal > 21)
		return Outcome_Bust;
	else if (dealerTotal > 21 || playerTotal > dealerTotal) {
		if (playerTotal == 21 && deck->cards().count() == 2)
			return Outcome_Blackjack;
		return Outcome_Win;
	}
	else if (playerTotal == dealerTotal) {
		return Outcome_Push;
	}
	return Outcome_Lose;
}

void BlackjackGame::scoreGame() {
	for (int i = 0; i < _playerHands.count(); i++) {
		int handBet = BET_AMOUNT;
		if (_doublingHands.contains(i)) {
			handBet = handBet * 2;
		}
		HandOutcome outcome = outcomeForDeck(_playerHands.at(i));
		if (outcome == Outcome_Bust || outcome == Outcome_Lose)
			_money -= handBet;
		else if (outcome == Outcome_Blackjack)
			_money += (int) (handBet * 1.5);
		else if (outcome == Outcome_Win)
			_money += handBet;
	}
	ui->score->setText(QString("YOUR SCORE: %1").arg(_money));
	update();
}

void BlackjackGame::dealersTurn() {
	_playersTurn = false;
	qDebug() << "DEALER'S TURN";
	updateNav();
	setFocus();
	repaint();
	bool playDealer = false;
	for (int i = 0; i < _playerHands.count(); i++) {
		if (valueForDeck(_playerHands.at(i)) <= 21) {
			playDealer = true;
			break;
		}
	}
	while(playDealer && valueForDeck(_dealerHand) < 17) {
		_dealerHand = dealToDeck(_dealerHand, _deck);
		qDebug() << "DEALER DECK: " << valueForDeck(_dealerHand);
		if (valueForDeck(_dealerHand) < 17)
			repaint();
	}
	scoreGame();
	_gameOver = true;
	update();
}

void BlackjackGame::stand() {
	qDebug() << "STAND";
	if (_currentPlayerHand == _playerHands.back()) {
		dealersTurn();
	}
	else {
		_currentPlayerHand = _playerHands.at(_playerHands.indexOf(_currentPlayerHand) + 1);
		updateNav();
		update();
	}
}

void BlackjackGame::doubleDown() {
	int handIndex = _playerHands.indexOf(_currentPlayerHand);
	_doublingHands.push_back(handIndex);
	hit();
}

void BlackjackGame::split() {
	QList<Card> cards = _currentPlayerHand->cards();
	QList<Card> leftHand;
	leftHand.push_back(cards.takeFirst());
	leftHand.push_back(_deck->pop());
	QList<Card> rightHand;
	rightHand.push_back(cards.takeFirst());
	rightHand.push_back(_deck->pop());
	int handIndex = _playerHands.indexOf(_currentPlayerHand);
	_playerHands.removeOne(_currentPlayerHand);
	delete _currentPlayerHand;
	_currentPlayerHand = new Deck(leftHand);
	_playerHands.insert(handIndex, new Deck(rightHand));
	_playerHands.insert(handIndex, _currentPlayerHand);
	updateNav();
	update();
}

void BlackjackGame::onEntrySelected(const QString& option) {
	if (option == "HIT ME!") {
		hit();
	}
	else if (option == "STAND") {
		stand();
	}
	else if (option == "DOUBLE") {
		doubleDown();
	}
	else if (option == "SPLIT") {
		split();
	}
}

void BlackjackGame::keyPressEvent(QKeyEvent *e) {
	QWidget *focusWidget = QApplication::focusWidget();
	qDebug() << "BLACKJACK KEY PRESS: " << focusWidget;
	if (_gameOver) {
		newGame();
		return;
	}
	if (e->key() == Qt::Key_Menu || e->key() == Qt::Key_Display) {
		// swallowed!
	}
	else {
		QWidget::keyPressEvent(e);
	}
}

BlackjackGame::~BlackjackGame() {
	QSettings settings;
	settings.setValue("Blackjack/money", _money);
	delete _deck;
    delete ui;
}


static const int HAND_HEIGHT = 96;
static const int CARD_WIDTH = 71;
void BlackjackGame::drawHand(int x, int y, int width, Deck *deck, QPainter &painter, bool hideFirst) {
	int spacing = (int) std::min((double)(width - CARD_WIDTH) / (double)std::max(deck->cards().count(), 1), 100.0);
	spacing = std::max(spacing, 5);
	qDebug() << "SPACING: " << spacing << " WIDTH: " << width;
	for (int i = 0; i < deck->cards().count(); i++) {
		QString path = getFilenameForCard(deck->cards().at(i));
		if (i == 0 && hideFirst) {
			path = ":/images/back.png";
		}
		painter.drawPixmap(x + (i * spacing), y, CARD_WIDTH, HAND_HEIGHT, QPixmap(path));
		if (!(hideFirst && deck == _dealerHand)) {
			QString status;
			if (_gameOver && deck != _dealerHand) {
				HandOutcome outcome = outcomeForDeck(deck);
				if (outcome == Outcome_Bust)
					status = "BUST";
				else if (outcome == Outcome_Blackjack)
					status = "BLACKJACK";
				else if (outcome == Outcome_Push)
					status = "PUSH";
				else if (outcome == Outcome_Lose)
					status = QString("%1 - LOSE").arg(valueForDeck(deck));
				else if (outcome == Outcome_Win)
					status = QString("%1 - WIN").arg(valueForDeck(deck));
			}
			else if (valueForDeck(deck) > 21) {
				status = "BUST";
			}
			else if (valueForDeck(deck) == 21 && deck->cards().count() == 2)
				status = "BLACKJACK";
			else {
				status = QString::number(valueForDeck(deck));
			}
			painter.drawText(x + 20, y + HAND_HEIGHT + 20, status);
		}
	}
}

void BlackjackGame::paintEvent(QPaintEvent *e) {
	Q_UNUSED(e);
	QPainter painter(this);
	int playWidth = 500;
	drawHand(50, 100, playWidth, _dealerHand, painter, _playersTurn && valueForDeck(_dealerHand) != 21);
	int numPlayerHands = _playerHands.count();
	int handWidth = std::min(playWidth / numPlayerHands, playWidth);
	for (int i = 0; i < numPlayerHands; i++) {
		Deck *deck = _playerHands.at(i);
		drawHand(50 + (i * handWidth), 450, handWidth, deck, painter, false);
		if (deck == _currentPlayerHand) {
			// TODO: this rect is too ugly!
			painter.drawRect(50 + (i * handWidth), 450, handWidth, HAND_HEIGHT);
		}
	}
}
