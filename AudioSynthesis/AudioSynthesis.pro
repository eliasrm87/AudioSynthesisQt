#-------------------------------------------------
#
# Project created by QtCreator 2015-02-15T20:24:25
#
#-------------------------------------------------

QT       += core gui multimedia

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AudioSynthesis
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    DataFlow/linkline.cpp \
    DataFlow/node.cpp \
    DataFlow/nodelinkline.cpp \
    AudioNodes/oscillatornode.cpp \
    DataFlow/nodeterminal.cpp \
    AudioNodes/outputnode.cpp \
    Audio/audiosamplesbuffer.cpp \
    Audio/mixerworker.cpp \
    Audio/player.cpp \
    Audio/source.cpp \
    DataFlow/dataflowview.cpp \
    Audio/oscillator.cpp \
    AudioNodes/loopnode.cpp \
    Audio/loop.cpp \
    AudioControls/waveform.cpp \
    Audio/synthetizer.cpp \
    synthetizerflowview.cpp \
    Audio/synpiece.cpp \
    AudioControls/loopcontrols.cpp \
    AudioControls/partcontrols.cpp \
    Audio/output.cpp \
    Audio/sink.cpp

HEADERS  += mainwindow.h \
    DataFlow/linkline.h \
    DataFlow/node.h \
    DataFlow/nodelinkline.h \
    AudioNodes/oscillatornode.h \
    DataFlow/nodeterminal.h \
    AudioNodes/outputnode.h \
    Audio/audiosamplesbuffer.h \
    Audio/mixerworker.h \
    Audio/player.h \
    Audio/source.h \
    DataFlow/dataflowview.h \
    Audio/audio.h \
    AudioNodes/audionodes.h \
    Audio/oscillator.h \
    AudioNodes/loopnode.h \
    Audio/loop.h \
    AudioControls/waveform.h \
    Audio/synthetizer.h \
    synthetizerflowview.h \
    Audio/synpiece.h \
    AudioControls/loopcontrols.h \
    AudioControls/partcontrols.h \
    AudioControls/audiocontrols.h \
    Audio/output.h \
    Audio/sink.h

RESOURCES +=
