#include "source.h"
#include <QDebug>

Source::Source(QString fileName, QObject *parent) : QObject(parent)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    samples_ = file.readAll();
    file.close();
    processedSamples_ = &samples_;

    pos_ = 0;
    size_ = processedSamples_->size() / 4; //2 channels of 2 bytes each (16 bits)
}

Source::Source(const Source &source) : QObject(source.parent())
{
    samples_ = *source.processedSamples_;
    processedSamples_ = &samples_;

    pos_ = 0;
    size_ = processedSamples_->size() / 4; //2 channels of 2 bytes each (16 bits)
}

Source::~Source()
{

}

const qint64& Source::size()
{
    return size_;
}

void Source::getSamples(QByteArray &dest, qint32 size)
{
    size *= 2; //2 bytes each (16 bits)

    if (size > processedSamples_->size())
        qDebug() << "ERROR";

    qint64 remaining = processedSamples_->size() - pos_;
    if (size > remaining) {
        dest = processedSamples_->mid(pos_);
        dest.append(processedSamples_->mid(0, size - remaining));
    } else {
        dest = processedSamples_->mid(pos_, size);
    }
}

void Source::incPosition(qint32 size)
{
    pos_ = (pos_+size)%processedSamples_->size();
}
