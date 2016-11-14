#include "output.h"
#include <QUuid>
#include <QDebug>

Output::Output(QJsonObject *params, QObject *parent) :
    Sink("Output", params, parent)
{

}
