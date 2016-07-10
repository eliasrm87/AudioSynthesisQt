#include "mixerworker.h"

#include <QtMath>

MixerWorker::MixerWorker(AudioSamplesBuffer *buffer, bool master, QObject *parent) :
    QThread(parent), isMaster_(master), buffer_(buffer)
{
    mtxExit_.lock();
}

MixerWorker::~MixerWorker()
{

}

void MixerWorker::addSource(Source *source)
{
    QMutexLocker locker(&mtxSources_);
    sources_.push_back(source);
}

void MixerWorker::removeSource(Source *source)
{
    QMutexLocker locker(&mtxSources_);
    sources_.removeAll(source);
}

void MixerWorker::quit()
{
    mtxExit_.unlock();
    QThread::quit();
}

void MixerWorker::run()
{
    quint64 nSamples = 441;

    qint64 w = 0;
    int nSources;
    SamplesVector result, tmpArray;
    bool first = true;
    mtxSources_.lock();
    while(!mtxExit_.tryLock(1)) {
        result.clear();
        nSources = sources_.size();
        if (nSources > 0) {
            if (isMaster_)
                emit clockUpdated(Source::incSamplesClock(w));
            for (int i = 0; i < nSources; ++i) {
                sources_[i]->getSamples(tmpArray, nSamples);
                mix16(result, tmpArray);
            }
            w = buffer_->write16(result);
        } else {
            result.fill(0, nSamples);
            w = buffer_->write16(result);
        }
        if (w < result.size()) {
            if ((first) && (nSources > 0)) {
                first = false;
                buffer_->swap();
            }
            mtxSources_.unlock();
            QThread::msleep(buffer_->seconds() * 2);
            mtxSources_.lock();
        }
    }
}

float MixerWorker::getSample16(const SamplesVector &samples, qint64 i)
{
    if (i >= samples.size())
        return 0;

    return samples[i];
}

void MixerWorker::mix16(SamplesVector &ch1, const SamplesVector &ch2)
{
    SamplesVector result;

    int size = ch2.size();
    result.reserve(size);

    for (int i = 0; i < size; i++) {
        result.push_back(mix16Samples(getSample16(ch1, i), getSample16(ch2, i)));
    }

    ch1 = result;
}

#define THRESHOLD 0.6f
#define ALPHA 7.48338f
// http://www.voegler.eu/pub/audio/digital-audio-mixing-and-normalization.html#idx_6_2
inline float MixerWorker::mix16Samples(float a, float b)
{
    qreal sum = a + b;
    qreal mSum = qFabs(sum);

    if (mSum > THRESHOLD)
        sum = (sum/mSum) * (THRESHOLD + (1.0f - THRESHOLD) * (qLn(1.0f + ALPHA * ((mSum - THRESHOLD)/(2.0f - THRESHOLD))) / qLn(1.0f + ALPHA)));

    return sum;
}
