#ifndef SYNTHETIZERFLOWVIEW_H
#define SYNTHETIZERFLOWVIEW_H

#include <DataFlow/dataflowview.h>

class SynthetizerFlowView : public DataFlowView
{
    Q_OBJECT
public:
    SynthetizerFlowView(QWidget * parent = 0);
    virtual void addNode(Node* node);
    Node* newNodeFromJson(const QJsonObject &obj);

};

#endif // SYNTHETIZERFLOWVIEW_H
