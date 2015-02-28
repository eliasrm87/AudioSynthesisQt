#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QGridLayout>
#include <QDebug>
#include <QSpinBox>
#include <QLabel>
#include <QDockWidget>
#include <QtMath>

#include <DataFlow/nodelinkline.h>
#include <DataFlow/dataflowview.h>
#include <AudioNodes/audionodes.h>
#include <Audio/synthetizer.h>
#include <AudioControls/audiocontrols.h>
#include "synthetizerflowview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onActFileOpen(bool checked);
    void onActFileSave(bool checked);
    void onActAddNode(bool checked);

    void onTempoChange();
    void onSignChange();

    void onNodeAdded(Node* node);
    void onNodeRemoved(QString nodeId);
    void onLinkAdded(NodeLinkLine* nodeLink);
    void onLinkRemoved(QString nodeId);
    void onNodeSelected(Node* node);

private:
    QGridLayout* lytMain_;
    QMenu* mnuFile_;
    QAction* actFileOpen_;
    QAction* actFileSave_;
    QMenu* mnuNode_;
    SynthetizerFlowView* dfv_;
    QLabel* lblTempo_;
    QSpinBox* spbTempo_;
    QLabel* lblSignNum_;
    QSpinBox* spbSignNum_;
    QLabel* lblSignDen_;
    QSpinBox* spbSignDen_;
    QDockWidget* selNodeControls_;

    Synthetizer* synthetizer_;
    QHash<QString, Node*> nodes_;
    QHash<QString, NodeLinkLine*> nodeLinks_;
    QHash<QString, PartControls*> partsControls_;

    void addNodeToMenu(QString nodeClass);














    qint16 getSample16(QByteArray &samples, qint64 i);
    QByteArray &getSamples16(QByteArray &samples, qint64 i, qint64 n);
    void sine16(QByteArray &array, quint64 samples, qreal frequency, qreal amplitude = 1, qreal sampleRate = 44100);
    QByteArray mix16(QByteArray ch1, QByteArray ch2);
    void delay16(QByteArray &result, QByteArray &samples, qreal delay, qreal sampleRate = 44100);
    void rever16(QByteArray &result, QByteArray &samples, qreal delay, qreal decay, qreal sampleRate = 44100);
    void fadeIn16(QByteArray &result, QByteArray &samples, qreal delay, qreal sampleRate = 44100);
    void fadeOut16(QByteArray &result, QByteArray &samples, qreal delay, qreal sampleRate = 44100);

    QByteArray organ16_6(quint64 samples, qreal frequency);
};

#endif // MAINWINDOW_H
