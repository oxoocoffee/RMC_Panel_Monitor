#-------------------------------------------------
#
# Project created by QtCreator 2015-04-30T14:13:59
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sigmux_GUI
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        inputmonitor.cpp \
        videoconnector.cpp \
        joystickconnector.cpp \
    broadcastudp.cpp

HEADERS  += mainwindow.h \
            inputmonitor.h \
            videoconnector.h \
            joystickconnector.h \
    broadcastudp.h

FORMS    += mainwindow.ui

LIBS += -L/usr/local/lib -lSDL2
INCLUDEPATH += /usr/local/include
