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
    void setConnection( const QString &conn );

private slots:
    void on_sliderValue_valueChanged(int value);

    void on_spinNow_valueChanged(double arg1);

    void on_sliderValue_sliderPressed();

    void on_sliderValue_sliderReleased();

private:
    Ui::axesKnob *ui;
    QDateTime mStartTime, mStopTime;
    float mStartAngle, mStopAngle;

    MegaDevice::VDevice *m_pDevice;
    int mAxesId;
};

#endif // AXESKNOB_H
