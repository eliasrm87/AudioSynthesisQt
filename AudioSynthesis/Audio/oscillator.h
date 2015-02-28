#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "source.h"

class Oscillator : public Source
{
    Q_OBJECT
public:
    Oscillator(QJsonObject *params, QObject *parent = 0);
    Oscillator(const Oscillator &oscillator);
    float frequency() const;

public slots:
    void setFrequency(float frequency);

protected:
    void synthesize();

private:
    float frequency_;

    void sine16(quint64 samples);
};

#endif // OSCILLATOR_H
