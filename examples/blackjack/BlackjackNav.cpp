#include "BlackjackNav.h"
#include "ui_BlackjackNav.h"
#include "SelectableEntry.h"
#include <QtGui>

extern QTime lpgtime;

static const int MAX_SECTIONS = 25;
static const int MAX_SEPARATORS = 4;

static int wrap(int value, int max);
static QLabel * createSeparator();
static QLabel * createLeftArrow();
static QLabel * createRightArrow();
static QLabel * createImageLabel(const char * image);

BlackjackNav::BlackjackNav(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::BlackjackNav),
	firstEnabledEntry(NULL),
	lastEnabledEntry(NULL),
	entriesPerPage(MAX_SECTIONS),
	_firstEntryIndex(-1),
	_numSeparators(-1),
	_leftArrow(NULL),
	_rightArrow(NULL)
{
	m_ui->setupUi(this);
}

BlackjackNav::~BlackjackNav()
{
	delete m_ui;
}

void BlackjackNav::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void BlackjackNav::keyPressEvent(QKeyEvent *e)
{
	qDebug() << "BlackjackNav::keyPressEvent(QKeyEvent *e)";

	SelectableEntry *focusWidget = dynamic_cast<SelectableEntry *>(QApplication::focusWidget());
	if (focusWidget != NULL)
	{
		if (e->key() == Qt::Key_Left)
		{
			if (entries.count() == 1) {
				QWidget::keyPressEvent(e);
				return;
			}

			if (focusWidget == firstEnabledEntry)
			{
				if (entriesPerPage < entries.count())
				{
					/* FIXME: this will wrap around; do we want that? */
					entryOffset = getNextPageOffset(false);
					updateWidgets();
				}
				lastEnabledEntry->setFocus();
			}
			else
			{
				focusPreviousChild();
			}
		}
		else if (e->key() == Qt::Key_Right)
		{
			if (entries.count() == 1) {
				QWidget::keyPressEvent(e);
				return;
			}

			if (focusWidget == lastEnabledEntry)
			{
				if (entriesPerPage < entries.count())
				{
					/* FIXME: this will wrap around; do we want that? */
					entryOffset = getNextPageOffset(true);
					updateWidgets();
				}
				firstEnabledEntry->setFocus();
			}
			else
			{
				focusNextChild();
			}
		}
		else if (e->key() == Qt::Key_Space || e->key() == Qt::Key_Return )
		{
			focusWidget->reverseColors();
			QString text = focusWidget->text();
			emit alphabetEntrySelected(text);
		}
		else
		{
			QWidget::keyPressEvent(e);
		}
	}
	else
	{
		QWidget::keyPressEvent(e);
	}
}

void BlackjackNav::focusItem(QString const&item)
{
	int itemIndex = entries.indexOf(item);
	if (itemIndex < entryOffset || itemIndex >= entryOffset + entriesPerPage)
	{
		entryOffset = itemIndex - (itemIndex % entriesPerPage);
		updateWidgets();
	}

	SelectableEntry *entry = findChild<SelectableEntry *>(item);
	if (entry != NULL)
	{
		entry->setFocus();
	}
}

SelectableEntry* BlackjackNav::getIndexSection(int i)
{
	SelectableEntry* result = NULL;
	if (i < entries.size())
	{
		if (_useSeparators)
		{
			if (i < _numSeparators)
				i *= 2;
			else
				i += _numSeparators;
		}
		result = dynamic_cast<SelectableEntry *>(m_ui->horizontalLayout->itemAt(i+_firstEntryIndex)->widget());
	}
	return result;
}

void BlackjackNav::setIndexSections(QList<QString> const& sections, int pageSize)
{
	/* Here is where all the child widgets get created */
	Q_ASSERT(entries.isEmpty());

	entries = sections;
	enabled.fill(true, entries.size());
	entriesPerPage = (pageSize > 0) ? pageSize : sections.count();

	int idx = -1;
	while(m_ui->horizontalLayout->itemAt(++idx) != m_ui->horizontalSpacer) ;

	_useSeparators = entriesPerPage <= MAX_SEPARATORS + 1;
	if (_useSeparators)
	{
		_numSeparators = entriesPerPage - 1;
		if (entries.size() > entriesPerPage)
		{
			_leftArrow = createLeftArrow();
			_rightArrow = createRightArrow();
			m_ui->horizontalLayout->insertWidget(idx++, _leftArrow);
			m_ui->horizontalLayout->insertWidget(idx, _rightArrow);
		}
	}
	_firstEntryIndex = idx;

	for (int i = 0; i< entries.size(); ++i)
	{
		SelectableEntry * entry = new SelectableEntry(this);
		entry->setText(entries.at(i));
		entry->setObjectName(entries.at(i));
		m_ui->horizontalLayout->insertWidget(idx++, entry);
		if (_useSeparators && i < _numSeparators)
			m_ui->horizontalLayout->insertWidget(idx++, createSeparator());
	}
	entryOffset = 0;
}

void BlackjackNav::updateWidgets()
{
	/* Update the widgets in case of paging.. and enable/disable items */
	firstEnabledEntry = NULL;
	lastEnabledEntry = NULL;
	int i;
	for (i = 0; i < entriesPerPage; i++)
	{
		int entryIndex = i + entryOffset;
		if (entryIndex >= entries.count())
			break;

		SelectableEntry *entry = getIndexSection(i);
		if (entry != NULL)
		{
			entry->show();

			entry->setEnabled(enabled[entryIndex]);
			if (enabled[entryIndex])
			{
				if (!firstEnabledEntry)
					firstEnabledEntry = entry;
				lastEnabledEntry = entry;
			}
		}
	}
	
	for (int j = i; j < entries.size(); j++)
	{
		SelectableEntry *entry = getIndexSection(j);
		if (entry != NULL)
		{
			entry->hide();
		}
	}


	/* set left/right arrow visibility based on whether there's another page
	   in the corresponding direction */
	if (_leftArrow && _rightArrow)
	{
		if (getNextPageOffset(true) > entryOffset)
			_rightArrow->show();
		else
			_rightArrow->hide();
		if (_leftArrow && getNextPageOffset(false) < entryOffset)
			_leftArrow->show();
		else
			_leftArrow->hide();
	}


	// Our layout changed substantially, so make
	// sure it gets updated before our next paint event.
	// Qt claims it will do this for us "at the most appropriate times".
	// For instance, I would expect updateGeometry() to call this.
	// Qt is a big liar.
	m_ui->horizontalLayout->activate();
}

void BlackjackNav::setSelectedItem(QString const& item)
{
	selected = item;
	int itemIndex = entries.indexOf(item);
	if (itemIndex < entryOffset || itemIndex >= entryOffset + entriesPerPage)
	{
		entryOffset = itemIndex - (itemIndex % entriesPerPage);
		updateWidgets();
	}

	for (int i = 0; i < MAX_SECTIONS; i++)
	{
		SelectableEntry *entry = getIndexSection(i);
		if (entry != NULL)
		{
			if (entry->text() == item)
			{
				entry->setSelected(true);
			}
			else
			{
				entry->setSelected(false);
			}
		}
	}
}

void BlackjackNav::setEnabledItems(QSet<QString> items)
{
	for (int i = 0; i <enabled.count(); ++i)
		enabled[i] = items.contains(entries[i]);
	updateWidgets();
}


int BlackjackNav::getNextPageOffset(bool forward)
{
	int increment = forward?1:-1;
	int start = forward ? entryOffset + entriesPerPage : entryOffset-1;

	for(int i = wrap(start, entries.count());
		i != entryOffset;
		i = wrap(i + increment, entries.count()))
	{
		if (enabled[i])
			return i - i%entriesPerPage;
	}
	/* didn't find anything else selectable, we're not moving */
	return entryOffset;
}

// Static helpers ==============================================================

static int wrap(int value, int max)
{
	if (value < 0)
		return max - 1;
	if (value >= max)
		return 0;
	return value;
}

static QLabel * createSeparator()
{
        QLabel * separator = createImageLabel(":/images/bullet_square_5px.jpg");
        separator->setStyleSheet("padding-bottom: 2px;");
	return separator;
}

static QLabel * createLeftArrow()
{
        return createImageLabel(":/images/nav_left_arrow.jpg");
}

static QLabel * createRightArrow()
{
        return createImageLabel(":/images/nav_right_arrow.jpg");
}

static QLabel * createImageLabel(const char * image)
{
        QLabel * label = new QLabel();
        label->setPixmap(QPixmap(QString::fromUtf8(image)));
        label->setAlignment(Qt::AlignCenter);
	return label;
}

