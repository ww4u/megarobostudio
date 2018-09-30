#ifndef ROBOAXES_H
#define ROBOAXES_H

#include <QDateTime>
#include <QDialog>
#include <QLabel>
#include <QLCDNumber>
#include "../../device/vrobot.h"
#include "dlgview.h"
#include "megalcdnumber.h"

namespace Ui {
class roboAxes;
}

class RoboJoint;
class roboAxes : public DlgView
{
    Q_OBJECT

public:
    explicit roboAxes( mcModel *pModel,
                       const QString &roboName,
                       QWidget *parent = 0);
    ~roboAxes();

Q_SIGNALS:
    void sig_close();

protected Q_SLOTS:
    void on_chkAngle_toggled(bool checked);
    void on_sampleTimer_valueChanged( int val );

    void slot_timeout();
    void slot_joint_action( int id, float dt, float angle );
    void slot_joint_zero( int id, bool bCcw );

    void slot_robo_changed( const QString &roboName );
    void slot_device_changed();

protected:
    VRobot *Robot();
    void adapteUiToRobot( VRobot *pRobo );

protected:
    void buildConnection();

    void rotate( int jointId,
                 float t1, float a1,
                 float t2, float a2 );

    void zero( int jointId,
               bool bCcw );

private slots:


    void on_btnZero_clicked();

    void on_spinStepTime_valueChanged(double arg1);

    void slot_comboBox_currentIndexChanged(int index);

    void on_toolButton_clicked();

private:
    Ui::roboAxes *ui;
    QTimer mTimer;

    QList< RoboJoint* > mJoints;
    QList< MegaLcdnumber *> mPoseLcds;
};

#endif // ROBOAXES_H
