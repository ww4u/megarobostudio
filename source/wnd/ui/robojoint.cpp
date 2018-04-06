#include "robojoint.h"
#include "ui_robojoint.h"
#include "../widget/megamessagebox.h"
RoboJoint::RoboJoint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoboJoint)
{
    ui->setupUi(this);

    mId = 0;
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

void RoboJoint::actionChanged( const QDateTime &endTime, int endVal )
{
    float dt = mPressTime.msecsTo( endTime );

    QString strInfo;

    strInfo = QString( "%1ms %2%3" ).arg( dt ).arg( endVal-mPressValue ).arg(QChar(0x00B0));
    ui->label_2->setText( strInfo );
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
    actionChanged( QDateTime::currentDateTime(), ui->horizontalSlider->value() );

    float dT = mPressTime.msecsTo( QDateTime::currentDateTime() );

    //! only emit on release
    emit signal_actionChanged( mId, dT, (ui->horizontalSlider->value() - mPressValue) );
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
