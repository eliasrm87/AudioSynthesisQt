#ifndef DATAFLOWVIEW_H
#define DATAFLOWVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHash>
#include <QList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QKeyEvent>
#include "node.h"
#include "nodelinkline.h"

class DataFlowView : public QGraphicsView
{
    Q_OBJECT
public:
    DataFlowView(QWidget * parent = 0);
    ~DataFlowView();
    virtual void addNode(Node* node);
    QJsonDocument toJson();
    void fromJson(QJsonDocument doc);
    QPoint mousePosition() const;
    virtual Node* newNodeFromJson(const QJsonObject &obj);

protected:
    void keyReleaseEvent(QKeyEvent * event);
    void mousePressEvent(QMouseEvent * event);

private slots:
    void onSelectionChanged();
    void onInputSelected(quint8 i);
    void onOutputSelected(quint8 i);
    void onInputDeselected(quint8 i);
    void onOutputDeselected(quint8 i);

private:
    QPoint mousePosition_;
    QGraphicsScene* scene_;
    QHash<QString, Node*> nodes_;
    QHash<QString, QList<NodeLinkLine*> > outLinks_;
    QHash<QString, QList<NodeLinkLine*> > inLinks_;

    Node* tmpInNode_;
    qint8 tmpInTerminal_;
    Node* tmpOutNode_;
    qint8 tmpOutTerminal_;

    QList<Node*> selectedNodes_;
    QList<NodeLinkLine*> selectedLinks_;

    void doConnection();
    void delNode(Node* node);
    void delLink(NodeLinkLine* link);

signals:
    void nodeAdded(Node* node);
    void nodeRemoved(QString nodeUuid);
    void linkAdded(NodeLinkLine* link);
    void linkRemoved(QString linkUuid);
    void nodeSelected(Node* node);

};

#endif // DATAFLOWVIEW_H
