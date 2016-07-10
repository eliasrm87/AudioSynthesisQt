#ifndef SYNTHETIZER_H
#define SYNTHETIZER_H

#include <QThread>
#include <QAudioOutput>
#include <QPair>
#include <AudioControls/audiocontrols.h>
#include "audio.h"

class Synthetizer : public QThread
{
    Q_OBJECT
public:
    typedef QPair<QString, QString> Connection;


public:
    Synthetizer(QObject *parent = Q_NULLPTR);
    ~Synthetizer();
    PartControls *getControls(QString Uuid) const;

public slots:
    SynPiece* addPiece(QString piece, QJsonObject *params, QString Uuid);
    SynPiece* getPiece(QString Uuid);
    int removePiece(QString Uuid);
    void connectPieces(QString from, QString to);
    void disconnectPieces(QString from, QString to);

//    void addNode(Node* node);
//    void removeNode(QString nodeUuid);
//    void addLink(NodeLinkLine* nodeLink);
//    void removeLink(QString linkUuid);

protected:
    void run();

private:
    QAudioFormat format_;
    Player* player_;
    MixerWorker* masterMixer_;
    AudioSamplesBuffer* buffer_;
    QHash<QString, SynPiece*> pieces_;
    QList<QString> outputs_;
    QHash<QString, Connection> links_;
    QHash<QString, PartControls*> controls_;

//    QHash<QString, Node*> nodes_;
//    QHash<QString, QPair<QString, QString> > links_;
//    QHash<QString, Oscillator*> oscillators_;
//    QHash<QString, Loop*> loops_;
};

#endif // SYNTHETIZER_H
