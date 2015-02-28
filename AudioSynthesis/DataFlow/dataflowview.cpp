#include "dataflowview.h"
#include <QDebug>

DataFlowView::DataFlowView(QWidget *parent) :
    QGraphicsView(parent)
{
    scene_ = new QGraphicsScene(this);
    setScene(scene_);

    connect(scene_, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));

    tmpInNode_ = 0;
    tmpOutNode_ = 0;

    //Work arround
    scene_->addText("");
}

DataFlowView::~DataFlowView()
{
    disconnect(scene_, 0, 0, 0);

    foreach (Node* node, nodes_) {
        delNode(node);
    }
}

void DataFlowView::fromJson(QJsonDocument doc)
{
    foreach (Node* node, nodes_) {
        delNode(node);
    }

    foreach (QJsonValue jsonNode, doc.object().value("nodes").toArray()) {
        Node* node = newNodeFromJson(jsonNode.toObject());
        addNode(node);
    }
    foreach (QJsonValue jsonLink, doc.object().value("links").toArray()) {
        NodeLinkLine* link = new NodeLinkLine(jsonLink.toObject(), nodes_);

        tmpInNode_      = link->toNode();
        tmpInTerminal_  = link->toTerminal();
        tmpOutNode_     = link->fromNode();
        tmpOutTerminal_ = link->fromTerminal();

        doConnection();
    }
}

void DataFlowView::addNode(Node *node)
{
    scene_->addItem(node);

    nodes_.insert(node->objectName(), node);

    connect(node, SIGNAL(inputSelected(quint8)),    this, SLOT(onInputSelected(quint8)));
    connect(node, SIGNAL(outputSelected(quint8)),   this, SLOT(onOutputSelected(quint8)));
    connect(node, SIGNAL(inputDeselected(quint8)),  this, SLOT(onInputDeselected(quint8)));
    connect(node, SIGNAL(outputDeselected(quint8)), this, SLOT(onOutputDeselected(quint8)));

    emit nodeAdded(node);
}

QJsonDocument DataFlowView::toJson()
{
    QJsonDocument doc;

    QJsonArray nodes;
    QJsonArray links;
    foreach (Node* node, nodes_) {
        nodes.append(node->toJsonObj());
        foreach (NodeLinkLine* link, outLinks_[node->objectName()]) {
            links.append(link->toJsonObj());
        }
    }

    QJsonObject mainObj;
    mainObj.insert("nodes", nodes);
    mainObj.insert("links", links);

    doc.setObject(mainObj);

    return doc;
}

void DataFlowView::delNode(Node *node)
{
    disconnect(node, 0, 0, 0);

    foreach (NodeLinkLine* link, outLinks_[node->objectName()]) {
        delLink(link);
    }
    foreach (NodeLinkLine* link, inLinks_[node->objectName()]) {
        delLink(link);
    }
    outLinks_.remove(node->objectName());
    inLinks_.remove(node->objectName());

    nodes_.remove(node->objectName());

    scene_->removeItem(node);

    qDebug() << "Deleted node" << node->objectName();

    emit nodeRemoved(node->objectName());

    disconnect(node, 0, 0, 0);
    node->deleteLater();
}

void DataFlowView::delLink(NodeLinkLine *link)
{
    outLinks_[link->fromNode()->objectName()].removeOne(link);
    inLinks_[link->toNode()->objectName()].removeOne(link);

    scene_->removeItem(link);

    qDebug() << "Deleted link" << link->objectName();

    emit linkRemoved(link->objectName());

    disconnect(link, 0, 0, 0);
    link->deleteLater();
}

void DataFlowView::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsView::keyReleaseEvent(event);

    switch (event->key()) {
    case Qt::Key_Delete:
        foreach (NodeLinkLine* selectedLink, selectedLinks_) {
            delLink(selectedLink);
        }
        foreach (Node* selectedNode, selectedNodes_) {
            delNode(selectedNode);
        }
        selectedNodes_.clear();
        selectedLinks_.clear();
        break;
    default:
        break;
    }
}

Node *DataFlowView::newNodeFromJson(const QJsonObject &obj)
{
    QString nodeClass = obj.value("class").toString();
    qDebug() << "WARN: Constructing a basic node for" << nodeClass;
    return new Node(obj, 0, 0);
}

void DataFlowView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

    QPointF pos = mapToScene(event->localPos().x(), event->localPos().y());
    mousePosition_ = QPoint(pos.x(), pos.y());
}

void DataFlowView::onSelectionChanged()
{
    QList<QGraphicsItem*> items = scene_->selectedItems();

    selectedNodes_.clear();
    selectedLinks_.clear();

    foreach (QGraphicsItem* item, items) {
        QString type = item->data(0).toString();
        if (type == "Node") {
            selectedNodes_.push_back((Node*)item);
            emit nodeSelected((Node*)item);
        } else if (type == "NodeLinkLine") {
            selectedLinks_.push_back((NodeLinkLine*)item);
        }
    }
}

void DataFlowView::onInputSelected(quint8 i)
{
    tmpInNode_ = (Node*)QObject::sender();
    tmpInTerminal_ = i;
    qDebug() << tmpInNode_->objectName() << "in" << i;
    doConnection();
}

void DataFlowView::onOutputSelected(quint8 i)
{
    tmpOutNode_ = (Node*)QObject::sender();
    tmpOutTerminal_ = i;
    qDebug() << tmpOutNode_->objectName() << "out" << i;
    doConnection();
}

void DataFlowView::onInputDeselected(quint8 i)
{
    tmpInNode_ = 0;
}

void DataFlowView::onOutputDeselected(quint8 i)
{
    tmpOutNode_ = 0;
}

QPoint DataFlowView::mousePosition() const
{
    return mousePosition_;
}

void DataFlowView::doConnection()
{
    if ((tmpInNode_ == 0) || (tmpOutNode_ == 0))
        return;

    foreach (NodeLinkLine* link, outLinks_[tmpOutNode_->objectName()]) {
        if ((link->fromNode() == tmpOutNode_) && (link->toNode() == tmpInNode_))
            return;
    }

    NodeLinkLine* link = new NodeLinkLine(tmpOutNode_, tmpOutTerminal_, tmpInNode_, tmpInTerminal_);
    scene_->addItem(link);

    tmpInNode_->deselectTerminals();
    tmpOutNode_->deselectTerminals();

    outLinks_[tmpOutNode_->objectName()].push_back(link);
    inLinks_[tmpInNode_->objectName()].push_back(link);

    qDebug() << "Added link from " + tmpOutNode_->objectName() + " to " + tmpInNode_->objectName();

    emit linkAdded(link);

    tmpInNode_ = 0;
    tmpOutNode_ = 0;
}
