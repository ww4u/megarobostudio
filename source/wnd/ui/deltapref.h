#ifndef DELTAPREF_H
#define DELTAPREF_H

#include <QtWidgets>

#include "modelview.h"

namespace Ui {
class deltapref;
}

class DeltaPref : public modelView
{
    Q_OBJECT

public:
    explicit DeltaPref(QWidget *parent = 0);
    ~DeltaPref();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

protected:
    void updateData();
    void updateUi();

    void zeroJoint( int jointId, bool bCcw );

protected Q_SLOTS:
    void slot_joint_zero( int jId, bool bccw );
    void slot_body_changed();

Q_SIGNALS:
    void signal_joint_zero( int jointid, bool bccw );

private slots:
    void on_btnZeroLS_clicked();

    void on_btnZeroRS_clicked();

    void on_btnZeroHand_clicked();

    void on_btnZeroPlate_clicked();

    void on_btnZeroBody_clicked();
private:
    Ui::deltapref *ui;

    QList<QCheckBox*> mBodyChecks;
    QList<QCheckBox*> mCcwChecks;
};

#endif // DELTAPREF_H
