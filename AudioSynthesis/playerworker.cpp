#include "playerworker.h"

PlayerWorker::PlayerWorker(const QAudioFormat &format, AudioSamplesBuffer *buffer, QObject *parent) :
    QObject(parent), format_(format), buffer_(buffer)
{
    audio_ = 0;
}

PlayerWorker::~PlayerWorker()
{
    if (audio_ != 0)
        audio_->stop();
}

void PlayerWorker::play()
{
    audio_ = new QAudioOutput(format_, this);
    connect(audio_, SIGNAL(stateChanged(QAudio::State)),
            this,   SLOT(onStateChanged(QAudio::State)));

    audio_->start(buffer_);
}

void PlayerWorker::onStateChanged(QAudio::State state)
{
    qDebug() << state << " " << audio_->error();
    if (audio_->error() != QAudio::NoError)
        audio_->start(buffer_);
}
