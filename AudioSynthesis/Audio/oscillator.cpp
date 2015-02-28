#include "oscillator.h"
#include <QtMath>
#include <QDebug>

#define MAX_16 65535
#define MAX_16_2 32767

Oscillator::Oscillator(QJsonObject *params, QObject *parent) :
    Source::Source("Oscillator", params, parent)
{
    frequency_ = params_->value("frequency").toDouble(440.0f);
}

Oscillator::Oscillator(const Oscillator &oscillator) :
    Source::Source(oscillator)
{
    frequency_ = oscillator.frequency_;
}

float Oscillator::frequency() const
{
    return frequency_;
}

void Oscillator::setFrequency(float freq)
{
    frequency_ = freq;
    params_->insert("frequency", freq);
    synthesize();
}

void Oscillator::synthesize()
{
    qDebug() << "Oscillator::synthesize" << frequency_ << sampleRate_;

    samplesBuffer_.clear();
    sine16(sampleRate_);
}

void Oscillator::sine16(quint64 samples)
{
    qreal t = 0;
    qreal T = (qreal)1.0f / (qreal)sampleRate_;

    quint64 i = 0;
    while (i < samples) {
        float sample = qSin((qreal)2.0f * (qreal)M_PI * frequency_ * t) * amplitude_;
        t += T;
        samplesBuffer_.push_back(sample);
        i++;
    }
}
