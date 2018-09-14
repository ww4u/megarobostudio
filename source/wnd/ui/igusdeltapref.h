#ifndef IGUS_DELTA_PREF_H
#define IGUS_DELTA_PREF_H

#include <QtWidgets>

#include "modelview.h"

namespace Ui {
class igusdrylinpref;
}

class IgusDeltaPref : public modelView
{
    Q_OBJECT

public:
    explicit IgusDeltaPref(QWidget *parent = 0);
    ~IgusDeltaPref();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

    virtual void updateScreen();
protected:
    void spyEdited();
    void updateData();
    void updateUi();

    void zeroJoint( int jointId, bool bCcw );

protected Q_SLOTS:
    void slot_joint_zero( int jId, bool bccw );
    void slot_ccw_changed();
    void slot_body_changed();

Q_SIGNALS:
    void signal_joint_zero( int jointid, bool bccw );


private slots:
    void on_btnZeroLS_clicked();

    void on_btnZeroRS_clicked();

    void on_btnZeroHand_clicked();

    void on_btnZeroPlate_clicked();

    void on_btnZeroBody_clicked();


    void on_chkAllCcw_clicked(bool checked);

    void on_chkAllBody_clicked(bool checked);

private:
    Ui::igusdrylinpref *ui;

    QList<QCheckBox*> mBodyChecks;
    QList<QCheckBox*> mCcwChecks;
};

#endif // DELTAPREF_H
