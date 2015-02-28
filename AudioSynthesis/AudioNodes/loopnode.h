#ifndef LOOPNODE_H
#define LOOPNODE_H

#include <DataFlow/node.h>

class LoopNode : public Node
{
    Q_OBJECT
public:
    LoopNode(QPoint position = QPoint(0, 0), QGraphicsItem * parent = 0);
    LoopNode(QJsonObject obj, QGraphicsItem *parent = 0);
    ~LoopNode();
    QJsonObject toJsonObj();

private:
    void init();

};

#endif // LOOPNODE_H
