#ifndef MOTORPREF_H
#define MOTORPREF_H

#include <QtWidgets>
#include "modelview.h"
namespace Ui {
class MotorPref;
}

class MotorPref : public modelView
{
    Q_OBJECT

public:
    explicit MotorPref(QWidget *parent = 0);
    ~MotorPref();

protected:
    virtual void changeEvent( QEvent *event );

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

    virtual void updateScreen();
protected:
    void spyEdited();
    void updateData();
    void updateUi();

    void adaptUi();

    void zeroJoint( int jointId, bool bCcw );

protected Q_SLOTS:
    void slot_joint_zero( int jId, bool bCcw );

    void on_btnZeroBody_clicked();

    void on_btnZeroMotor_clicked();

Q_SIGNALS:
    void signal_joint_zero( int jointid, bool bCcw );

private:
    Ui::MotorPref *ui;
};

#endif // MOTORPREF_H
