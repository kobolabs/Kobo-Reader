NICKEL_DIR = ../..
include($$NICKEL_DIR/common.pri)

CONFIG += noqca

TEMPLATE = lib
CONFIG += plugin
TARGET = Blackjack

SOURCES += Deck.cpp \
    Card.cpp \
    BlackjackGame.cpp \
    BlackjackPlugin.cpp \
    SelectableEntry.cpp \
    BlackjackNav.cpp

HEADERS += Deck.h \
    ../PluginInterface.h \
    Card.h \
    BlackjackGame.h \
    BlackjackPlugin.h \
	SelectableEntry.h \
	BlackjackNav.h

FORMS += BlackjackGame.ui \
		 BlackjackNav.ui

RESOURCES += images/Blackjack.qrc
