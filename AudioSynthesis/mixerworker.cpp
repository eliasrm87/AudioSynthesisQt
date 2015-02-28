#include "mixerworker.h"

MixerWorker::MixerWorker(AudioSamplesBuffer *buffer, QObject *parent) :
    QObject(parent), buffer_(buffer)
{

}

MixerWorker::~MixerWorker()
{

}

void MixerWorker::addSource(Source *source)
{
    sources_.push_back(source);
}

void MixerWorker::start()
{
    quint64 nSamples = 4096;

    qint64 w = 0;
    int nSources;
    QByteArray result, tmpArray;
    bool first = true;
    while (true) {
        result.clear();
        nSources = sources_.size();
        if (nSources > 0) {
            for (int i = 0; i < nSources; ++i) {
                sources_[i]->incPosition(w);
                sources_[i]->getSamples(tmpArray, nSamples);
                mix16(result, tmpArray);
            }
            w = buffer_->write(result);
        }
        if (w < result.size()) {
            if ((first) && (nSources > 0)) {
                first = false;
                buffer_->swap();
            }
            QThread::msleep(10);
        }
    }
}

qint16 MixerWorker::getSample16(const QByteArray &samples, qint64 i)
{
    i *= 2;

    if ((i+1) > samples.size())
        return 0;

    return *((qint16*)(samples.data() + i));
}

void MixerWorker::mix16(QByteArray &ch1, const QByteArray &ch2)
{
    QByteArray result;

    int size = ch2.size() / 2;
    qint32 sample;
    for (int i = 0; i < size; ++i) {
        sample = getSample16(ch1, i);
        sample += getSample16(ch2, i);

        sample /= 2;

        result.push_back(sample & 0x00FF);
        result.push_back((sample & 0xFF00) >> 8);
    }

    ch1 = result;
}

