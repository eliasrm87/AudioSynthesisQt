#ifndef SOURCE_H
#define SOURCE_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include "synpiece.h"

typedef QVector<float> SamplesVector;

class Source : public SynPiece
{
    Q_OBJECT
public:
    Source(const QString &sourceClass, QJsonObject *params, QObject* parent = 0);
    Source(const Source &source);
    ~Source();
    virtual void getSamples(SamplesVector &dest, const qint32 &size);
    void setAmplitude(float amplitude);
    float getAmplitude() const;
    virtual const SamplesVector &getSamples();

protected:
    bool synthesized_;
    quint64 pos_;
    float amplitude_;
    SamplesVector samplesBuffer_;
    virtual void synthesize();

signals:
    void samplesUpdated(const QVector<float> &samples);

private:
    void init();

//STATIC CLASS
public:
    static void incSamplesClock(quint64 size);
    static quint16 getTempo();
    static void setTempo(const quint16 &tempo);
    static QPair<quint8, quint8> getSign();
    static void setSign(QPair<quint8, quint8> sign);
    static quint16 getSampleRate();
    static void setSampleRate(const quint16 &sampleRate);

protected:
    static QMutex* staticVarsMtx_;
    static quint16 sampleRate_;
    static quint64 samplesClock_;
    static quint16 tempo_;
    static QPair<quint8, quint8> sign_;
    static quint32 blackLength_;
    static quint32 beatLength_;
    static quint32 signLength_;
    static void updateVariables();

};

#endif // SOURCE_H
