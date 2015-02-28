#ifndef MIXERWORKER_H
#define MIXERWORKER_H

#include <QObject>
#include <QList>
#include <QThread>
#include "source.h"
#include "audiosamplesbuffer.h"

class MixerWorker : public QObject
{
    Q_OBJECT
public:
    explicit MixerWorker(AudioSamplesBuffer* buffer, QObject *parent = 0);
    ~MixerWorker();
    void addSource(Source* source);

public slots:
    void start();

protected:
    qint16 getSample16(const QByteArray &samples, qint64 i);
    void mix16(QByteArray &ch1, const QByteArray &ch2);

private:
    QList<Source*> sources_;
    AudioSamplesBuffer* buffer_;

};

#endif // MIXERWORKER_H
