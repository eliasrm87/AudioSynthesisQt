#include "partcontrols.h"

PartControls::PartControls(QWidget *parent) :
    QWidget(parent)
{
    lay_ = new QGridLayout(this);
    this->setLayout(lay_);
}
