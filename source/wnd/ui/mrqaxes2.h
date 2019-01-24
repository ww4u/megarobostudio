#ifndef MRQAXES2_H
#define MRQAXES2_H

#include <QWidget>
#include "mrqview.h"

namespace Ui {
class mrqaxes2;
}

class mrqAxes2 : public mrqView
{
    Q_OBJECT

public:
    explicit mrqAxes2(QWidget *parent = 0);
    ~mrqAxes2();

protected:
    virtual void changeEvent( QEvent *event );

protected Q_SLOTS:


public:
    virtual int setApply();
public:
    virtual void modelChanged();
    virtual void settingChanged(enumSetting setting, const QVariant &v);
protected:
    void spyEdited();
    void setupUi();
    void desetupUi();

    int apply();
    int updateUi();

private slots:
    void on_cmbStopMode_currentIndexChanged(int index);

    void on_cmbTuneMode_currentIndexChanged(int index);

    void on_cmbPlanMode_currentIndexChanged(int index);

    void on_spinSAcc_valueChanged(double arg1);

    void on_spinSDec_valueChanged(double arg1);

    void on_cmbExecMode_currentIndexChanged(int index);

private:
    Ui::mrqaxes2 *ui;
};

#endif // MRQAXES2_H
