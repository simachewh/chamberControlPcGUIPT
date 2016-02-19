#-------------------------------------------------
#
# Project created by QtCreator 2015-05-20T11:19:05
#
#-------------------------------------------------

QT       += core gui serialport printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUIPrototype
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addprogram.cpp \
    addstep.cpp \
    chamber.cpp \
    communication.cpp \
    databackup.cpp \
    loadprogram.cpp \
    program.cpp \
    step.cpp \
    stepsmodel.cpp \
    renamedialog.cpp \
    controller.cpp \
    controlcommands.cpp \
    optionswidget.cpp \
    pid.cpp \
    addpid.cpp \
    pidlistmodel.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    addprogram.h \
    addstep.h \
    chamber.h \
    communication.h \
    databackup.h \
    loadprogram.h \
    program.h \
    step.h \
    stepsmodel.h \
    renamedialog.h \
    controller.h \
    controlcommands.h \
    optionswidget.h \
    pid.h \
    addpid.h \
    pidlistmodel.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    addprogram.ui \
    addstep.ui \
    loadprogram.ui \
    renamedialog.ui \
    optionswidget.ui \
    addpid.ui

DISTFILES += \
    GUIPrototype.pro.user

RESOURCES += \
    res.qrc \
    options-icons.qrc
