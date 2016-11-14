#ifndef SINK_H
#define SINK_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include "synpiece.h"

class Sink : public SynPiece
{
    Q_OBJECT
public:
    Sink(QString sinkClass, QJsonObject *params, QObject* parent = 0);
};

#endif // SINK_H
