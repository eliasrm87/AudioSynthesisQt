#ifndef OUTPUT_H
#define OUTPUT_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include "sink.h"

class Output : public Sink
{
    Q_OBJECT
public:
    Output(QJsonObject *params, QObject* parent = 0);
};

#endif // OUTPUT_H
