#include "robojoint.h"
#include "ui_robojoint.h"
#include "../widget/megamessagebox.h"
RoboJoint::RoboJoint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoboJoint)
{
    ui->setupUi(this);

    ui->btnLr01->setStep( 1.0f );

    connect( ui->btnLr01, SIGNAL(signal_step(float)),
             this, SLOT(slot_step(float)));

    mId = 0;
    mStepTime = 1.0f;
}

RoboJoint::~RoboJoint()
{
    delete ui;
}
void RoboJoint::setId( int id )
{
    mId = id;
}
void RoboJoint::setTitle( const QString &title )
{
    ui->groupBox->setTitle( title );
}
void RoboJoint::setAngle( float angle )
{
    ui->lcdNumber->display( angle );
}

void RoboJoint::setAngleVisible( bool b )
{
    ui->lcdNumber->setVisible( b );
}

void RoboJoint::setCcwVisible( bool b )
{ ui->chkInvert->setVisible( b ); }

bool RoboJoint::isCcwVisible()
{ return ui->chkInvert->isVisible(); }

void RoboJoint::setCcwChecked( bool b )
{ ui->chkInvert->setChecked( b ); }

bool RoboJoint::isCcwChecked()
{ return ui->chkInvert->isChecked(); }

void RoboJoint::setStepTime( float t )
{ mStepTime = t; }
float RoboJoint::stepTime()
{ return mStepTime; }

void RoboJoint::actionChanged( const QDateTime &endTime, int endVal )
{
    float dt = mPressTime.msecsTo( endTime );

    QString strInfo;

    mCurValue = endVal;
    strInfo = QString( "%2%3 %1ms" ).arg( dt ).arg( endVal-mPressValue ).arg(QChar(0x00B0));
    ui->label_2->setText( strInfo );
}

void RoboJoint::rotate( float ang, float ts )
{
    QString strInfo;
    strInfo = QString( "%2%3 %1ms" ).arg( ts * 1000 ).arg( ang ).arg(QChar(0x00B0));
    ui->label_2->setText( strInfo );

    //! move
    emit signal_actionChanged( mId, ts, ang );
}

void RoboJoint::on_doubleSpinBox_valueChanged(double arg1)
{
    ui->horizontalSlider->setValue( arg1 );
}

void RoboJoint::on_horizontalSlider_valueChanged(int value)
{
    ui->doubleSpinBox->setValue( value );
}

void RoboJoint::on_horizontalSlider_sliderPressed()
{
    mPressTime = QDateTime::currentDateTime();
    mPressValue = ui->horizontalSlider->value();
}

void RoboJoint::on_horizontalSlider_sliderReleased()
{
    actionChanged( QDateTime::currentDateTime(), mCurValue );   //! show time

    float dT = mPressTime.msecsTo( QDateTime::currentDateTime() );

    //! only emit on release
//    emit signal_actionChanged( mId, dT, (ui->horizontalSlider->value() - mPressValue) );

    rotate( (ui->horizontalSlider->value() - mPressValue), dT / 1000.0 );
}

void RoboJoint::on_horizontalSlider_sliderMoved(int position)
{
    actionChanged( QDateTime::currentDateTime(), ui->horizontalSlider->value() );
}

void RoboJoint::on_pushButton_clicked()
{
    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        emit signal_zeroClicked( mId, ui->chkInvert->isChecked() );
    }
}

void RoboJoint::slot_step( float stp )
{
    //! slider value changed
    ui->doubleSpinBox->setValue( ui->doubleSpinBox->value() + stp );

    rotate( stp, mStepTime );
}
