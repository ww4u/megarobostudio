#include "mrqaxes.h"
#include "ui_mrqaxes.h"

#define driver_current_unit (0.1f)

mrqAxes::mrqAxes(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqAxes)
{
    ui->setupUi(this);

    setupUi();
}

mrqAxes::~mrqAxes()
{
    delete ui;

    desetupUi();
}

int mrqAxes::setApply()
{
    return apply();
}

void mrqAxes::modelChanged()
{
    updateData();
}

void mrqAxes::setupUi()
{
    ui->maxVolt->setValidator( &dValidator );
    ui->maxCurrent->setValidator( &dValidator );
    ui->maxSpeed->setValidator( &dValidator );
    ui->maxAcc->setValidator( &dValidator );

    ui->cmbIdleCurrent->setValidator( &dValidator );
    ui->cmbIdleTime->setValidator( &dValidator );
}
void mrqAxes::desetupUi()
{}

//! view->model
int mrqAxes::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    //! for each data
    pDevice->setMOTOR_SIZE( mAxesId,
                                (MRQ_MOTOR_SIZE)ui->cmbSize->currentIndex() );
    pDevice->setMOTOR_STEPANGLE( mAxesId,
                                     (MRQ_MOTOR_STEPANGLE)ui->cmbStepAngle->currentIndex() );

    pDevice->setMOTOR_TYPE( mAxesId,
                                (MRQ_MOTOR_TYPE)ui->cmbMotionType->currentIndex() );
    pDevice->setMOTOR_POSITIONUNIT( mAxesId,
                                        (MRQ_MOTOR_POSITIONUNIT)ui->cmbPosUnit->currentIndex() );

    //! \todo limit

    pDevice->setMOTOR_VOLTAGE( mAxesId, ui->maxVolt->currentText().toFloat() );
    pDevice->setMOTOR_CURRENT( mAxesId, ui->maxCurrent->currentText().toFloat() );
    pDevice->setMOTOR_PEAKSPEED( mAxesId, ui->maxSpeed->currentText().toFloat() );
    pDevice->setMOTOR_PEAKACCELERATION( mAxesId, ui->maxAcc->currentText().toFloat() );

    //! \todo idle
    //! idle current
    //! idle time

    //! \todo float->u8
    pDevice->setDRIVER_CURRENT( mAxesId, ui->dblCurrent->value()/driver_current_unit );
    pDevice->setDRIVER_MICROSTEPS( mAxesId, (MRQ_DRIVER_MICROSTEPS)ui->cmbDrvVernier->currentIndex() );

    //! encoder
    pDevice->setENCODER_TYPE( mAxesId,
                                  (MRQ_ENCODER_TYPE)ui->cmbEncType->currentIndex() );
    pDevice->setENCODER_CHANNELNUM( mAxesId,
                                        (MRQ_ENCODER_CHANNELNUM)ui->cmbEncChs->currentIndex() );
    pDevice->setENCODER_MULTIPLE( mAxesId,
                                      (MRQ_ENCODER_MULTIPLE)ui->cmbEncMul->currentIndex() );

    //! lead
    //! \todo
    pDevice->setMOTOR_GEARRATIONUM( mAxesId, ui->cmbTransN->currentText().toInt() );
    pDevice->setMOTOR_GEARRATIODEN( mAxesId, ui->cmbTransM->currentText().toInt() );

    pDevice->setMOTOR_LEAD( mAxesId, ui->cmbTransDist->currentText().toFloat() );
    pDevice->setMOTOR_BACKLASH( mAxesId, ui->cmbInverseGap->currentText().toFloat() );

    return 0;
}

//! model -> view
int mrqAxes::updateData()
{
    if ( NULL == m_pMrqModel )
    { return -1; }

    MegaDevice::MRQ_model *pModel;
    pModel = m_pMrqModel->getModel();

    ui->cmbSize->setCurrentIndex( pModel->mMOTOR_SIZE[ mAxesId ] );
    ui->cmbStepAngle->setCurrentIndex( pModel->mMOTOR_STEPANGLE[mAxesId] );
    ui->cmbMotionType->setCurrentIndex( pModel->mMOTOR_TYPE[mAxesId] );
    ui->cmbPosUnit->setCurrentIndex( pModel->mMOTOR_POSITIONUNIT[mAxesId] );

    ui->maxVolt->setCurrentText( QString::number( pModel->mMOTOR_VOLTAGE[mAxesId] ) );
    ui->maxCurrent->setCurrentText( QString::number( pModel->mMOTOR_CURRENT[mAxesId] ) );
    ui->maxSpeed->setCurrentText( QString::number( pModel->mMOTOR_PEAKSPEED[mAxesId] ) );
    ui->maxAcc->setCurrentText( QString::number( pModel->mMOTOR_PEAKACCELERATION[mAxesId] ) );

    ui->dblCurrent->setValue( pModel->mDRIVER_CURRENT[mAxesId]*driver_current_unit );
    ui->cmbDrvVernier->setCurrentIndex( pModel->mDRIVER_MICROSTEPS[mAxesId] );

    ui->cmbEncType->setCurrentIndex( pModel->mENCODER_TYPE[mAxesId] );
    ui->cmbEncChs->setCurrentIndex( pModel->mENCODER_CHANNELNUM[mAxesId] );
    ui->cmbEncMul->setCurrentIndex( pModel->mENCODER_MULTIPLE[mAxesId] );

    ui->cmbTransN->setCurrentText( QString::number( pModel->mMOTOR_GEARRATIODEN[mAxesId]) );
    ui->cmbTransM->setCurrentText( QString::number( pModel->mMOTOR_GEARRATIONUM[mAxesId]) );

    ui->cmbTransDist->setCurrentText( QString::number( pModel->mMOTOR_LEAD[mAxesId]) );
    ui->cmbInverseGap->setCurrentText( QString::number( pModel->mMOTOR_BACKLASH[mAxesId]) );

    return 0;
}
