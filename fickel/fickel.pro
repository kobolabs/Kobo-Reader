QT += core
QT -= gui
TARGET = fickel
CONFIG += console
CONFIG -= app_bundle
linux-arm-g++ {
	INCLUDEPATH += /chroot/include
}
LIBS += -liw
TEMPLATE = app
SOURCES += main.cpp
HEADERS += main.h
