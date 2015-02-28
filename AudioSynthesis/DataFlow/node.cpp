#include "node.h"

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPen>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

Node::Node(QString nodeClass, QPoint position, quint8 inputs, quint8 outputs, QGraphicsItem *parent) :
    QGraphicsItem(parent), inputs_(inputs), outputs_(outputs)
{
    setObjectName(QUuid::createUuid().toString());

    setData(1, nodeClass);

    setPos(position.x(), position.y());

    init();
}

Node::Node(const QJsonObject &obj, quint8 inputs, quint8 outputs, QGraphicsItem *parent) :
    QGraphicsItem(parent), inputs_(inputs), outputs_(outputs)
{
    setObjectName(obj.value("uuid").toString());
    setData(1, obj.value("class").toString());
    setPos(QPointF(obj.value("x").toInt(), obj.value("y").toInt()));
    params_ = obj.value("parameters").toObject();

    init();
}

void Node::init()
{
    setData(0, "Node");

    borderSize_ = 3;
    backgroundColor_ = QColor(195, 222, 243);
    borderColor_ = QColor(152, 152, 152);
    textColor_ = QColor(0, 0, 0);
    terminalSize_ = 8;
    terminalBorderSize_ = 2;
    font_ = QFont("Arial", 12);

    selInput_ = -1;
    selOutput_ = -1;

    w_ = 120;
    h_ = 60;

    initInputsOutputs();

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);

    update();
}

void Node::initInputsOutputs()
{
    if (inputTerminals_.size())
        inputTerminals_.clear();
    for (quint8 i = 0; i < inputs_; i++) {
        inputTerminals_.push_back(NodeTerminal(this, i, true));
    }

    if (outputTerminals_.size())
        outputTerminals_.clear();
    for (quint8 i = 0; i < outputs_; i++) {
        outputTerminals_.push_back(NodeTerminal(this, i, false));
    }
}

Node::~Node()
{

}

QJsonObject Node::toJsonObj()
{
    QJsonObject obj;
    obj.insert("uuid", objectName());
    obj.insert("class", nodeClass());
    obj.insert("x", pos().x());
    obj.insert("y", pos().y());
    obj.insert("parameters", params_);
    return obj;
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    button_ = event->button();
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    if (button_ != Qt::LeftButton) {
        return;
    }

    qint16 selInput = selInput_;
    qint16 selOutput = selOutput_;

    if (selectTerminal(inputTerminals_, selInput_, event)) {
        if (selOutput_ > -1) {
            outputTerminals_[selOutput_].setSelected(false);
            emit outputDeselected(selOutput_);
            selOutput_ = -1;
        }
        if (selInput_ > -1) {
            if (selInput > -1)
                emit inputDeselected(selInput);
            emit inputSelected(selInput_);
        } else {
            emit inputDeselected(selInput);
        }
    } else if (selectTerminal(outputTerminals_, selOutput_, event)) {
        if (selInput_ > -1) {
            inputTerminals_[selInput_].setSelected(false);
            emit inputDeselected(selInput_);
            selInput_ = -1;
        }
        if (selOutput_ > -1) {
            if (selOutput > -1)
                emit inputDeselected(selOutput);
            emit outputSelected(selOutput_);
        } else {
            emit outputDeselected(selOutput);
        }
    }

    update();
}

QJsonObject *Node::params()
{
    return &params_;
}

void Node::setParams(const QJsonObject &params)
{
    params_ = params;
}

QFont Node::font() const
{
    return font_;
}

void Node::setFont(const QFont &font)
{
    font_ = font;
    update();
}

QColor Node::textColor() const
{
    return textColor_;
}

void Node::setTextColor(const QColor &textColor)
{
    textColor_ = textColor;
    update();
}

QString Node::text() const
{
    return text_;
}

void Node::setText(const QString &text)
{
    text_ = text;
    update();
}

QPixmap Node::icon() const
{
    return icon_;
}

void Node::setIcon(const QPixmap &icon)
{
    icon_ = icon;
    update();
}

QString Node::nodeClass() const
{
    return data(1).toString();
}

bool Node::selectTerminal(QVector<NodeTerminal> &terminals, qint16 &selected, QGraphicsSceneMouseEvent *event)
{
    for (quint8 i = 0; i < terminals.size(); ++i) {
        if (terminals[i].contains(event->buttonDownPos(Qt::LeftButton))) {
            if (selected != i) {
                if (selected > -1) {
                    terminals[selected].setSelected(false);
                }
                selected = i;
                terminals[i].setSelected(true);
            } else {
                selected = -1;
                terminals[i].setSelected(false);
            }
            update();
            return true;
        }
    }
    return false;
}

quint16 Node::h() const
{
    return h_;
}

void Node::deselectTerminals()
{
    if (selInput_ > -1) {
        inputTerminals_[selInput_].setSelected(false);
        selInput_ = - 1;
    }
    if (selOutput_ > -1) {
        outputTerminals_[selOutput_].setSelected(false);
        selOutput_ = -1;
    }
    update();
}

NodeTerminal &Node::inputTerminal(const quint8 &i)
{
    return inputTerminals_[i];
}

NodeTerminal &Node::outputTerminal(const quint8 &i)
{
    return outputTerminals_[i];
}

quint16 Node::w() const
{
    return w_;
}

quint8 Node::terminalBorderSize() const
{
    return terminalBorderSize_;
}

void Node::setSize(quint16 w, quint16 h)
{
    if (w == w_ && h == h_) {
        return;
    }

    w_ = w;
    h_ = h;

    initInputsOutputs();
    update();
}

quint8 Node::terminalSize() const
{
    return terminalSize_;
}

quint8 Node::outputs() const
{
    return outputs_;
}

quint8 Node::inputs() const
{
    return inputs_;
}

QColor Node::backgroundColor() const
{
    return backgroundColor_;
}

void Node::setBackgroundColor(const QColor &backgroundColor)
{
    backgroundColor_ = backgroundColor;
    update();
}

QColor Node::borderColor() const
{
    return borderColor_;
}

void Node::setBorderColor(const QColor &borderColor)
{
    borderColor_ = borderColor;
    update();
}

quint8 Node::borderSize() const
{
    return borderSize_;
}

void Node::setBorderSize(const quint8 &borderSize)
{
    borderSize_ = borderSize;
    update();
}

QRectF Node::boundingRect() const
{
    return QRectF(0, 0, w_ + borderSize_ * 2.1, h_ + borderSize_ * 2.1);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    QRectF rect = QRectF(borderSize_ / 2 + terminalSize_ / 2 - terminalBorderSize_ / 2, borderSize_ / 2, w_, h_);
    if (borderSize_) {
        QPainterPath path;
        path.addRoundedRect(rect,
                            borderSize_ * 2,
                            borderSize_ * 2);

        QPen pen(borderColor_, borderSize_);
        if (option->state & QStyle::State_Selected)
            pen.setColor(QColor(127, 0, 0));

        painter->setPen(pen);
        painter->fillPath(path, backgroundColor_);
        painter->drawPath(path);
    } else {
        painter->fillRect(rect, backgroundColor_);
    }

    painter->setPen(textColor_);
    painter->setFont(font_);
    painter->drawText(rect, Qt::AlignCenter, nodeClass());

    for (quint8 i = 0; i < inputs_; i++) {
        inputTerminals_[i].draw(painter);
    }
    for (quint8 i = 0; i < outputs_; i++) {
        outputTerminals_[i].draw(painter);
    }
}
