#ifndef ROBOAXES_H
#define ROBOAXES_H

#include <QDateTime>
#include <QDialog>

#include "../../device/vrobot.h"

namespace Ui {
class roboAxes;
}

class roboAxes : public QDialog
{
    Q_OBJECT

public:
    explicit roboAxes(QWidget *parent = 0);
    ~roboAxes();

Q_SIGNALS:
    void sig_close();

protected Q_SLOTS:
    void on_close();

    void on_sldBase_valueChanged(int v );
    void on_sldBigArm_valueChanged(int v );
    void on_sldLitArm_valueChanged(int v );
    void on_sldWrist_valueChanged(int v);
    void on_sldHand_valueChanged(int v);

    void on_sBoxBase_valueChanged( double v );
    void on_sBoxBigArm_valueChanged( double v );
    void on_sBoxLitArm_valueChanged( double v );
    void on_sBoxWrist_valueChanged( double v );
    void on_sBoxHand_valueChanged( double v );

    void on_sldBase_sliderPressed();
    void on_sldBase_sliderReleased();

    void on_sldBigArm_sliderPressed();
    void on_sldBigArm_sliderReleased();

    void on_sldHand_sliderPressed();
    void on_sldHand_sliderReleased();

    void on_sldLitArm_sliderPressed();
    void on_sldLitArm_sliderReleased();

    void on_sldWrist_sliderPressed();
    void on_sldWrist_sliderReleased();

public:
    void attachRobot( VRobot *pRobo );
    VRobot *Robot();

protected:
    void buildConnection();

    void rotate( int jointId,
                 float t1, float a1,
                 float t2, float a2 );

private:
    Ui::roboAxes *ui;

    VRobot *m_pRobo;

    QDateTime mFromTime, mEndTime;
    float mAngleFrom, mAngleTo;
};

#endif // ROBOAXES_H
