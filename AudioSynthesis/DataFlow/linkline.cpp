#include "linkline.h"

#include <QRectF>
#include <QPainterPath>
#include <QDebug>

LinkLine::LinkLine(QGraphicsItem * parent) :
    QAbstractGraphicsShapeItem(parent)
{
    QPen pen;
    pen.setWidth(4);
    pen.setColor(QColor(127, 127, 127));
    setPen(pen);

    startPos_ = QPointF(1,1);
    endPos_ = QPointF(1,1);

    setPath(QPointF(0,0), QPointF(100,100));
}

void LinkLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    QPen p = pen();
    if (option->state & QStyle::State_Selected)
        p.setColor(QColor(127, 0, 0));
    else
        p.setColor(QColor(127, 127, 127));
    setPen(p);

    painter->setPen(p);
    painter->setBrush(brush());
    painter->drawPath(path_);
}

QPointF LinkLine::startPos() const
{
    return startPos_;
}

void LinkLine::setStartPos(const QPointF &startPos)
{
    startPos_ = startPos;
}

QPointF LinkLine::endPos() const
{
    return endPos_;
}

void LinkLine::setEndPos(const QPointF &endPos)
{
    endPos_ = endPos;
}

QRectF LinkLine::boundingRect() const
{
    return shape_.controlPointRect();
}

QPainterPath LinkLine::shape() const
{
    return shape_;
}

void LinkLine::setPath(const QPointF &startPos, const QPointF &endPos)
{
    if ((startPos == startPos_) && (endPos == endPos_))
        return;

    prepareGeometryChange();

    startPos_ = startPos;
    endPos_   = endPos;

    QRectF pathRect = QRectF(startPos_, endPos_);

    path_ = QPainterPath(pathRect.topLeft());
    path_.cubicTo(pathRect.topRight(),
                  pathRect.bottomLeft(),
                  pathRect.bottomRight());

    QPainterPathStroker ps;
    ps.setCapStyle(pen().capStyle());
    ps.setWidth(pen().widthF());
    ps.setJoinStyle(pen().joinStyle());
    ps.setMiterLimit(pen().miterLimit());
    shape_ = ps.createStroke(path_);
    shape_.addPath(path_);

    update();
}

