#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioOutput>
#include <QDebug>
#include <QPushButton>
#include <QBuffer>
#include <QtMath>
#include <QFile>
#include <QElapsedTimer>
#include "audiosamplesbuffer.h"
#include "playerworker.h"
#include "mixerworker.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onPlay();
protected:
    void changeEvent(QEvent * event);
private:
    qint16 getSample16(QByteArray &samples, qint64 i);
    QByteArray &getSamples16(QByteArray &samples, qint64 i, qint64 n);
    void sine16(QByteArray &array, quint64 samples, qreal frequency, qreal amplitude = 1, qreal sampleRate = 44100);
    QByteArray mix16(QByteArray ch1, QByteArray ch2);
    void delay16(QByteArray &result, QByteArray &samples, qreal delay, qreal sampleRate = 44100);
    void rever16(QByteArray &result, QByteArray &samples, qreal delay, qreal decay, qreal sampleRate = 44100);
    void fadeIn16(QByteArray &result, QByteArray &samples, qreal delay, qreal sampleRate = 44100);
    void fadeOut16(QByteArray &result, QByteArray &samples, qreal delay, qreal sampleRate = 44100);

    QByteArray organ16_6(quint64 samples, qreal frequency);

    QAudioFormat format_;
    PlayerWorker* player_;
    QThread* playerThread_;
    MixerWorker* mixer_;
    QThread* mixerThread_;
    AudioSamplesBuffer* buffer_;
    QPushButton* btnPlay_;

};

#endif // MAINWINDOW_H
