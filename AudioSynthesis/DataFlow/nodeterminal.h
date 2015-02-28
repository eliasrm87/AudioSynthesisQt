#ifndef NODETERMINAL_H
#define NODETERMINAL_H

#include <QRectF>
#include <QPainter>
#include "node.h"

class Node;

class NodeTerminal : public QRectF
{
public:
    NodeTerminal();
    NodeTerminal(Node* node, quint8 index, bool input);
    NodeTerminal(const NodeTerminal &other);
    void draw(QPainter *painter);

    bool selected() const;
    void setSelected(bool selected);

private:
    Node* node_;
    bool input_;
    quint8 nTerminals_;
    quint8 inStep_;
    bool selected_;
};

#endif // NODETERMINAL_H
