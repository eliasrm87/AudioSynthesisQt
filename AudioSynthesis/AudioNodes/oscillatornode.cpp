#include "oscillatornode.h"

#include <QtMath>
#include <QApplication>
#include <QDebug>

OscillatorNode::OscillatorNode(QPoint position, QGraphicsItem * parent) :
    Node("Oscillator", position, 0, 1, parent)
{
    init();
}

OscillatorNode::OscillatorNode(const QJsonObject &obj, QGraphicsItem *parent) :
    Node(obj, 0, 1, parent)
{
    init();
}

OscillatorNode::~OscillatorNode()
{

}

void OscillatorNode::init()
{

}
