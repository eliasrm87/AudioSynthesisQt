#ifndef OUTPUTNODE_H
#define OUTPUTNODE_H

#include <DataFlow/node.h>

class OutputNode : public Node
{
    Q_OBJECT
public:
    OutputNode(QPoint position = QPoint(0, 0), QGraphicsItem * parent = 0);
    OutputNode(QJsonObject obj, QGraphicsItem *parent = 0);
    ~OutputNode();

private:
    void init();

};

#endif // OUTPUTNODE_H
