#ifndef AXESKNOB_H
#define AXESKNOB_H

#include <QDialog>
#include <QTime>
#include "../../../include/mcstd.h"

//#include "../../model/mcmodelobj.h"
#include "../../model/tpvgroup.h"
#include "../../device/vdevice.h"

namespace Ui {
class axesKnob;
}

class axesKnob : public QDialog
{
    Q_OBJECT

public:
    explicit axesKnob(QWidget *parent = 0);
    ~axesKnob();

public:
    void setDevice( MegaDevice::VDevice *pDevice,
                    int axesId );

protected:
//    void clearTpvRows();

//    double value();
//    double sliderStep();
//    int    sliderValue( double value );

//    void appendAngle( double val );

Q_SIGNALS:
    void sig_start();

private slots:
//    void slot_transtimer_timeout();

//    void on_spinFrom_valueChanged(double arg1);
//    void on_spinTo_valueChanged(double arg1);

    void on_sliderValue_valueChanged(int value);

    void on_spinNow_valueChanged(double arg1);

//    void on_spinRst_valueChanged(double arg1);

    void on_sliderValue_sliderPressed();

    void on_sliderValue_sliderReleased();

private:
    Ui::axesKnob *ui;
    QDateTime mStartTime, mStopTime;
    float mStartAngle, mStopAngle;

    MegaDevice::VDevice *m_pDevice;
    int mAxesId;

//    QList<tpvRow*> mTpvRows;
//    int mCurIndex;
//    QTimer mTransTimer;
};

#endif // AXESKNOB_H
