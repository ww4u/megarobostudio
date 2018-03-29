#include "axesknob.h"
#include "ui_axesknob.h"
#include <QMessageBox>
#include "../../device/mrq/deviceMRQ.h"

axesKnob::axesKnob( mcModel *pModel,
                    QWidget *parent) :
    DlgView( pModel, parent),
    ui(new Ui::axesKnob)
{
    ui->setupUi(this);
}

axesKnob::~axesKnob()
{
    delete ui;
}

MegaDevice::deviceMRQ * axesKnob::currentDevice( int &ax )
{
    Q_ASSERT( NULL != m_pMcModel );

    //! set model && axesid
    QString str;
    int id;
    str = m_pMcModel->getConnection().getDeviceName();
    id = m_pMcModel->getConnection().getDeviceCH();

    MegaDevice::deviceMRQ *pMrq = m_pMcModel->m_pInstMgr->findDevice( str,
                                                                      id );
    ax = id;
    return pMrq;
}

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
    mStopAngle = ui->spinNow->value();

    int ax;
    MegaDevice::deviceMRQ *pMrq = currentDevice( ax );
    if ( NULL == pMrq )
    {
        sysError( tr("Invalid device") );
        return;
    }

    if ( pMrq->fsmState( tpvRegion(ax,0) ) != MegaDevice::mrq_state_idle )
    {
        QMessageBox::warning( this, tr("Warning"), tr("Device is busy now, try later!") );
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
                        time_to_s( mStartTime.msecsTo( mStopTime ) ),
                        mStopAngle - mStartAngle
                        );
    if ( ret != 0 )
    { return; }

    ret = pMrq->run( tpvRegion(ax,0) );
    if ( ret != 0 )
    { return; }

    //! view connection
    ui->labConnection->setText( QString("CH%1@%2").arg( ax + 1 ).arg( m_pMcModel->getConnection().getDeviceName() ) );
    ui->label->setText( QString("%1ms %2%3").arg( mStartTime.msecsTo( mStopTime ) )
                                          .arg( mStopAngle - mStartAngle ).arg(QChar(0x00B0)) );
}
