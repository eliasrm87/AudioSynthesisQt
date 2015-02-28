#ifndef AUDIOSAMPLESBUFFER_H
#define AUDIOSAMPLESBUFFER_H

#include <QIODevice>
#include <QAudioFormat>
#include <QBuffer>
#include <QMutexLocker>
#include <QVector>
#include <QDebug>

/**
 * @brief Buffer de audio PCM estereo a 16 bits (Little endian)
 */

class AudioSamplesBuffer : public QIODevice
{
public:
    AudioSamplesBuffer(quint16 sampleRate, quint8 seconds = 1, QObject* parent = 0);
    ~AudioSamplesBuffer();
    bool isSequential() const;
    quint32 bufferSize() const;
    void setBufferSize(const quint32 &bufferSize);
    quint8 seconds() const;
    quint16 sampleRate() const;
    void swap();
    qint64 write16(const QVector<float> &floatArray);

protected:
    qint64 readData(char* data, qint64 maxSize);
    qint64 writeData(const char* data, qint64 maxSize);

private:
    quint16 sampleRate_;
    quint32 bufferSize_;
    char* bufferR_;
    char* bufferW_;
    qint64 usedBytesR_;
    qint64 usedBytesW_;
    qint64 posW_;
    qint64 posR_;
    quint8 seconds_;
    QByteArray tmpArray_;
    QMutex mutex_;

};

#endif // AUDIOSAMPLESBUFFER_H
