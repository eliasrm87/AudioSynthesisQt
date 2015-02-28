#include "mainwindow.h"

#define MAX_16 65535
#define MAX_16_2 32767

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //GUI
    btnPlay_ = new QPushButton(tr("Play"), this);
    this->setCentralWidget(btnPlay_);
    connect(btnPlay_, SIGNAL(pressed()), this, SLOT(onPlay()));

    //AUDIO
    format_.setSampleRate(44100);
    format_.setChannelCount(2);
    format_.setSampleSize(16);
    format_.setCodec("audio/pcm");
    format_.setByteOrder(QAudioFormat::LittleEndian);
    format_.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format_)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    buffer_ = new AudioSamplesBuffer(44100, this);
    buffer_->open(QIODevice::ReadWrite);

    player_ = new PlayerWorker(format_, buffer_);
    playerThread_ = new QThread(this);
    playerThread_->start();
    while (!playerThread_->isRunning())
        QThread::msleep(1);
    player_->moveToThread(playerThread_);

    QMetaObject::invokeMethod(player_, "play");

    mixer_ = new MixerWorker(buffer_);
    mixerThread_ = new QThread(this);
    mixerThread_->start();
    while (!mixerThread_->isRunning())
        QThread::msleep(1);
    mixer_->moveToThread(mixerThread_);

    QMetaObject::invokeMethod(mixer_, "start");
}

MainWindow::~MainWindow()
{
    mixerThread_->quit();
    while (mixerThread_->isRunning())
        QThread::msleep(1);
    delete mixer_;

    playerThread_->quit();
    while (playerThread_->isRunning())
        QThread::msleep(1);
    delete player_;
}

void MainWindow::onPlay()
{
    Source* source1 = new Source("/home/elias/loop.raw", this);
    Source* source2 = new Source("/home/elias/loop2.raw", this);

    mixer_->addSource(source1);
    mixer_->addSource(source2);

    qDebug() << "Done";
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {

    }
    else {
        QWidget::changeEvent(event);
    }
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

