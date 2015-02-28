#include <QDebug>
#include <QJsonArray>
#include "loop.h"


Loop::Loop(QJsonObject *params, QObject *parent) :
    Source("Loop", params, parent)
{
    //Path
    path_ = params_->value("path").toString("");
    //Parts
    QJsonArray parts = params_->value("parts").toArray();
    foreach (QJsonValue part, parts) {
        parts_.push_back(part.toString().toULongLong());
    }
    if (parts_.isEmpty()) {
        addPart(0);
    } else {
        qSort(parts_);
    }
}

Loop::Loop(const Loop &loop) :
    Source(loop)
{
    path_ = loop.path_;
    parts_ = loop.parts_;
}

Loop::~Loop()
{

}

const SamplesVector &Loop::getSamples()
{
    return Source::getSamples();
}

void Loop::getSamples(SamplesVector &dest, const qint32 &size)
{
    QMutexLocker mtxLock(&mtx_);

    if (!synthesized_) {
        synthesized_ = true;
        synthesize();
    }

    dest.clear();

    int currentBeat, nextBeat;
    quint32 beatLength, beatPos;
    {
        QMutexLocker staticVarsMtxLoc(staticVarsMtx_);
        currentBeat = (samplesClock_ / beatLength_) % parts_.size();
        nextBeat = (currentBeat + 1) % parts_.size();
        beatPos = samplesClock_ % beatLength_;
        pos_ = parts_[currentBeat] + beatPos;
        beatLength = beatLength_;
    }

    qint32 remSize = size;
    qint32 mSize, remBeat;
    qint32 available;

    while (remSize > 0) {
        remBeat = beatLength - beatPos;

        if (remSize > remBeat) {
            mSize = remBeat;
        } else {
            mSize = remSize;
        }

        if (nextBeat == 0) {
            available = samplesBuffer_.size() - pos_;
        } else {
            available = parts_[nextBeat] - pos_;
        }

        if (available < 0) {
            available = 0;
        } else if (available > remBeat) {
            available = remBeat;
        }

        if (mSize > available) {
            dest.append(samplesBuffer_.mid(pos_, available));
            SamplesVector filling;
            filling.fill(0, mSize - available);
            dest.append(filling);
        } else {
            dest.append(samplesBuffer_.mid(pos_, mSize));
        }

        pos_ += mSize;
        remSize -= mSize;
        nextBeat = (currentBeat + 1) % parts_.size();
    }
}

QString Loop::path() const
{
    return path_;
}

void Loop::setPath(const QString &path)
{
    QMutexLocker mtxLock(&mtx_);
    path_ = path;
    params_->insert("path", path);
    synthesize();
}

void Loop::addPart(const quint64 &part)
{
    QMutexLocker mtxLock(&mtx_);
    if (parts_.contains(part))
        return;

    parts_.push_back(part);

    QJsonArray jsonArr;
    foreach (quint64 p, parts_) {
        jsonArr.push_back(QString::number(p));
    }
    params_->insert("parts", jsonArr);
    qSort(parts_);
}

void Loop::delPart(const quint64 &part)
{
    QMutexLocker mtxLock(&mtx_);
    parts_.removeOne(part);
}

void Loop::synthesize()
{
    qDebug() << "Loop::synthesize" << path_;

    QFile file(path_);
    file.open(QFile::ReadOnly);
    QByteArray buffer = file.readAll();
    file.close();

    qint16 value16;
    samplesBuffer_.clear();
    for (int i = 0; i < buffer.size(); i += 2) {
        value16 = (qint16)(((buffer[i+1] & 0xFF) << 8) | (buffer[i] & 0xFF));
        samplesBuffer_.push_back((float)value16 / (float)INT16_MAX);
    }

    pos_ = 0;

    Source::synthesize();
}
