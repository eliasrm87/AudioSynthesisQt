#ifndef OSCILLATORNODE_H
#define OSCILLATORNODE_H

#include <DataFlow/node.h>

class OscillatorNode : public Node
{
    Q_OBJECT
public:
    OscillatorNode(QPoint position = QPoint(0, 0), QGraphicsItem * parent = 0);
    OscillatorNode(const QJsonObject &obj, QGraphicsItem *parent = 0);
    ~OscillatorNode();

private:
    void init();
};

#endif // OSCILLATORNODE_H
