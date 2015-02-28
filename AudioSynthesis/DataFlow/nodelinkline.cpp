#include "nodelinkline.h"

#include <QDebug>

NodeLinkLine::NodeLinkLine(Node *fromNode, quint8 fromTerminal, Node *toNode, quint8 toTerminal, QGraphicsItem* parent) :
    LinkLine(parent), fromNode_(fromNode), fromTerminal_(fromTerminal), toNode_(toNode), toTerminal_(toTerminal)
{
    setObjectName(QUuid::createUuid().toString());

    init();
}

NodeLinkLine::NodeLinkLine(QJsonObject obj, const QHash<QString, Node *> &nodes, QGraphicsItem *parent)
{
    setObjectName(obj.value("uuid").toString());

    fromNode_ = nodes[obj.value("fromNode").toString()];
    fromTerminal_ = obj.value("fromTerminal").toInt();
    toNode_ = nodes[obj.value("toNode").toString()];
    toTerminal_ = obj.value("toTerminal").toInt();

    init();
}

void NodeLinkLine::init()
{
    setData(0, "NodeLinkLine");
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void NodeLinkLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPointF startPos = fromNode_->sceneBoundingRect().topLeft() +
            fromNode_->outputTerminal(fromTerminal_).topRight() +
            QPointF(0, fromNode_->terminalBorderSize() * 2);
    QPointF endPos = toNode_->sceneBoundingRect().topLeft() +
            toNode_->inputTerminal(toTerminal_).topLeft() +
            QPointF(0, toNode_->terminalBorderSize() * 2);

    setPath(startPos, endPos);

    LinkLine::paint(painter, option, widget);
}

QJsonObject NodeLinkLine::toJsonObj()
{
    QJsonObject obj;
    obj.insert("uuid", objectName());
    obj.insert("fromNode", fromNode_->objectName());
    obj.insert("fromTerminal", fromTerminal_);
    obj.insert("toNode", toNode_->objectName());
    obj.insert("toTerminal", toTerminal_);
    return obj;
}

Node *NodeLinkLine::fromNode() const
{
    return fromNode_;
}

quint8 NodeLinkLine::fromTerminal() const
{
    return fromTerminal_;
}

Node *NodeLinkLine::toNode() const
{
    return toNode_;
}

quint8 NodeLinkLine::toTerminal() const
{
    return toTerminal_;
}

