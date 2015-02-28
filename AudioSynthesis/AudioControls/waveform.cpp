#include "waveform.h"
#include "QPen"
#include <QPointF>
#include <QDebug>

WaveFormDiv::WaveFormDiv(QGraphicsItem *parent) :
    QGraphicsLineItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
}

Waveform::Waveform(QWidget *parent) :
    QGraphicsView(parent)
{
//    setMinimumSize(900, 250);

    pressedKey_ = 0;

    scene_ = new QGraphicsScene(this);
    setScene(scene_);
}

void Waveform::setWaveForm(const QVector<float> &samples)
{
    qDebug() << "Waveform::setWaveForm" << samples.size();

    waveFormMed_.clear();

    float med = 0;
    int step = 200;
    int j = 1;
    waveFormMed_.push_back(QPointF(0, 0));
    for (int i = 0; i < samples.size(); i += 2) {
        float v = samples[i] * 200.0f;
        if ((i % step) == 0) {
            waveFormMed_.push_back(QPointF(j, med / (float)step));
            med = v;
            j++;
        } else {
            med += v;
        }
    }
    waveFormMed_.push_back(QPointF(j, 0));

    scene_->clear();
    scene_->addPolygon(waveFormMed_, QPen(QColor(50,50,200), 1));
}

void Waveform::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);

    pressedKey_ = event->key();
}

void Waveform::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsView::keyReleaseEvent(event);

    switch (event->key()) {
    case Qt::Key_Delete:
        break;
    default:
        break;
    }

    pressedKey_ = 0;
}

void Waveform::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

    QPointF pos = mapToScene(event->localPos().x(), event->localPos().y());
    mousePosition_ = QPoint(pos.x(), pos.y());
}

void Waveform::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (pressedKey_ == Qt::Key_Control) {
            WaveFormDiv* div = new WaveFormDiv(0);
            divs_.insert(mousePosition_.x(), div);
            div->setPen(QPen(QBrush(QColor(255,0,0)), 5));
            div->setLine(mousePosition_.x(), -100, mousePosition_.x(), 100);

            scene_->addItem(div);

            return;
        }
    } else if (event->button() == Qt::RightButton) {

    }

    QGraphicsView::mouseReleaseEvent(event);
}
