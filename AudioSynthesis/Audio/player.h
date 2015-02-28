#ifndef PLAYERTHREAD_H
#define PLAYERTHREAD_H

#include <QObject>
#include <QAudioOutput>
#include "audiosamplesbuffer.h"
#include <QDebug>

class Player : public QObject
{
    Q_OBJECT
public:
    Player(const QAudioFormat &format, AudioSamplesBuffer* buffer, QObject * parent = 0);
    ~Player();
    void start();

private slots:
    void onStateChanged(QAudio::State state);

private:
    QAudioFormat format_;
    QAudioOutput* audio_;
    AudioSamplesBuffer* buffer_;

};

#endif // PLAYERTHREAD_H
