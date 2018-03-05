#include "axesknob.h"
#include "ui_axesknob.h"

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

void axesKnob::on_sliderValue_sliderReleased()
{
    mStopTime = QDateTime::currentDateTime();
    mStopAngle = ui->spinNow->value();

    MegaDevice::deviceMRQ *pMrq = static_cast<MegaDevice::deviceMRQ*>(m_pDevice);
    Q_ASSERT( NULL != pMrq );

    //! wait for end
    if ( pMrq->status( mAxesId) == MRQ_MOTION_STATE_RUNNING )
    { logDbg()<<pMrq->status(mAxesId); return; }

    pMrq->pvtWrite( mAxesId,
                    0,0,
                    mStartTime.msecsTo( mStopTime )/1000,       //! to s
                    mStopAngle - mStartAngle
                    );
    pMrq->run( mAxesId );
}
