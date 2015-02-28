#ifndef LOOPCONTROLS_H
#define LOOPCONTROLS_H

#include <QPushButton>
#include <QLineEdit>

#include "partcontrols.h"
#include "waveform.h"

class LoopControls : public PartControls
{
    Q_OBJECT
public:
    explicit LoopControls(QWidget *parent = 0);

signals:
    void pathChanged(QString path);

public slots:
    void setWaveForm(const QVector<float> &samples);
    void setPath(QString path);

private slots:
    void onExplore();

private:
    QLineEdit* txtLoopPath_;
    QPushButton* btnExplore_;
    Waveform* wave_;

};

#endif // LOOPCONTROLS_H
