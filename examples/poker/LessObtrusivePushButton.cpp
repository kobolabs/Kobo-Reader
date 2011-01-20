#include "LessObtrusivePushButton.h"

LessObtrusivePushButton::LessObtrusivePushButton(QWidget *parent) : QPushButton(parent) {}

void LessObtrusivePushButton::keyPressEvent(QKeyEvent *e) {e->ignore();}
