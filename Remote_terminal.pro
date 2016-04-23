
TEMPLATE = app
QT += network gui widgets
TARGET = RemoteTerminal
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += release

# Input
HEADERS +=   fenetre_rt.h \
    frame.h
FORMS += fenetre_rt.ui
SOURCES +=  main.cpp   fenetre_rt.cpp \
    frame.cpp
