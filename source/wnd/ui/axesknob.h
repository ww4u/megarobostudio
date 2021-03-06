#ifndef AXESKNOB_H
#define AXESKNOB_H

#include <QDialog>
#include <QTime>
#include "../../../include/mcstd.h"

//#include "../../model/mcmodelobj.h"
#include "../../model/tpvgroup.h"
#include "../../device/vdevice.h"
#include "dlgview.h"

#include "roboview.h"

namespace Ui {
class axesKnob;
}

class axesKnob : public RoboView
{
    Q_OBJECT

public:
    explicit axesKnob( mcModel *pMcModel,
                       int subAx,
                       const QString &connectStr,
                       QWidget *parent = 0);
    ~axesKnob();

protected:
    MegaDevice::deviceMRQ * currentDevice( int &ax );

    void actionChanged( const QDateTime &time, int valEnd  );

    void rotate( float angle, float t, bool bKeep = false );
    void stop();

private slots:
    void slot_device_changed();

    void on_sliderValue_valueChanged(int value);

    void on_spinNow_valueChanged(double arg1);

    void on_sliderValue_sliderPressed();

    void on_sliderValue_sliderReleased();

    void on_sliderValue_sliderMoved(int position);

    void slot_step( float step );
    void slot_step( float step, bool bKeep );

private:
    Ui::axesKnob *ui;
    QDateTime mStartTime, mStopTime;
    float mStartAngle, mStopAngle;
};

#endif // AXESKNOB_H
