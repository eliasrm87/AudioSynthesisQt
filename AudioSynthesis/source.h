#ifndef SOURCE_H
#define SOURCE_H

#include <QObject>
#include <QFile>

class Source : public QObject
{
    Q_OBJECT

public:
    Source(QString fileName, QObject *parent = 0);
    Source(const Source &source);
    ~Source();
    const qint64& size();
    void getSamples(QByteArray &dest, qint32 size);
    void incPosition(qint32 size);

protected:
    qint64 pos_;
    qint64 size_;
    QByteArray samples_;
    QByteArray* processedSamples_;

};

#endif // SOURCE_H
