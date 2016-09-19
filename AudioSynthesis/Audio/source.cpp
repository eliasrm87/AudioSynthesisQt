#include "source.h"
#include <QDebug>

QMutex* Source::staticVarsMtx_ = new QMutex();
quint16 Source::sampleRate_    = 44100;
quint64 Source::samplesClock_  = 0;
quint16 Source::tempo_         = 120;
Sign    Source::sign_          = Sign(4,4);
quint32 Source::blackLength_   = 22050;
quint32 Source::beatLength_    = 22050;
quint32 Source::signLength_    = 88200;

Source::Source(const QString &sourceClass, QJsonObject *params, QObject *parent) :
    SynPiece("Source", sourceClass, params, parent)
{
    amplitude_ = params_->value("amplitude").toDouble(1.0f);

    init();
}

Source::Source(const Source &source) :
    SynPiece(source)
{
    setParent(source.parent());

    samplesBuffer_ = source.samplesBuffer_;
    pos_ = source.pos_;
    sampleRate_ = source.sampleRate_;
    amplitude_ = source.amplitude_;

    init();
}

Source::~Source()
{

}

void Source::getSamples(SamplesVector &dest, const qint32 &size)
{
    if (!synthesized_) {
        synthesized_ = true;
        synthesize();
    }

    if (size > samplesBuffer_.size())
        qDebug() << "ERROR" << samplesBuffer_.size();

    {
        QMutexLocker locker(staticVarsMtx_);
        pos_ = samplesClock_ % beatLength_;
    }

    if (pos_ >= samplesBuffer_.size()) {
        dest.fill(0, size);
        return;
    }

    qint64 remaining = samplesBuffer_.size() - pos_;
    if (size > remaining) {
        dest = samplesBuffer_.mid(pos_);
        SamplesVector filling;
        filling.fill(0, size - remaining);
        dest.append(filling);
    } else {
        dest = samplesBuffer_.mid(pos_, size);
    }
}

void Source::setAmplitude(float amplitude)
{
    amplitude_ = amplitude;
    params_->insert("amplitude", amplitude);
    synthesize();
}

float Source::getAmplitude() const
{
    return amplitude_;
}

const SamplesVector &Source::getSamples()
{
    if (!synthesized_) {
        synthesized_ = true;
        synthesize();
    }

    return samplesBuffer_;
}

quint64 Source::incSamplesClock(quint64 size)
{
    samplesClock_ += size;
    return samplesClock_;
}

quint16 Source::getTempo()
{
    return tempo_;
}

void Source::setTempo(const quint16 &tempo)
{
    QMutexLocker locker(staticVarsMtx_);

    tempo_ = tempo;
    updateVariables();
}

Sign Source::getSign()
{
    return sign_;
}

void Source::setSign(QPair<quint8, quint8> sign)
{
    QMutexLocker locker(staticVarsMtx_);

    sign_ = sign;
    updateVariables();
}

void Source::synthesize()
{
    qDebug() << "Source::synthesize";

    emit samplesUpdated(samplesBuffer_);
}

void Source::init()
{
    synthesized_ = false;
}

quint16 Source::getSampleRate()
{
    return sampleRate_;
}

void Source::setSampleRate(const quint16 &sampleRate)
{
    QMutexLocker locker(staticVarsMtx_);

    sampleRate_ = sampleRate;
    updateVariables();
}

void Source::updateVariables()
{
    blackLength_ = (60.0f / (float)tempo_) * (float)sampleRate_;
    beatLength_  = (1.0f / (float)sign_.second) * 4.0f * (float)blackLength_;
    signLength_  = beatLength_ * sign_.first;
}
