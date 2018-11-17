#ifndef MRQSENSOR_H
#define MRQSENSOR_H

#include <QWidget>
#include "mrqview.h"

namespace Ui {
class mrqSensor;
}

class mrqSensor : public mrqView
{
    Q_OBJECT

public:
    explicit mrqSensor(const QStringList &sensorList, QWidget *parent = 0);
    ~mrqSensor();

public:
    virtual int setApply();
public:
    virtual void modelChanged();

protected:
    void spyEdited();
    void setupUi();
    void desetupUi();

    int apply();
    int updateUi();

    void adaptUi();

private slots:


private:
    Ui::mrqSensor *ui;
};

#endif // MRQSENSOR_H
