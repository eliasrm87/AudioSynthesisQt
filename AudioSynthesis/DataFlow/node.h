#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QVector>
#include <QUuid>
#include <QJsonObject>
#include "nodeterminal.h"

class NodeTerminal;

class Node : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Node(QString nodeClass = "Node", QPoint position = QPoint(0, 0), quint8 inputs = 0, quint8 outputs = 0, QGraphicsItem * parent = 0);
    Node(const QJsonObject &obj, quint8 inputs, quint8 outputs, QGraphicsItem *parent = 0);
    ~Node();
    virtual QJsonObject toJsonObj();

    QString nodeClass() const;
    quint8 borderSize() const;
    void setBorderSize(const quint8 &borderSize);
    QColor borderColor() const;
    void setBorderColor(const QColor &borderColor);
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);
    QColor textColor() const;
    void setTextColor(const QColor &textColor);
    quint8 inputs() const;
    quint8 outputs() const;
    quint8 terminalSize() const;
    quint8 terminalBorderSize() const;
    void setSize(quint16 w, quint16 h);
    quint16 w() const;
    quint16 h() const;
    void deselectTerminals();
    NodeTerminal &inputTerminal(const quint8 &i);
    NodeTerminal &outputTerminal(const quint8 &i);
    QPixmap icon() const;
    void setIcon(const QPixmap &icon);
    QString text() const;
    void setText(const QString &text);
    QFont font() const;
    void setFont(const QFont &font);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QJsonObject* params();
    void setParams(const QJsonObject &params);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

protected:
    quint8 borderSize_;
    QColor borderColor_;
    QColor backgroundColor_;
    QColor textColor_;
    quint8 terminalSize_;
    quint8 terminalBorderSize_;
    QFont font_;
    quint16 w_;
    quint16 h_;
    quint8 inputs_;
    quint8 outputs_;
    qint16 selInput_;
    qint16 selOutput_;
    QJsonObject params_;

private:
    QImage image_;
    QPixmap icon_;
    QString text_;
    Qt::MouseButton button_;
    QVector<NodeTerminal> inputTerminals_;
    QVector<NodeTerminal> outputTerminals_;

    void init();
    void initInputsOutputs();
    bool selectTerminal(QVector<NodeTerminal> &terminals, qint16 &selected, QGraphicsSceneMouseEvent *event);

signals:
    void inputSelected(quint8 i);
    void inputDeselected(quint8 i);
    void outputSelected(quint8 i);
    void outputDeselected(quint8 i);

};

#endif // NODE_H
