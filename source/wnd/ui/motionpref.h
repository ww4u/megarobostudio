#ifndef MOTIONPREF_H
#define MOTIONPREF_H

#include <QDialog>

namespace Ui {
class MotionPref;
}

class MotionModel
{
public:
    double mRotWrist, mRotSmallArm, mRotBigArm, mRotBase;       //! angle
    double mInitWrist, mInitSmallArm, mInitBigArm, mInitBase;   //! angle
};

class MotionPref : public QDialog
{
    Q_OBJECT

public:
    explicit MotionPref(QWidget *parent = 0);
    ~MotionPref();

public:
    void setModel( MotionModel *pModel );
    MotionModel *model();

    void updateData();
    void updateUi();

private slots:
    void on_MotionPref_accepted();

private:
    Ui::MotionPref *ui;

    MotionModel *m_pModel;
};

#endif // MOTIONPREF_H
