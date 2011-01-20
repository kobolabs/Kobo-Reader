#include "SelectableEntry.h"
#include <QtGui>
#include "ui_SelectableEntry.h"

static const QString EnabledStyle = "font-family: \"Avenir\"; font-size: 15px; font-weight: bold;";
static const QString DisabledStyle = "color: grey;font-family: \"Avenir\"; font-size: 15px; font-weight: bold;";


SelectableEntry::SelectableEntry(QWidget *parent)
: QLabel(parent)
, m_ui(new Ui::SelectableEntry)
, _selected(false)
, _styleSet(false)
{
	m_ui->setupUi(this);

	setFocusLook(false);

	/* don't apply style right away; we need to do it in setEnabled, and
	   don't want to do setStyleSheet twice per Library Index button.
	   This means everyone MUST call setEnabled.
	 */
}

SelectableEntry::~SelectableEntry() {
	delete m_ui;
}

void SelectableEntry::setText(QString const& text) {
	m_ui->text->setText(text);
	updateSize();
}

QString SelectableEntry::text() const {
	return m_ui->text->text();
}

void SelectableEntry::setEnabled(bool enabled) {
	/* do nothing if we're already in the right state, setStyleSheet is heavyweight */
	if (_styleSet && enabled == isEnabled())
		return;
	if (enabled)
		setFocusPolicy(Qt::TabFocus);
	else
		setFocusPolicy(Qt::NoFocus);
	setStyle();
	_styleSet = true;
}

bool SelectableEntry::isEnabled() {
	return focusPolicy() == Qt::TabFocus;
}

void SelectableEntry::setSelected(bool selected) {
	_selected = selected;
}

void SelectableEntry::focusInEvent(QFocusEvent *e) {
	Q_UNUSED(e);
	setFocusLook(true);
}

void SelectableEntry::focusOutEvent(QFocusEvent *e) {
	Q_UNUSED(e);
	setFocusLook(false);
}

void SelectableEntry::showEvent(QShowEvent *e) {
	/* setEnabled needs to be called to apply styles BEFORE we get here;
	   applying styles here leads to flickering. Let's be noisy if someone
	   forgets. */
	if (!_styleSet) {
		qDebug()<< "SelectableEntry::showEvent: Someone forgot to call setEnabled!";
	}
}

void SelectableEntry::setStyle() {
	QString style;

	if (isEnabled())
		style = EnabledStyle;
	else
		style = DisabledStyle;
	m_ui->text->setStyleSheet(style);
	updateSize();
}

void SelectableEntry::reverseColors() {
	bool focused = m_ui->topLine->isVisible();
	setFocusLook(false);
	QString originalStyle = styleSheet();
	setStyleSheet(originalStyle+"; color: white; background-color: black");
	setAutoFillBackground(true);
	/* Force a repaint */
	repaint();
	/* Reset things for the next time */
	setStyleSheet(originalStyle);
	if(focused) {
		setFocusLook(true);
	}

}


void SelectableEntry::setFocusLook(bool focused)
{
	m_ui->topLine->setVisible(focused);
	m_ui->bottomLine->setVisible(focused);
}


void SelectableEntry::updateSize()
{
	QFontMetrics metrics(m_ui->text->font());
	int textWidth = metrics.width(m_ui->text->text()) + 2;
	int textHeight = metrics.height();

	QRect topRect = m_ui->topLine->geometry();
	topRect.setWidth(textWidth);
	m_ui->topLine->setGeometry(topRect);

	QRect textRect = m_ui->text->geometry();
	textRect.setWidth(textWidth);
	textRect.setHeight(textHeight);
	m_ui->text->setGeometry(textRect);

	/*
	Calculate vertical displacement between the top line and the text label (as
	defined in the SelectableEntry.ui file), and use that same displacement
	between the text label and the bottom line.
	We always (?) use uppercase labels, which means we have to exclude the
	font's descent from our calculations.
	*/
	
	int delta_y = textRect.top() - topRect.bottom() + 1;
	
	QRect bottomRect = m_ui->bottomLine->geometry();
	bottomRect.setWidth(textWidth);
	bottomRect.moveTop(textRect.bottom() + delta_y - metrics.descent());
	m_ui->bottomLine->setGeometry(bottomRect);

	this->setFixedWidth(textWidth);
	this->setFixedHeight(bottomRect.bottom() + 1);
}
