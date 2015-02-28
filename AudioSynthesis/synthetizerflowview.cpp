#include "synthetizerflowview.h"
#include <AudioNodes/audionodes.h>

SynthetizerFlowView::SynthetizerFlowView(QWidget *parent) :
    DataFlowView(parent)
{

}

void SynthetizerFlowView::addNode(Node *node)
{
    DataFlowView::addNode(node);
}

Node *SynthetizerFlowView::newNodeFromJson(const QJsonObject &obj)
{
    QString nodeClass = obj.value("class").toString();

    if (nodeClass == "Oscillator") {
        return new OscillatorNode(obj);
    } else if (nodeClass == "Output") {
        return new OutputNode(obj);
    } else if (nodeClass == "Loop") {
        return new LoopNode(obj);
    }

    return DataFlowView::newNodeFromJson(obj);
}
