#include "loopnode.h"
#include <QDebug>

LoopNode::LoopNode(QPoint position, QGraphicsItem *parent) :
    Node("Loop", position, 0, 1, parent)
{
    init();
}

LoopNode::LoopNode(QJsonObject obj, QGraphicsItem *parent) :
    Node(obj, 0, 1, parent)
{
    init();
}

LoopNode::~LoopNode()
{

}

QJsonObject LoopNode::toJsonObj()
{
    return Node::toJsonObj();
}

void LoopNode::init()
{

}
