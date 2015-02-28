#include "mainwindow.h"

#define MAX_16 65535
#define MAX_16_2 32767

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(1024, 768);

    setCentralWidget(new QWidget(this));
    lytMain_ = new QGridLayout(centralWidget());
    centralWidget()->setLayout(lytMain_);

    dfv_ = new SynthetizerFlowView(this);
    dfv_->setContextMenuPolicy(Qt::ActionsContextMenu);

    selNodeControls_ = new QDockWidget(tr("Node controls"), this);
    this->addDockWidget(Qt::BottomDockWidgetArea, selNodeControls_);

    lblTempo_ = new QLabel(tr("Tempo:"));
    spbTempo_ = new QSpinBox(this);
    spbTempo_->setMaximum(512);
    spbTempo_->setValue(Source::getTempo());
    lblSignNum_ = new QLabel(tr("Sign:"));
    spbSignNum_ = new QSpinBox(this);
    spbSignNum_->setMinimum(1);
    spbSignNum_->setValue(Source::getSign().first);
    lblSignDen_ = new QLabel(tr("/"));
    spbSignDen_ = new QSpinBox(this);
    spbSignDen_->setMinimum(1);
    spbSignDen_->setValue(Source::getSign().second);

    dfv_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    lytMain_->addWidget(dfv_,        0, 0, 10, 16);
    lytMain_->addWidget(lblTempo_,   15, 0, 1, 1);
    lytMain_->addWidget(spbTempo_,   15, 1, 1, 1);
    lytMain_->addWidget(lblSignNum_, 15, 2, 1, 1);
    lytMain_->addWidget(spbSignNum_, 15, 3, 1, 1);
    lytMain_->addWidget(lblSignDen_, 15, 4, 1, 1);
    lytMain_->addWidget(spbSignDen_, 15, 5, 1, 1);

    mnuFile_ = new QMenu("File", this);
    menuBar()->addMenu(mnuFile_);
    actFileOpen_ = new QAction("Open", this);
    mnuFile_->addAction(actFileOpen_);
    actFileSave_ = new QAction("Save", this);
    mnuFile_->addAction(actFileSave_);

    mnuNode_ = new QMenu("Node", this);
    menuBar()->addMenu(mnuNode_);

    addNodeToMenu("Loop");
    addNodeToMenu("Oscillator");
    addNodeToMenu("Output");

    connect(actFileOpen_, SIGNAL(triggered(bool)), this, SLOT(onActFileOpen(bool)));
    connect(actFileSave_, SIGNAL(triggered(bool)), this, SLOT(onActFileSave(bool)));

    synthetizer_ = new Synthetizer(this);
    synthetizer_->start();

    connect(dfv_,        SIGNAL(nodeAdded(Node*)),         this, SLOT(onNodeAdded(Node*)));
    connect(dfv_,        SIGNAL(nodeRemoved(QString)),     this, SLOT(onNodeRemoved(QString)));
    connect(dfv_,        SIGNAL(linkAdded(NodeLinkLine*)), this, SLOT(onLinkAdded(NodeLinkLine*)));
    connect(dfv_,        SIGNAL(linkRemoved(QString)),     this, SLOT(onLinkRemoved(QString)));
    connect(dfv_,        SIGNAL(nodeSelected(Node*)),      this, SLOT(onNodeSelected(Node*)));
    connect(spbTempo_,   SIGNAL(valueChanged(int)),        this, SLOT(onTempoChange()));
    connect(spbSignNum_, SIGNAL(valueChanged(int)),        this, SLOT(onSignChange()));
    connect(spbSignDen_, SIGNAL(valueChanged(int)),        this, SLOT(onSignChange()));
}

MainWindow::~MainWindow()
{
    synthetizer_->quit();
}

void MainWindow::onActFileOpen(bool checked)
{
    Q_UNUSED(checked);

    QString filename = QFileDialog::getOpenFileName(this, tr("Open flow"), "", tr("Flow files (*.flow)"));

    if (!filename.isEmpty()) {
        QFile file(filename);
        file.open(QFile::ReadOnly);
        dfv_->fromJson(QJsonDocument::fromJson(file.readAll()));
        file.close();
    }
}

void MainWindow::onActFileSave(bool checked)
{
    Q_UNUSED(checked);

    QString filename = QFileDialog::getSaveFileName(this, tr("Save flow"), "", tr("Flow files (*.flow)"));

    if (!filename.isEmpty()) {
        if (!filename.endsWith(".flow")) {
            filename += ".flow";
        }
        QFile file(filename);
        file.open(QFile::Truncate | QFile::WriteOnly);
        file.write(dfv_->toJson().toJson());
        file.close();
    }
}

void MainWindow::onActAddNode(bool checked)
{
    Q_UNUSED(checked);

    QAction* sender = (QAction*)QObject::sender();

    QPoint pos = dfv_->mousePosition();

    QJsonObject nodeJsonObj;
    nodeJsonObj.insert("class", sender->data().toString());
    nodeJsonObj.insert("x", pos.x());
    nodeJsonObj.insert("y", pos.y());

    dfv_->addNode(dfv_->newNodeFromJson(nodeJsonObj));
}

void MainWindow::onTempoChange()
{
    Source::setTempo(spbTempo_->value());
}

void MainWindow::onSignChange()
{
    Source::setSign(QPair<quint8, quint8>(spbSignNum_->value(), spbSignDen_->value()));
}

void MainWindow::onNodeAdded(Node *node)
{
    nodes_.insert(node->objectName(), node);

    SynPiece* piece = synthetizer_->addPiece(node->nodeClass(), node->params(), node->objectName());
    if (piece == Q_NULLPTR)
        return;

    PartControls* controls = synthetizer_->getControls(piece->objectName());
    if (controls)
        partsControls_.insert(node->objectName(), controls);
}

void MainWindow::onNodeRemoved(QString nodeId)
{
    if (!nodes_.contains(nodeId))
        return;

    synthetizer_->removePiece(nodeId);

    nodes_.remove(nodeId);
}

void MainWindow::onLinkAdded(NodeLinkLine *nodeLink)
{
    synthetizer_->connectPieces(nodeLink->fromNode()->objectName(),
                                nodeLink->toNode()->objectName());
    nodeLinks_.insert(nodeLink->objectName(), nodeLink);
}

void MainWindow::onLinkRemoved(QString nodeId)
{
    if (!nodeLinks_.contains(nodeId))
        return;

    NodeLinkLine *nodeLink = nodeLinks_[nodeId];
    synthetizer_->disconnectPieces(nodeLink->fromNode()->objectName(),
                                nodeLink->toNode()->objectName());
    nodeLinks_.remove(nodeId);
}

void MainWindow::onNodeSelected(Node *node)
{
    if (!partsControls_.contains(node->objectName()))
        return;

    selNodeControls_->setWidget(partsControls_[node->objectName()]);
}

void MainWindow::addNodeToMenu(QString nodeClass)
{
    QAction* act = new QAction(nodeClass, this);
    act->setData(nodeClass);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(onActAddNode(bool)));

    mnuNode_->addAction(act);
    dfv_->addAction(act);
}

qint16 MainWindow::getSample16(QByteArray &samples, qint64 i)
{
    i *= 2;

    if ((i+1) > samples.size())
        return 0;

    return *((qint16*)(samples.data() + i));
}

void MainWindow::sine16(QByteArray &array, quint64 samples, qreal frequency, qreal amplitude, qreal sampleRate)
{
    qreal t = 0;
    qreal T = (qreal)1.0f / sampleRate;

    quint64 i = 0;
    while (i < samples) {
        qint16 sample = (qreal)MAX_16_2 * qSin((qreal)2 * (qreal)M_PI * frequency * t) * amplitude;
        t += T;
        array.push_back(sample & 0x00FF);
        array.push_back((sample & 0xFF00) >> 8);
        i++;
    }
}

QByteArray MainWindow::mix16(QByteArray ch1, QByteArray ch2)
{
    QByteArray result;
    result.clear();

    int size = ch1.size();
    if (ch2.size() < size)
        size = ch2.size();

    size /= 2;

    int i = 0;
    while (i < size) {
        qint32 sample = getSample16(ch1, i);
        sample += getSample16(ch2, i);

        sample /= 2;

        result.push_back(sample & 0x00FF);
        result.push_back((sample & 0xFF00) >> 8);

        ++i;
    }

    return result;
}

void MainWindow::delay16(QByteArray &result, QByteArray &samples, qreal delay, qreal sampleRate)
{
    result.clear();
    delay = (qint64)(delay*sampleRate)*2;

    if (delay > 0) {
        result.fill(0, delay);
        result.append(samples);
    } else {
        result.append(samples);

        QByteArray fill;
        fill.fill(0, delay*-1);
        result.append(fill);
    }
}

void MainWindow::rever16(QByteArray &result, QByteArray &samples, qreal delay, qreal decay, qreal sampleRate)
{
    result.clear();

    delay = (qint64)(delay*sampleRate);
    qint32 sample;
    qint64 size = samples.size() / 2;

    qint64 i = 0;
    while (i < (size+delay)) {
        if (i < delay) {
            sample = getSample16(samples, i);

            result.push_back(sample & 0x00FF);
            result.push_back((sample & 0xFF00) >> 8);
        } else if (i < size) {
            sample = getSample16(samples, i);
            sample += (qreal)getSample16(samples, i-delay)*decay;

            sample /= 2;

            result.push_back(sample & 0x00FF);
            result.push_back((sample & 0xFF00) >> 8);
        } else if ((i-delay) < size){
            sample = (qreal)getSample16(samples, i-delay)*decay;

            result.push_back(sample & 0x00FF);
            result.push_back((sample & 0xFF00) >> 8);
        }

        ++i;
    }
}

void MainWindow::fadeIn16(QByteArray &result, QByteArray &samples, qreal delay, qreal sampleRate)
{
    result.clear();

    qreal decay = 0;
    qreal decayStep = 1.f/(delay*sampleRate);
    delay = (qint64)(delay*sampleRate);
    qint32 sample;
    qint64 size = samples.size() / 2;

    qint64 i = 0;
    while (i < size) {
        sample = getSample16(samples, i);

        if (i < delay) {
            sample = (qreal)sample*decay;
            decay += decayStep;
        }

        result.push_back(sample & 0x00FF);
        result.push_back((sample & 0xFF00) >> 8);

        ++i;
    }
}

void MainWindow::fadeOut16(QByteArray &result, QByteArray &samples, qreal delay, qreal sampleRate)
{
    result.clear();

    qreal decay = 1;
    qreal decayStep = 1.f/(delay*sampleRate);
    delay = (qint64)(delay*sampleRate);
    qint32 sample;
    qint64 size = samples.size() / 2;

    qint64 i = 0;
    while (i < size) {
        sample = getSample16(samples, i);

        if (i >= (size-delay)) {
            sample = (qreal)sample*decay;
            decay -= decayStep;
        }

        result.push_back(sample & 0x00FF);
        result.push_back((sample & 0xFF00) >> 8);

        ++i;
    }
}

QByteArray MainWindow::organ16_6(quint64 samples, qreal frequency)
{
    QByteArray ch1;
    QByteArray ch2;
    QByteArray ch3;
    QByteArray ch4;
    QByteArray ch5;
    QByteArray ch6;
    QByteArray result;

    sine16(ch1, samples, frequency/4, 0.8f);
    sine16(ch2, samples, frequency/2, 0.9f);
    sine16(ch3, samples, frequency);
    sine16(ch4, samples, frequency*2, 0.9f);
    sine16(ch5, samples, frequency*4, 0.8f);
    sine16(ch6, samples, frequency*8, 0.7f);

    result = mix16(ch1, ch2);
    result = mix16(result, ch3);
    result = mix16(result, ch4);
    result = mix16(result, ch5);
    result = mix16(result, ch6);

    return result;
}

