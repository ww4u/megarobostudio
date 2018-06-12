#include "axesknob.h"
#include "ui_axesknob.h"
#include <QMessageBox>
#include "../../device/mrq/deviceMRQ.h"

axesKnob::axesKnob( mcModel *pMcModel,
                    const QString &connStr,
                    QWidget *parent) :
    DlgView( pMcModel, parent),
    ui(new Ui::axesKnob)
{    
    ui->setupUi(this);

    //! step
    ui->btnLr01->setStep( 0.1f );
    ui->btnLr1->setStep( 1.0f );
    ui->btnLr10->setStep( 10.0f );

    connect( ui->btnLr01, SIGNAL(signal_step(float)),
             this, SLOT(slot_step(float)) );
    connect( ui->btnLr1, SIGNAL(signal_step(float)),
             this, SLOT(slot_step(float)) );
    connect( ui->btnLr10, SIGNAL(signal_step(float)),
             this, SLOT(slot_step(float)) );

    ui->labConnection->setText( connStr );

    setAttribute( Qt::WA_DeleteOnClose );
}

axesKnob::~axesKnob()
{
    delete ui;
}

MegaDevice::deviceMRQ * axesKnob::currentDevice( int &ax )
{
    Q_ASSERT( NULL != m_pMcModel );

    MegaDevice::deviceMRQ *pMrq = m_pMcModel->m_pInstMgr->findDevice(  ui->labConnection->text(),
                                                                       &ax );
    return pMrq;
}

void axesKnob::actionChanged( const QDateTime &endTime, int valEnd  )
{
    float dt = mStartTime.msecsTo( endTime );

    QString strInfo;

    mStopAngle = valEnd;
    strInfo = QString( "%2%3 %1ms" ).arg( dt ).arg( mStopAngle-mStartAngle ).arg(QChar(0x00B0));
    ui->label->setText( strInfo );
}

void axesKnob::rotate( float angle, float t )
{
    int ax;
    MegaDevice::deviceMRQ *pMrq = currentDevice( ax );
    if ( NULL == pMrq )
    {
        sysError( tr("Invalid device") );
        return;
    }

    if ( pMrq->fsmState( tpvRegion(ax,0) ) != MegaDevice::mrq_state_idle )
    {
        QMessageBox::warning( this, tr("Warning"), tr("Device is not idle now, try later!") );
        return;
    }

    int ret;
    //! write only one time
    ret = pMrq->setMOTIONPLAN_CYCLENUM( ax,
                                        MRQ_MOTION_SWITCH_1_MAIN,
                                        1 );
    if ( ret != 0 )
    { return; }

    //! run the device
    ret = pMrq->pvtWrite( tpvRegion( ax,
                                     (int)MRQ_MOTION_SWITCH_1_MAIN ),
                        0,0,
                        t,
                        angle
                        );
    if ( ret != 0 )
    { return; }

    ret = pMrq->run( tpvRegion(ax,0) );
    if ( ret != 0 )
    { return; }

    //! view connection
    ui->label->setText( QString("%2%3 %1ms").arg( t*1000 )
                                            .arg( angle ).arg(QChar(0x00B0)) );

}

void axesKnob::slot_device_changed()
{ close(); }

void axesKnob::on_sliderValue_valueChanged(int val )
{
    ui->spinNow->setValue( val );
}

void axesKnob::on_spinNow_valueChanged(double arg1)
{
    ui->sliderValue->setValue( arg1 );
}

void axesKnob::on_sliderValue_sliderPressed()
{
    mStartTime = QDateTime::currentDateTime();
    mStartAngle = ui->spinNow->value();
}

#define time_to_s( t )  ((t)/1000.0)
void axesKnob::on_sliderValue_sliderReleased()
{
    mStopTime = QDateTime::currentDateTime();

    actionChanged( mStopTime, mStopAngle );

    rotate( mStopAngle - mStartAngle,
           time_to_s( mStartTime.msecsTo( mStopTime ) ) );

//    int ax;
//    MegaDevice::deviceMRQ *pMrq = currentDevice( ax );
//    if ( NULL == pMrq )
//    {
//        sysError( tr("Invalid device") );
//        return;
//    }

//    if ( pMrq->fsmState( tpvRegion(ax,0) ) != MegaDevice::mrq_state_idle )
//    {
//        QMessageBox::warning( this, tr("Warning"), tr("Device is not idle now, try later!") );
//        return;
//    }

//    int ret;
//    //! write only one time
//    ret = pMrq->setMOTIONPLAN_CYCLENUM( ax,
//                                        MRQ_MOTION_SWITCH_1_MAIN,
//                                        1 );
//    if ( ret != 0 )
//    { return; }

//    //! run the device
//    ret = pMrq->pvtWrite( tpvRegion( ax,
//                                     (int)MRQ_MOTION_SWITCH_1_MAIN ),
//                        0,0,
//                        time_to_s( mStartTime.msecsTo( mStopTime ) ),
//                        mStopAngle - mStartAngle
//                        );
//    if ( ret != 0 )
//    { return; }

//    ret = pMrq->run( tpvRegion(ax,0) );
//    if ( ret != 0 )
//    { return; }

//    //! view connection
//    ui->label->setText( QString("%2%3 %1ms").arg( mStartTime.msecsTo( mStopTime ) )
//                                          .arg( mStopAngle - mStartAngle ).arg(QChar(0x00B0)) );
}

void axesKnob::on_sliderValue_sliderMoved(int position)
{
    actionChanged( QDateTime::currentDateTime(), ui->sliderValue ->value() );
}

void axesKnob::slot_step( float step )
{
    rotate( step, ui->spinStepTime->value() );
    ui->spinNow->setValue( ui->spinNow->value() + step );
}
