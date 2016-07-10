#ifndef MIXERWORKER_H
#define MIXERWORKER_H

#include <QObject>
#include <QList>
#include <QThread>
#include "source.h"
#include "audiosamplesbuffer.h"

class MixerWorker : public QThread
{
    Q_OBJECT
public:
    explicit MixerWorker(AudioSamplesBuffer* buffer, bool master, QObject *parent = 0);
    ~MixerWorker();
    void addSource(Source* source);
    void removeSource(Source* source);

public slots:
    void quit();

signals:
    void clockUpdated(const quint64 tick);

protected:
    void run();
    inline float getSample16(const SamplesVector &samples, qint64 i);
    inline void mix16(SamplesVector &ch1, const SamplesVector &ch2);

private:
    bool isMaster_;
    QList<Source*> sources_;
    QMutex mtxSources_;
    QMutex mtxExit_;
    AudioSamplesBuffer* buffer_;

    float mix16Samples(float a, float b);
};

#endif // MIXERWORKER_H
