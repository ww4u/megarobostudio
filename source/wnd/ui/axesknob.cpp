#include "axesknob.h"
#include "ui_axesknob.h"
#include <QMessageBox>
#include "../../device/mrq/deviceMRQ.h"

axesKnob::axesKnob(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::axesKnob)
{
    ui->setupUi(this);

    m_pDevice = NULL;
    mAxesId = 0;
}

axesKnob::~axesKnob()
{
    delete ui;
}

void axesKnob::setDevice( MegaDevice::VDevice *pDevice,
                          int axesId )
{
    Q_ASSERT( NULL != pDevice );
    m_pDevice = pDevice;
    mAxesId = axesId;
}

void axesKnob::setConnection( const QString &conn )
{
    ui->labConnection->setText( conn );

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

    MegaDevice::deviceMRQ *pMrq = static_cast<MegaDevice::deviceMRQ*>(m_pDevice);
    Q_ASSERT( NULL != pMrq );


//    if ( pMrq->status( mAxesId ) !=  )
    if ( pMrq->fsmState(mAxesId) != MegaDevice::mrq_state_idle )
    {
        QMessageBox::warning( this, tr("Warning"), tr("Device is busy now, try later!") );
        return;
    }

    int ret;
    //! write only one time
    ret = pMrq->setMOTIONPLAN_CYCLENUM( mAxesId,
                                        MRQ_MOTION_SWITCH_1_MAIN,
                                        1 );
    if ( ret != 0 )
    { return; }

    //! run the device
    ret = pMrq->pvtWrite( mAxesId,
                          MRQ_MOTION_SWITCH_1_MAIN,
                    0,0,
                    time_to_s( mStartTime.msecsTo( mStopTime ) ),
                    mStopAngle - mStartAngle
                    );
    if ( ret != 0 )
    { return; }

    ret = pMrq->run( mAxesId );
    if ( ret != 0 )
    { return; }

    ui->label->setText( QString("%1ms %2%3").arg( mStartTime.msecsTo( mStopTime ) )
                                          .arg( mStopAngle - mStartAngle ).arg(QChar(0x00B0)) );
}
