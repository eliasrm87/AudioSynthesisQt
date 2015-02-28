#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QGraphicsView>
#include <QPolygonF>
#include <QPoint>
#include <QMouseEvent>
#include <QGraphicsLineItem>
#include <QGraphicsItem>

class WaveFormDiv : public QGraphicsLineItem {
public:
    WaveFormDiv(QGraphicsItem *parent = Q_NULLPTR);

};

class Waveform : public QGraphicsView
{
    Q_OBJECT
public:
    Waveform(QWidget * parent = 0);

public slots:
    void setWaveForm(const QVector<float> &samples);

protected:
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

private:
    QGraphicsScene* scene_;
    QPolygonF waveFormMed_;
    QPolygonF waveFormMax_;
    int pressedKey_;
    QPoint mousePosition_;
    QMap<int, WaveFormDiv*> divs_;
};

#endif // WAVEFORM_H
