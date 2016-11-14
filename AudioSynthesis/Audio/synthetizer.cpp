#include "synthetizer.h"

Synthetizer::Synthetizer(QObject *parent) :
    QThread(parent)
{

}

Synthetizer::~Synthetizer()
{
    masterMixer_->quit();
    while (masterMixer_->isRunning())
        QThread::msleep(1);
    masterMixer_->deleteLater();
    player_->deleteLater();
    buffer_->deleteLater();
}

void Synthetizer::addPiece(SynPiece* piece)
{
    if (pieces_.contains(piece->objectName()))
        return;

    pieces_.insert(piece->objectName(), piece);

    if (piece->pieceClass() == "Output") {
        outputs_.push_back(piece->objectName());
    }
}

SynPiece *Synthetizer::getPiece(QString Uuid)
{
    if (!pieces_.contains(Uuid))
        return Q_NULLPTR;

    return pieces_[Uuid];
}

int Synthetizer::removePiece(QString Uuid)
{
    if (!pieces_.contains(Uuid))
        return 0;

    foreach (Connection link, links_) {
        if ((link.first == Uuid) || (link.second == Uuid)) {
            disconnectPieces(link.first, link.second);
        }
    }

//    if (controls_.contains(Uuid)) {
//        controls_[Uuid]->deleteLater();
//        controls_.remove(Uuid);
//    }

    pieces_[Uuid]->deleteLater();
    return pieces_.remove(Uuid);
}

void Synthetizer::connectPieces(QString from, QString to) {
    Connection link(from, to);
    links_.insert(from + to, link);

    qDebug() << link.first << link.second;

    if (!pieces_.contains(from) || !outputs_.contains(to))
        return;

    if (pieces_[from]->pieceType() == "Source") {
        Source *source = static_cast<Source*>(pieces_[from]);
        masterMixer_->addSource(source);
        qDebug() << source->pieceClass() << "to output";
    }
}

void Synthetizer::disconnectPieces(QString from, QString to)
{
    if (!links_.remove(from + to))
        return;

    if (!pieces_.contains(from) || !outputs_.contains(to))
        return;

    if (pieces_[from]->pieceType() == "Source") {
        Source *source = static_cast<Source*>(pieces_[from]);
        masterMixer_->removeSource(source);
    }
}

void Synthetizer::run()
{
    //AUDIO
    format_.setSampleRate(44100);
    format_.setChannelCount(2);
    format_.setSampleSize(16);
    format_.setCodec("audio/pcm");
    format_.setByteOrder(QAudioFormat::LittleEndian);
    format_.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format_)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    buffer_ = new AudioSamplesBuffer(44100, 1);
    buffer_->open(QIODevice::ReadWrite);

    player_ = new Player(format_, buffer_);
    player_->start();

    masterMixer_ = new MixerWorker(buffer_, true);
    masterMixer_->start();

    exec();
}
