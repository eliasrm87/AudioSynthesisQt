#ifndef NODELINKLINE_H
#define NODELINKLINE_H

#include "linkline.h"
#include "node.h"

class NodeLinkLine : public QObject, public LinkLine
{
    Q_OBJECT
public:
    NodeLinkLine(Node* fromNode, quint8 fromTerminal, Node* toNode, quint8 toTerminal, QGraphicsItem* parent = 0);
    NodeLinkLine(QJsonObject obj, const QHash<QString, Node *> &nodes, QGraphicsItem* parent = 0);
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
    QJsonObject toJsonObj();

    Node *fromNode() const;
    quint8 fromTerminal() const;
    Node *toNode() const;
    quint8 toTerminal() const;

private:
    Node* fromNode_;
    quint8 fromTerminal_;
    Node* toNode_;
    quint8 toTerminal_;

    void init();

signals:
    void selected(bool selected);

};

#endif // NODELINKLINE_H
