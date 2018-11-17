#include "robojoint.h"
#include "ui_robojoint.h"
#include "../widget/megamessagebox.h"

#include "../../include/mcstd.h"

RoboJoint::RoboJoint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoboJoint)
{
    ui->setupUi(this);

    ui->btnLr01->setStep( 1.0f );

    connect( ui->btnLr01, SIGNAL(signal_step(float, bool)),
             this, SLOT(slot_step(float, bool)));

    connect( ui->btnLr01, SIGNAL(signal_step(float)),
             this, SLOT(slot_step(float)));

    mId = 0;
    mStepTime = 1.0f;
    mbSingle = true;
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
void RoboJoint::setAngle( float angle,
                          float deltaAngle )
{
    ui->lcdNumber->display( angle );
    ui->lcdDelta->display( deltaAngle );
}

void RoboJoint::setAngleVisible( bool b, bool bDeltaAngle )
{
    ui->lcdNumber->setVisible( b );
    ui->lcdDelta->setVisible( bDeltaAngle );
    ui->labelDelta->setVisible( bDeltaAngle );
}

void RoboJoint::setCcwVisible( bool b )
{ ui->chkInvert->setVisible( b ); }

bool RoboJoint::isCcwVisible()
{ return ui->chkInvert->isVisible(); }

void RoboJoint::setCcwChecked( bool b )
{ ui->chkInvert->setChecked( b ); }

bool RoboJoint::isCcwChecked()
{ return ui->chkInvert->isChecked(); }

void RoboJoint::setAlignAble( bool b )
{ ui->btnNorm->setVisible( b );}
bool RoboJoint::alignAble()
{ return ui->btnNorm->isVisible(); }

void RoboJoint::setStepTime( float t )
{ mStepTime = t; }
float RoboJoint::stepTime()
{ return mStepTime; }

void RoboJoint::setSingle( bool b )
{ mbSingle = b; }
bool RoboJoint::single()
{ return mbSingle; }

void RoboJoint::actionChanged( const QDateTime &endTime, int endVal )
{
    float dt = mPressTime.msecsTo( endTime );

    QString strInfo;

    mCurValue = endVal;
    strInfo = QString( "%2%3 %1ms" ).arg( dt ).arg( endVal-mPressValue ).arg( char_deg );
    ui->label_2->setText( strInfo );
}

void RoboJoint::rotate( float ang, float ts, float ev )
{
    QString strInfo;
    strInfo = QString( "%2%3 %1ms" ).arg( ts * 1000 ).arg( ang ).arg( char_deg );
    ui->label_2->setText( strInfo );

    //! move
    emit signal_actionChanged( mId, ts, ang, ev );
}

void RoboJoint::stop()
{ emit signal_stop( mId ); }

void RoboJoint::on_doubleSpinBox_valueChanged(double arg1)
{
    ui->horizontalSlider->setValue( arg1 );logDbg();
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

void RoboJoint::on_btnNorm_clicked()
{
    emit signal_align_clicked( mId );
}

//! step by click
void RoboJoint::slot_step( float stp )
{
    //! only valid in single
    if ( mbSingle )
    {}
    else
    { return; }

    //! slider value changed
    ui->doubleSpinBox->setValue( ui->doubleSpinBox->value() + stp );

    rotate( stp, mStepTime );
}

void RoboJoint::slot_step( float stp, bool b )
{
    //! invalid in single mode
    if ( mbSingle )
    { return; }
    else
    {}

    if ( b )
    {
        //! slider value changed
        ui->doubleSpinBox->setValue( ui->doubleSpinBox->value() + stp );

        rotate( stp, mStepTime, stp/mStepTime );
    }
    else
    { stop(); }
}

