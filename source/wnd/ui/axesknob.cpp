#include "axesknob.h"
#include "ui_axesknob.h"
#include <QMessageBox>
#include "../../device/mrq/deviceMRQ.h"

axesKnob::axesKnob( mcModel *pMcModel,
                    int subAx,
                    const QString &connStr,
                    QWidget *parent) :
    //! \note (VRobot *)pMcModel
    RoboView( (VRobot *)pMcModel, RoboView::view_axes, subAx, pMcModel, parent ),
    ui(new Ui::axesKnob)
{    
    ui->setupUi(this);

    //! step
    ui->btnLr01->setStep( 0.1f );
    ui->btnLr1->setStep( 1.0f );
    ui->btnLr10->setStep( 10.0f );

    //! single
    connect( ui->btnLr01, SIGNAL(signal_step(float)),
             this, SLOT(slot_step(float)) );
    connect( ui->btnLr1, SIGNAL(signal_step(float)),
             this, SLOT(slot_step(float)) );
    connect( ui->btnLr10, SIGNAL(signal_step(float)),
             this, SLOT(slot_step(float)) );

    //! keep moving
    connect( ui->btnLr01, SIGNAL(signal_step(float,bool)),
             this, SLOT(slot_step(float,bool)) );
    connect( ui->btnLr1, SIGNAL(signal_step(float,bool)),
             this, SLOT(slot_step(float,bool)) );
    connect( ui->btnLr10, SIGNAL(signal_step(float,bool)),
             this, SLOT(slot_step(float,bool)) );

    ui->labConnection->setText( connStr );

    setAttribute( Qt::WA_DeleteOnClose );
}

axesKnob::~axesKnob()
{
    delete ui;
}

void axesKnob::changeEvent( QEvent *event )
{
    RoboView::changeEvent( event );

    if ( event->type() == QEvent::LanguageChange )
    { ui->retranslateUi( this ); }
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
    strInfo = QString( "%2%3 %1ms" ).arg( dt ).arg( mStopAngle-mStartAngle ).arg( char_deg );
    ui->label->setText( strInfo );
}

void axesKnob::rotate( float angle, float t, bool bKeep )
{
    int ax;
    MegaDevice::deviceMRQ *pMrq = currentDevice( ax );
    if ( NULL == pMrq )
    {
        sysError( tr("Invalid device") );
        return;
    }

    int state = pMrq->fsmState( tpvRegion(ax, ui->pageOption->page() ) );
    if ( state == MegaDevice::mrq_state_idle
         || state == MegaDevice::mrq_state_calcend
         || state == MegaDevice::mrq_state_standby
         || state == MegaDevice::mrq_state_unk
         )
    {

    }
    else
    {
        QMessageBox::warning( this, tr("Warning"), tr("Device is not idle now, try later or STOP!") );
        return;
    }

    int ret;
    //! write only one time
    ret = pMrq->setMOTIONPLAN_CYCLENUM( ax,
                                        (MRQ_MOTION_SWITCH_1)ui->pageOption->page(),
                                        1 );
    if ( ret != 0 )
    { return; }

    //! run the device
    if ( bKeep )
    {
        ret = pMrq->pvtWrite( tpvRegion( ax,
                                         ui->pageOption->page()),
                                t,
                                angle,
                                angle/t
                                );
    }
    else
    {
        ret = pMrq->pvtWrite( tpvRegion( ax,
                                         ui->pageOption->page()),
                            t,
                            angle
                            );
    }
    if ( ret != 0 )
    { return; }

    ret = pMrq->run( tpvRegion(ax, ui->pageOption->page() ) );
    if ( ret != 0 )
    { return; }

    //! view connection
    ui->label->setText( QString("%2%3 %1ms").arg( t*1000 )
                                            .arg( angle ).arg( char_deg ) );

}

void axesKnob::stop()
{
    int ax;
    MegaDevice::deviceMRQ *pMrq = currentDevice( ax );
    if ( NULL == pMrq )
    {
        sysError( tr("Invalid device") );
        return;
    }

    pMrq->stop( tpvRegion(ax, ui->pageOption->page() ) );
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
}

void axesKnob::on_sliderValue_sliderMoved(int position)
{
    actionChanged( QDateTime::currentDateTime(), ui->sliderValue ->value() );
}

void axesKnob::slot_step( float step )
{
    if ( ui->chkSingle->isChecked() )
    {
        rotate( step, ui->spinStepTime->value() );
        ui->spinNow->setValue( ui->spinNow->value() + step );
    }
}

void axesKnob::slot_step( float step, bool bKeep )
{
    if ( ui->chkSingle->isChecked() )
    { return; }
    else
    {  }

    if ( bKeep )
    {
        rotate( step, ui->spinStepTime->value(), true );
        ui->spinNow->setValue( ui->spinNow->value() + step );
    }
    else
    { stop(); }
}
