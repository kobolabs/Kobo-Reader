#ifndef LESS_OBTRUSIVE_PUSH_BUTTON_H
#define LESS_OBTRUSIVE_PUSH_BUTTON_H

#include <QtGui>

// stop eating my key presses, QPushButton!
class LessObtrusivePushButton : public QPushButton {
	Q_OBJECT
public:
	LessObtrusivePushButton(QWidget *parent = 0);
protected:
	virtual void keyPressEvent(QKeyEvent *e);
};

#endif
