#ifndef SYNPART_H
#define SYNPART_H

#include <QObject>
#include <QJsonObject>

class SynPiece : public QObject
{
    Q_OBJECT
public:
    SynPiece(QString pieceType, QString pieceClass, QJsonObject* params, QObject *parent = Q_NULLPTR);
    SynPiece(const SynPiece &synPart);
    QString pieceType() const;
    QString pieceClass() const;

protected:
    QString pieceType_;
    QString pieceClass_;
    QJsonObject* params_;

};

#endif // SYNPART_H
