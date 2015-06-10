#-------------------------------------------------
#
# Project created by QtCreator 2015-05-21T10:43:49
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUIPrototype
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chamber.cpp \
    communication.cpp \
    controlpc.cpp

HEADERS  += mainwindow.h \
    chamber.h \
    communication.h \
    controlpc.h

FORMS    += mainwindow.ui
