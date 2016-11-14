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

public slots:
    void addPiece(SynPiece *piece);
    SynPiece* getPiece(QString Uuid);
    int removePiece(QString Uuid);
    void connectPieces(QString from, QString to);
    void disconnectPieces(QString from, QString to);

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
};

#endif // SYNTHETIZER_H
