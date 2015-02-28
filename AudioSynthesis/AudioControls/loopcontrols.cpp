#include <QFileDialog>

#include "loopcontrols.h"

LoopControls::LoopControls(QWidget *parent) :
    PartControls(parent)
{
    wave_ = new Waveform(this);
    txtLoopPath_ = new QLineEdit(this);
    btnExplore_ = new QPushButton("...", this);

    lay_->addWidget(txtLoopPath_, 0, 0,  1, 15);
    lay_->addWidget(btnExplore_,  0, 15, 1, 1);
    lay_->addWidget(wave_,        1, 0,  1, 16);

    connect(btnExplore_,  SIGNAL(pressed()),            this, SLOT(onExplore()));
    connect(txtLoopPath_, SIGNAL(textChanged(QString)), this, SIGNAL(pathChanged(QString)));
}

void LoopControls::setWaveForm(const QVector<float> &samples)
{
    wave_->setWaveForm(samples);
}

void LoopControls::setPath(QString path)
{
    txtLoopPath_->setText(path);
}

void LoopControls::onExplore()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open loop"), "", tr("Loop files (*.raw *.loop)"));

    if (!filename.isEmpty()) {
        txtLoopPath_->setText(filename);
    }
}
