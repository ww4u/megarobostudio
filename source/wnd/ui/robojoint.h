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
    void setAngle( float angle );

    void setAngleVisible( bool b );
    void setCcwVisible( bool b );
    void setCcwChecked( bool b );

    void setStepTime( float );
    float stepTime();
protected:
    void actionChanged( const QDateTime &time, int valEnd  );
    void rotate( float ang, float ts );
Q_SIGNALS:
    void signal_actionChanged( int id, float time, float val );
    void signal_zeroClicked( int id, bool bCcw );

private slots:
    void on_doubleSpinBox_valueChanged(double arg1);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_sliderPressed();

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_sliderMoved(int position);

    void on_pushButton_clicked();

    void slot_step( float stp );

private:
    Ui::RoboJoint *ui;

    QDateTime mPressTime;
    float mStepTime;
    int mPressValue, mCurValue;
    int mId;
};

#endif // ROBOJOINT_H
