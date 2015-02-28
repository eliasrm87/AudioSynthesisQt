#ifndef PLAYERTHREAD_H
#define PLAYERTHREAD_H

#include <QThread>
#include <QAudioOutput>
#include "audiosamplesbuffer.h"
#include <QDebug>

class PlayerWorker : public QObject
{
    Q_OBJECT

public:
    PlayerWorker(const QAudioFormat &format, AudioSamplesBuffer* buffer, QObject * parent = 0);
    ~PlayerWorker();

public slots:
    void play();

private slots:
    void onStateChanged(QAudio::State state);

private:
    QAudioFormat format_;
    QAudioOutput* audio_;
    AudioSamplesBuffer* buffer_;

};

#endif // PLAYERTHREAD_H
