#include "audiosamplesbuffer.h"

AudioSamplesBuffer::AudioSamplesBuffer(quint16 sampleRate, quint8 seconds, QObject *parent) :
    QIODevice(parent), sampleRate_(sampleRate), seconds_(seconds)
{
    bufferR_ = 0;
    bufferW_ = 0;

    setBufferSize(sampleRate_ * 2 * seconds_);

    usedBytesR_ = 0;
    usedBytesW_ = 0;

    posR_ = 0;
    posW_ = 0;
}

AudioSamplesBuffer::~AudioSamplesBuffer()
{
    if (bufferR_ != 0)
        delete bufferR_;
    if (bufferW_ != 0)
        delete bufferW_;
}

bool AudioSamplesBuffer::isSequential() const
{
    return true;
}

qint64 AudioSamplesBuffer::readData(char *data, qint64 maxSize)
{
    qint64 size = maxSize;
    if (maxSize > usedBytesR_)
        size = usedBytesR_;
    usedBytesR_ -= size;

    if (size > 0) {
        qint64 remaining = bufferSize_ - posR_;
        if (size > remaining) {
            memcpy(data, bufferR_ + posR_, remaining);
            memcpy(data + remaining, bufferR_, size - remaining);
        } else {
            memcpy(data, bufferR_ + posR_, size);
        }

        posR_ = (posR_+size)%bufferSize_;

        if (usedBytesR_ == 0)
            swap();
    }

    return size;
}

qint64 AudioSamplesBuffer::writeData(const char *data, qint64 maxSize)
{
    QMutexLocker locker(&mutex_);

    if (maxSize > (bufferSize_ - usedBytesW_)) {
        maxSize = (bufferSize_ - usedBytesW_);
    }
    usedBytesW_ += maxSize;

    qint64 remaining = bufferSize_ - posW_;
    if (maxSize > remaining) {
        memcpy(bufferW_ + posW_, data, remaining);
        memcpy(bufferW_, data + remaining, maxSize - remaining);
    } else {
        memcpy(bufferW_ + posW_, data, maxSize);
    }

    posW_ = (posW_+maxSize)%bufferSize_;

    return maxSize;
}

quint16 AudioSamplesBuffer::sampleRate() const
{
    return sampleRate_;
}

quint8 AudioSamplesBuffer::seconds() const
{
    return seconds_;
}

void AudioSamplesBuffer::swap()
{
    QMutexLocker locker(&mutex_);

    char*  tmpBuffer = bufferR_;
    bufferR_         = bufferW_;
    bufferW_         = tmpBuffer;

    posR_       = 0;
    usedBytesR_ = usedBytesW_;
    posW_       = 0;
    usedBytesW_ = 0;
}

qint64 AudioSamplesBuffer::write16(const QVector<float> &floatArray)
{
    tmpArray_.clear();
    qint16 value16;
    foreach (float value, floatArray) {
        value16 = INT16_MAX * value;
        tmpArray_.push_back(value16);
        tmpArray_.push_back(value16 >> 8);
    }

    return write(tmpArray_) / 2;
}

quint32 AudioSamplesBuffer::bufferSize() const
{
    return bufferSize_;
}

void AudioSamplesBuffer::setBufferSize(const quint32 &bufferSize)
{
    if (bufferR_ != 0)
        delete bufferR_;

    if (bufferW_ != 0)
        delete bufferW_;

    bufferSize_ = bufferSize;
    //Inicializa los buffers
    bufferR_ = new char[bufferSize_];
    memset(bufferR_, 0, bufferSize_);

    bufferW_ = new char[bufferSize_];
    memset(bufferW_, 0, bufferSize_);
}
