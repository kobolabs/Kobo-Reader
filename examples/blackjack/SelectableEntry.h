#ifndef SELECTABLEENTRY_H
#define SELECTABLEENTRY_H

#include <QtGui>

namespace Ui {
	class SelectableEntry;
}

class SelectableEntry : public QLabel
{
	Q_OBJECT

public:
	SelectableEntry(QWidget *parent = 0);
	~SelectableEntry();
	void setEnabled(bool enabled);
	bool isEnabled();
	void setSelected(bool selected);
	bool isSelected() { return _selected; }
	void setText(QString const& text);
	QString text() const;
	void reverseColors();
protected:
	void focusInEvent(QFocusEvent *e);
	void focusOutEvent(QFocusEvent *e);
	void showEvent(QShowEvent *e);
private:
	void setStyle();
	void setFocusLook(bool focused);
	void updateSize();

	Ui::SelectableEntry *m_ui;
	bool _selected;
	bool _styleSet;
};

#endif // SELECTABLEENTRY_H
