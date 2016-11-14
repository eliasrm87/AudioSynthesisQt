#include "sink.h"
#include <QUuid>
#include <QDebug>

Sink::Sink(QString sinkClass, QJsonObject *params, QObject *parent) :
    SynPiece("Sink", sinkClass, params, parent)
{

}
