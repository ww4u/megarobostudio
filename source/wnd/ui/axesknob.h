#ifndef AXESKNOB_H
#define AXESKNOB_H

#include <QDialog>
#include <QTime>
#include "../../../include/mcstd.h"

//#include "../../model/mcmodelobj.h"
#include "../../model/tpvgroup.h"
#include "../../device/vdevice.h"
#include "dlgview.h"

namespace Ui {
class axesKnob;
}

class axesKnob : public DlgView
{
    Q_OBJECT

public:
    explicit axesKnob( mcModel *pMcModel,
                       const QString &connectStr,
                       QWidget *parent = 0);
    ~axesKnob();

protected:
    MegaDevice::deviceMRQ * currentDevice( int &ax );

//public:
//    void setDevice( MegaDevice::VDevice *pDevice,
//                    int axesId );
//    void setConnection( const QString &conn );

private slots:
    void slot_device_changed();

    void on_sliderValue_valueChanged(int value);

    void on_spinNow_valueChanged(double arg1);

    void on_sliderValue_sliderPressed();

    void on_sliderValue_sliderReleased();


private:
    Ui::axesKnob *ui;
    QDateTime mStartTime, mStopTime;
    float mStartAngle, mStopAngle;
};

#endif // AXESKNOB_H
