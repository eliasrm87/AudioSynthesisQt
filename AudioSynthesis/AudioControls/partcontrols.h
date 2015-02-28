#ifndef PARTCONTROLS_H
#define PARTCONTROLS_H

#include <QWidget>
#include <QGridLayout>

class PartControls : public QWidget
{
    Q_OBJECT
public:
    explicit PartControls(QWidget *parent = 0);

signals:

public slots:

protected:
    QGridLayout* lay_;
};

#endif // PARTCONTROLS_H
