#include "synpiece.h"
#include <QUuid>
#include <QDebug>

SynPiece::SynPiece(QString pieceType, QString pieceClass, QJsonObject *params, QObject *parent) :
    QObject(parent), pieceType_(pieceType), pieceClass_(pieceClass), params_(params)
{
    setObjectName(QUuid::createUuid().toString());
}

SynPiece::SynPiece(const SynPiece &synPart) :
    QObject(synPart.parent())
{
    setParent(synPart.parent());
    setObjectName(synPart.objectName());

    pieceType_  = synPart.pieceType_;
    pieceClass_ = synPart.pieceClass_;
    params_     = synPart.params_;
}

QString SynPiece::pieceType() const
{
    return pieceType_;
}

QString SynPiece::pieceClass() const
{
    return pieceClass_;
}
