#ifndef ROBOJOINT_H
#define ROBOJOINT_H

#include <QtWidgets>

namespace Ui {
class RoboJoint;
}

class RoboJoint : public QWidget
{
    Q_OBJECT

public:
    explicit RoboJoint(QWidget *parent = 0);
    ~RoboJoint();

    void setId( int id );
    void setTitle( const QString &title );

    void setAngle( float angle, float deltaAngle = 0 );
    void setAngleVisible( bool b, bool bDeltaAngle = false );

    void setCcwVisible( bool b );
    bool isCcwVisible();

    void setCcwChecked( bool b );
    bool isCcwChecked();

    void setStepTime( float );
    float stepTime();

    void setSingle( bool b );
    bool single();

protected:
    void actionChanged( const QDateTime &time, int valEnd  );
    void rotate( float ang, float ts, float ev=0.0f );
    void stop();

Q_SIGNALS:
    void signal_actionChanged( int id, float time, float val, float ev );
    void signal_stop( int id );
    void signal_zeroClicked( int id, bool bCcw );

private slots:
    void on_doubleSpinBox_valueChanged(double arg1);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_sliderPressed();

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_sliderMoved(int position);

    void on_pushButton_clicked();

    void slot_step( float stp );
    void slot_step( float stp, bool b );

private:
    Ui::RoboJoint *ui;

    QDateTime mPressTime;
    float mStepTime;
    bool mbSingle;

    int mPressValue, mCurValue;
    int mId;
};

#endif // ROBOJOINT_H
