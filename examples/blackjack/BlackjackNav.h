#ifndef BLACKJACKNAV_H
#define BLACKJACKNAV_H

#include <QtGui>

namespace Ui {
	class BlackjackNav;
}

class SelectableEntry;

class BlackjackNav : public QWidget {
	Q_OBJECT
public:
	BlackjackNav(QWidget *parent = 0);
	~BlackjackNav();
	void focusItem(QString const& item);
	void setSelectedItem(QString const& item);
	void setIndexSections(QList<QString> const& sections, int pageSize = -1);
	QString getSelectedItem() const  { return selected; }
	void updateWidgets();

	void setEnabledItems(QSet<QString> items);

protected:
	void changeEvent(QEvent *e);
	void keyPressEvent(QKeyEvent *e);

signals:
	void alphabetEntrySelected(const QString &text);

private:
	SelectableEntry* getIndexSection(int i);
	int getNextPageOffset(bool forward);

	Ui::BlackjackNav *m_ui;
	SelectableEntry *firstEnabledEntry;
	SelectableEntry *lastEnabledEntry;

	int entriesPerPage;
	int entryOffset;
	int _firstEntryIndex;
	int _numSeparators;
	bool _useSeparators;
	QList<QString> entries;
	QVector<bool> enabled;
	QString selected;

	QLabel* _leftArrow;
	QLabel* _rightArrow;
};

#endif // BLACKJACKNAV_H
