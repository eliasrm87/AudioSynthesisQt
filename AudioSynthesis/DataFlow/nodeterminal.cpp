#include "nodeterminal.h"
#include <QPen>
#include <QDebug>

NodeTerminal::NodeTerminal()
{

}

NodeTerminal::NodeTerminal(Node *node, quint8 index, bool input) :
    QRectF(), node_(node), input_(input)
{
    selected_ = false;

    if (input_) {
        nTerminals_ = node_->inputs();
    } else {
        nTerminals_ = node_->outputs();
    }

    quint16 left;
    if (input_) {
        left = node_->terminalBorderSize() / 2;
    } else {
        left = node->w() + node->borderSize() - node_->terminalBorderSize() - node_->terminalBorderSize() / 2;
    }

    inStep_ = (node->h() + node->borderSize() - (node_->terminalSize() + node_->terminalBorderSize()) * nTerminals_) / (nTerminals_ + 1);

    setRect(left, inStep_ * (index + 1) + (node_->terminalSize() + node_->terminalBorderSize()) * index + 1,
            node_->terminalSize(),
            node_->terminalSize());
}

NodeTerminal::NodeTerminal(const NodeTerminal &other) :
    QRectF(other)
{
    node_ = other.node_;
    input_ = other.input_;
    selected_ = other.selected_;
    nTerminals_ = other.nTerminals_;
    inStep_ = other.inStep_;
}

void NodeTerminal::draw(QPainter *painter)
{
    QPainterPath path;
    QColor color(220, 219, 217);
    if (selected_) {
        color = QColor(142, 0, 0);
    }
    path.addRect(*this);
    QPen pen(node_->borderColor(), node_->terminalBorderSize());
    painter->setPen(pen);
    painter->fillPath(path, color);
    painter->drawPath(path);
}

bool NodeTerminal::selected() const
{
    return selected_;
}

void NodeTerminal::setSelected(bool selected)
{
    selected_ = selected;
}
