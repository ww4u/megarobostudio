#include "roboaxes.h"
#include "ui_roboaxes.h"

#include "../../../include/mcstd.h"

#include "../../device/mrq/deviceMRQ.h"

roboAxes::roboAxes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::roboAxes)
{
    ui->setupUi(this);

    mJointLabels[0] = ui->label_11;
    mJointLabels[1] = ui->label_12;
    mJointLabels[2] = ui->label_13;
    mJointLabels[3] = ui->label_14;
    mJointLabels[4] = ui->label_15;

    setModal( false );

    m_pRobo = NULL;
}

roboAxes::~roboAxes()
{
    delete ui;
}

void roboAxes::on_close()
{
}

void roboAxes::on_sldBase_valueChanged(int v )
{ ui->sBoxBase->setValue( v );}
void roboAxes::on_sldBigArm_valueChanged(int v )
{ ui->sBoxBigArm->setValue( v); }
void roboAxes::on_sldLitArm_valueChanged(int v )
{ ui->sBoxLitArm->setValue( v);}
void roboAxes::on_sldWrist_valueChanged(int v)
{ ui->sBoxWrist->setValue( v);}
void roboAxes::on_sldHand_valueChanged(int v)
{ ui->sBoxHand->setValue(v);}

void roboAxes::on_sBoxBase_valueChanged( double v )
{ ui->sldBase->setValue( v );}
void roboAxes::on_sBoxBigArm_valueChanged( double v )
{ ui->sldBigArm->setValue(v);}
void roboAxes::on_sBoxLitArm_valueChanged( double v )
{ ui->sldLitArm->setValue(v);}
void roboAxes::on_sBoxWrist_valueChanged( double v )
{ ui->sldWrist->setValue(v);}
void roboAxes::on_sBoxHand_valueChanged( double v )
{ ui->sldHand->setValue(v);}


#define ROTATE( jointid )   \
                                float dT = mFromTime.msecsTo( mEndTime );\
                            \
                                rotate( jointid, 0, mAngleFrom, dT, mAngleTo );

void roboAxes::on_sldBase_sliderPressed()
{
    mFromTime = QDateTime::currentDateTime();
    mAngleFrom = ui->sBoxBase->value();
}
void roboAxes::on_sldBase_sliderReleased()
{
    mEndTime = QDateTime::currentDateTime();
    mAngleTo = ui->sBoxBase->value();

    ROTATE( 0 );
}

void roboAxes::on_sldBigArm_sliderPressed()
{
    mFromTime = QDateTime::currentDateTime();
    mAngleFrom = ui->sBoxBigArm->value();
}
void roboAxes::on_sldBigArm_sliderReleased()
{
    mEndTime = QDateTime::currentDateTime();
    mAngleTo = ui->sBoxBigArm->value();

    ROTATE( 1 );
}

void roboAxes::on_sldHand_sliderPressed()
{
    mFromTime = QDateTime::currentDateTime();
    mAngleFrom = ui->sBoxHand->value();
}
void roboAxes::on_sldHand_sliderReleased()
{
    mEndTime = QDateTime::currentDateTime();
    mAngleTo = ui->sBoxHand->value();

    ROTATE( 4 );
}

void roboAxes::on_sldLitArm_sliderPressed()
{
    mFromTime = QDateTime::currentDateTime();
    mAngleFrom = ui->sBoxLitArm->value();
}
void roboAxes::on_sldLitArm_sliderReleased()
{
    mEndTime = QDateTime::currentDateTime();
    mAngleTo = ui->sBoxLitArm->value();

    ROTATE( 2 );
}

void roboAxes::on_sldWrist_sliderPressed()
{
    mFromTime = QDateTime::currentDateTime();
    mAngleFrom = ui->sBoxWrist->value();

}
void roboAxes::on_sldWrist_sliderReleased()
{
    mEndTime = QDateTime::currentDateTime();
    mAngleTo = ui->sBoxWrist->value();

    ROTATE( 3 );
}

void roboAxes::attachRobot( VRobot *pRobo )
{
    Q_ASSERT( NULL != pRobo );

    m_pRobo = pRobo;

}
VRobot *roboAxes::Robot()
{
    return m_pRobo;
}

void roboAxes::buildConnection()
{
}

//! convert the time by t
#define time_to_s( t )  ((t)/1000.0)
void roboAxes::rotate( int jointId,
                       float t1, float a1,
                       float t2, float a2 )
{
    logDbg()<<jointId<<t1<<a1<<t2<<a2;

    Q_ASSERT( NULL != m_pRobo );

    int subAx;
    MegaDevice::deviceMRQ *pMrq = m_pRobo->jointDevice( jointId, &subAx );
    if ( NULL == pMrq )
    { return; }

    logDbg()<<subAx<<pMrq->name();

    //! only one time
    pMrq->setMOTIONPLAN_CYCLENUM( subAx, MRQ_MOTION_SWITCH_1_MAIN, 1 );

    pMrq->pvtWrite( subAx,0,
                    time_to_s(t1), a1,
                    time_to_s(t2), a2
                    );
    pMrq->run( subAx );

    //! info
    Q_ASSERT( jointId < sizeof_array( mJointLabels) );
    QString strInfo;
    strInfo = QString( "%1ms %2%3" ).arg( (t2-t1) ).arg( a2-a1 ).arg(QChar(0x00B0));
    mJointLabels[ jointId ]->setText( strInfo );
}
