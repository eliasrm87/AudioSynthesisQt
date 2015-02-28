#ifndef LOOP_H
#define LOOP_H

#include <QFile>
#include "source.h"

class Loop : public Source
{
    Q_OBJECT
public:
    Loop(QJsonObject *params, QObject* parent = 0);
    Loop(const Loop &loop);
    ~Loop();
    void getSamples(SamplesVector &dest, const qint32 &size);
    const SamplesVector &getSamples();
    QString path() const;

public slots:
    void setPath(const QString &path);
    void addPart(const quint64 &part);
    void delPart(const quint64 &part);

protected:
    void synthesize();

private:
    QString path_;
    QMutex mtx_;
    QList<quint64> parts_;
};

#endif // LOOP_H
