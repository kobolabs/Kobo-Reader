NICKEL_DIR = ../..

CONFIG += noqca

TEMPLATE = lib
CONFIG += plugin
TARGET = VideoPoker

SOURCES += Deck.cpp \
    Card.cpp \
    VideoPokerGame.cpp \
    VideoPokerPlugin.cpp \
    WinningsCalculator.cpp 

contains(project, nickel) {
    SOURCES += LessObtrusivePushButton.cpp
}

HEADERS += Deck.h \
    ../PluginInterface.h \
    Card.h \
    VideoPokerGame.h \
    VideoPokerPlugin.h \
    WinningsCalculator.h \
    PokerHands.h \

contains(project, nickel) {
    HEADERS += LessObtrusivePushButton.h
}
    
FORMS += VideoPokerGame.ui
RESOURCES += images/VideoPoker.qrc
