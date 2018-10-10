#ifndef PVTPREF_H
#define PVTPREF_H

#include <QtWidgets>

namespace Ui {
class PvtPref;
}

class modelPvtPref
{
public:
    int mVernierStep;
    int mSteps;
    int mEncoderLines;
    int mSlowRatio;

    int mAcc, mDec;

public:
    modelPvtPref();
};

class PvtPref : public QDialog
{
    Q_OBJECT

public:
    explicit PvtPref(QWidget *parent = 0);
    ~PvtPref();

public:
    void setModel( modelPvtPref *pref );
    modelPvtPref *model();

    void updateData();
    void updateUi();

private slots:
    void on_PvtPref_accepted();

    void on_spinAcc_valueChanged(double arg1);

    void on_spinDec_valueChanged(double arg1);

private:
    Ui::PvtPref *ui;
    modelPvtPref *m_pPref;
};

#endif // PVTPREF_H
