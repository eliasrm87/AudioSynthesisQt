#-------------------------------------------------
#
# Project created by QtCreator 2015-02-15T20:24:25
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AudioSynthesis
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    audiosamplesbuffer.cpp \
    playerworker.cpp \
    mixerworker.cpp \
    source.cpp

HEADERS  += mainwindow.h \
    audiosamplesbuffer.h \
    playerworker.h \
    mixerworker.h \
    source.h
