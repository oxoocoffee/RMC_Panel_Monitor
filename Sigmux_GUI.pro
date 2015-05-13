#-------------------------------------------------
#
# Project created by QtCreator 2015-04-30T14:13:59
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=gnu++11
TARGET         = Sigmux_GUI
TEMPLATE       = app

SOURCES += main.cpp\
        mainwindow.cpp \
        videoconnector.cpp \
        joystickconnector.cpp \
        broadcastudp.cpp \
    inputThrottler.cpp

HEADERS  += mainwindow.h \
            videoconnector.h \
            joystickconnector.h \
            broadcastudp.h \
    inputThrottler.h \
    commonhdr.h

FORMS    += mainwindow.ui

LIBS += -L/usr/local/lib -lSDL2
INCLUDEPATH += /usr/local/include
