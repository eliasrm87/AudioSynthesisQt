#ifndef LINKLINE_H
#define LINKLINE_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QPen>
#include <QBrush>
#include <QPointF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class LinkLine : public QAbstractGraphicsShapeItem
{
public:
    LinkLine(QGraphicsItem* parent = 0);

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

    QPointF startPos() const;
    void setStartPos(const QPointF &startPos);

    QPointF endPos() const;
    void setEndPos(const QPointF &endPos);

    QRectF boundingRect() const;
    QPainterPath shape() const;

    void setPath(const QPointF &startPos, const QPointF &endPos);

protected:
    QPointF startPos_;
    QPointF endPos_;

private:
    QPainterPath path_;
    QPainterPath shape_;

};

#endif // LINKLINE_H
