#include "VideoPokerGame.h"
#include "ui_VideoPokerGame.h"
#include <QString>
#include "Deck.h"
#include "WinningsCalculator.h"

static const int BET_AMOUNT = 10;
static const int START_AMOUNT = 210;

VideoPokerGame::VideoPokerGame(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoPokerGame)
{
	srand((unsigned)time(0));
	ui->setupUi(this);

	_deck = NULL;
	_hand = NULL;
	QSettings settings;
	_money = settings.value("videopoker/money", START_AMOUNT).toInt();
	newGame();
}

void VideoPokerGame::newGame() {
	if (_money <= 0) {
		_money = START_AMOUNT;
	}
	_money -= BET_AMOUNT;
	_canDraw = true;
	updateScore();
	_heldCards.clear();
	delete _deck;
	_deck = new Deck();
	lastFocusedHoldButton = ui->hold1;
	deal();
	ui->hold1->setFocus();
}

void VideoPokerGame::deal() {
	QList<Card> cards;
	for (int i = 0; i < 5; i++) {
		if (_heldCards.contains(i)) {
			cards.push_back(_hand->cards().at(i));
		}
		else {
			cards.push_back(_deck->pop());
		}
	}
	delete _hand;
	_hand = new Deck(cards);
	_heldCards.clear();
	showCards();
}

QString getFilenameForCard(const Card& card) {
		QString rankString = QString::number((int)card.rank());
		if (rankString.length() == 1) {
			rankString.prepend("0");
		}
		return QString::fromUtf8(":/images/%1%2.png").arg(rankString).arg(card.suitString().left(1));
}

void VideoPokerGame::showCards() {
	for (int i = 0; i < 5; i++) {
		QLabel *cardLabel = findChild<QLabel*>(QString::fromUtf8("card").append(QString::number(i + 1)));
		QString cardFileName = getFilenameForCard(_hand->cards().at(i));
		cardLabel->setPixmap(QPixmap(cardFileName));
		
		QLabel *heldLabel = findChild<QLabel*>(QString::fromUtf8("held").append(QString::number(i + 1)));
		heldLabel->setText(_heldCards.contains(i) ? "X" : " ");

		QPushButton *holdButton = findChild<QPushButton *>(QString::fromUtf8("hold").append(QString::number(i + 1)));
		holdButton->setEnabled(_canDraw);
	}
}

void VideoPokerGame::holdButtonPressed() {
	QWidget *focusWidget = QApplication::focusWidget();
	if (focusWidget->objectName().startsWith("hold")) {
		int cardNo = focusWidget->objectName().right(1).toInt() - 1;
		if (_heldCards.contains(cardNo)) {
			_heldCards.remove(cardNo);
		}
		else {
			_heldCards.insert(cardNo);
		}
	}
	showCards();
}

void VideoPokerGame::dealButtonPressed() {
	if (_canDraw) {
		_canDraw = false;
		deal();
		int winnings = WinningsCalculator::calculateWinnings(*_hand, BET_AMOUNT);
		_money += winnings;
		updateScore();
		if (winnings > 0) {
			// display some thing that says the hand they had and how much they won
		}
	}
	else {
		newGame();
	}
}

void VideoPokerGame::keyPressEvent(QKeyEvent *e) {
	QWidget *focusWidget = QApplication::focusWidget();
	if (e->key() == Qt::Key_Down) {
		if (focusWidget->objectName().startsWith("hold")) {
			ui->deal->setFocus();
		}
		else {
			lastFocusedHoldButton->setFocus();
		}
	}
	else if (e->key() == Qt::Key_Up) {
		if (focusWidget->objectName().startsWith("deal")) {
			lastFocusedHoldButton->setFocus();
		}
		else {
			ui->deal->setFocus();
		}
	}
	else if (e->key() == Qt::Key_Left) {
		if (focusWidget == ui->hold1)
			ui->hold5->setFocus();
		else if (focusWidget == ui->deal)
			ui->hold1->setFocus();
		else
			focusPreviousChild();
	}
	else if (e->key() == Qt::Key_Right) {
		if (focusWidget == ui->hold5)
			ui->hold1->setFocus();
		else if (focusWidget == ui->deal)
			ui->hold5->setFocus();
		else
			focusNextChild();
	}
	else if (e->key() == Qt::Key_Space || e->key() == Qt::Key_Return) {
		if (focusWidget->objectName().startsWith("hold")) {
			holdButtonPressed();
		}
		else if (focusWidget->objectName().startsWith("deal")) {
			dealButtonPressed();
		}
		else {
			QWidget::keyPressEvent(e);
		}
	}
	else if (e->key() == Qt::Key_Menu || e->key() == Qt::Key_Display) {
		// swallowed!
	}
	else {
		QWidget::keyPressEvent(e);
	}
	
	focusWidget = QApplication::focusWidget();
	if (focusWidget->objectName().startsWith("hold"))
		lastFocusedHoldButton = focusWidget;
}

void VideoPokerGame::updateScore() {
	if (!_canDraw && _hand != NULL) {
		ui->score->setText(QString("%1 Your score: %2").arg(WinningsCalculator::handName(*_hand)).arg(_money));		
	}
	else {
		ui->score->setText(QString::fromUtf8("Your score: ").append(QString::number(_money)));
	}
}

VideoPokerGame::~VideoPokerGame() {
	QSettings settings;
	settings.setValue("videopoker/money", _money);
	delete _deck;
    delete ui;
}
