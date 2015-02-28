#include "outputnode.h"

#include <QDebug>

OutputNode::OutputNode(QPoint position, QGraphicsItem * parent) :
    Node("Output", position, 1, 0, parent)
{
    init();
}

OutputNode::OutputNode(QJsonObject obj, QGraphicsItem *parent) :
    Node(obj, 1, 0, parent)
{
    init();
}

OutputNode::~OutputNode()
{

}

void OutputNode::init()
{
    backgroundColor_ = QColor(199, 233, 192);
}
