#ifndef MEGATRONPREF_H
#define MEGATRONPREF_H

#include <QtWidgets>
#include "modelview.h"
namespace Ui {
class megatronpref;
}

class MegatronPref : public modelView
{
    Q_OBJECT

public:
    explicit MegatronPref(QWidget *parent = 0);
    ~MegatronPref();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

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
    void on_btnZeroFx_clicked();

    void on_btnZeroFz_clicked();

    void on_btnZeroBx_clicked();

    void on_btnZeroBz_clicked();

    void on_btnZeroLy_clicked();

    void on_btnZeroRy_clicked();

    void on_btnZeroBody_clicked();


    void on_chkAllCcw_clicked(bool checked);

    void on_chkAllBody_clicked(bool checked);

private:
    Ui::megatronpref *ui;

    QList<QLineEdit*> mEdits;
    QList<QLabel*> mLabels;

    QList<QCheckBox*> mBodyChecks;
    QList<QCheckBox*> mCcwChecks;
};

#endif // MEGATRONPREF_H
