#include "mrqaxes.h"
#include "ui_mrqaxes.h"
#include "../../com/comassist.h"

//! convert
#define driver_current_unit (0.1f)

#define motor_volt_unit     (0.1f)
#define motor_current_unit  (0.1f)
#define driver_time_unit    (0.001f)

mrqAxes::mrqAxes(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqAxes)
{
    ui->setupUi(this);

    setupUi();

    spyEdited();
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
    updateUi();
}

void mrqAxes::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkIdle,
    };

    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={
        ui->spinSlowMotor,
        ui->spinSlowGear,
        ui->spinEncLine,
    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinMaxVolt,
        ui->spinMaxCurrent,
        ui->spinMaxVelocity,
        ui->spinMaxCurrent,

        ui->spinMaxAcc,
        ui->spinIdleCurrent,
        ui->spinIdleTime,
        ui->dblCurrent,

        ui->spinDistance,
        ui->spinInvertGap,
    };

    QComboBox *comboxes[]={
        ui->cmbSize,
        ui->cmbStepAngle,
        ui->cmbMotionType,
        ui->cmbPosUnit,

        ui->cmbEncState,
        ui->cmbEncType,
        ui->cmbEncChs,
        ui->cmbEncMul,

        ui->cmbDrvVernier,
    };

    install_spy();
}

void mrqAxes::setupUi()
{
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

    //! volt
    pDevice->setMOTOR_VOLTAGE( mAxesId, comAssist::align( ui->spinMaxVolt->value(),motor_volt_unit) );
    pDevice->setMOTOR_CURRENT( mAxesId, comAssist::align( ui->spinMaxCurrent->value(),motor_current_unit) );
    pDevice->setMOTOR_PEAKSPEED( mAxesId, ui->spinMaxVelocity->value() );
    pDevice->setMOTOR_PEAKACCELERATION( mAxesId, ui->spinMaxAcc->value() );

    pDevice->setDRIVER_IDLECURRENT( mAxesId, comAssist::align( ui->spinIdleCurrent->value(),motor_current_unit) );
    pDevice->setDRIVER_SWITCHTIME( mAxesId, comAssist::align( ui->spinIdleTime->value(),driver_time_unit) );

    //! driver
    pDevice->setDRIVER_CURRENT( mAxesId, comAssist::align( ui->dblCurrent->value(),driver_current_unit) );
    pDevice->setDRIVER_MICROSTEPS( mAxesId, (MRQ_DRIVER_MICROSTEPS)ui->cmbDrvVernier->currentIndex() );

    //! encoder
    pDevice->setENCODER_STATE( mAxesId, (MRQ_ENCODER_STATE)ui->cmbEncState->currentIndex() );
    pDevice->setENCODER_TYPE( mAxesId,
                                  (MRQ_ENCODER_TYPE)ui->cmbEncType->currentIndex() );
    pDevice->setENCODER_CHANNELNUM( mAxesId,
                                        (MRQ_ENCODER_CHANNELNUM)ui->cmbEncChs->currentIndex() );
    pDevice->setENCODER_MULTIPLE( mAxesId,
                                      (MRQ_ENCODER_MULTIPLE)ui->cmbEncMul->currentIndex() );
    pDevice->setENCODER_LINENUM( mAxesId,
                                 ui->spinEncLine->value() );

    //! lead
    pDevice->setMOTOR_GEARRATIONUM( mAxesId, ui->spinSlowMotor->value() );
    pDevice->setMOTOR_GEARRATIODEN( mAxesId, ui->spinSlowGear->value() );

    pDevice->setMOTOR_LEAD( mAxesId, ui->spinDistance->value() );
    pDevice->setMOTOR_BACKLASH( mAxesId, ui->spinInvertGap->value() );

    return 0;
}

//! model -> view
int mrqAxes::updateUi()
{logDbg();
    Q_ASSERT( NULL != m_pMrqModel );

    MegaDevice::MRQ_model *pModel;
    pModel = m_pMrqModel->getModel();
logDbg();
    //! motor
    ui->cmbSize->setCurrentIndex( pModel->mMOTOR_SIZE[ mAxesId ] );
    ui->cmbStepAngle->setCurrentIndex( pModel->mMOTOR_STEPANGLE[mAxesId] );
    ui->cmbMotionType->setCurrentIndex( pModel->mMOTOR_TYPE[mAxesId] );
    ui->cmbPosUnit->setCurrentIndex( pModel->mMOTOR_POSITIONUNIT[mAxesId] );

    //! volt
    ui->spinMaxVolt->setValue( pModel->mMOTOR_VOLTAGE[mAxesId]*motor_volt_unit );
    ui->spinMaxCurrent->setValue( ( pModel->mMOTOR_CURRENT[mAxesId]*motor_current_unit ) );
    ui->spinMaxVelocity->setValue( ( pModel->mMOTOR_PEAKSPEED[mAxesId] ) );
    ui->spinMaxAcc->setValue( ( pModel->mMOTOR_PEAKACCELERATION[mAxesId] ) );

    ui->spinIdleCurrent->setValue( pModel->mDRIVER_IDLECURRENT[mAxesId]*driver_current_unit );
    ui->spinIdleTime->setValue( pModel->mDRIVER_SWITCHTIME[mAxesId]*driver_time_unit );

    //! driver
    ui->dblCurrent->setValue( pModel->mDRIVER_CURRENT[mAxesId]*driver_current_unit );
    ui->cmbDrvVernier->setCurrentIndex( pModel->mDRIVER_MICROSTEPS[mAxesId] );

    //! encoder
    ui->cmbEncState->setCurrentIndex( pModel->mENCODER_STATE[mAxesId] );
    ui->cmbEncType->setCurrentIndex( pModel->mENCODER_TYPE[mAxesId] );
    ui->cmbEncChs->setCurrentIndex( pModel->mENCODER_CHANNELNUM[mAxesId] );
    ui->cmbEncMul->setCurrentIndex( pModel->mENCODER_MULTIPLE[mAxesId] );
    ui->spinEncLine->setValue( pModel->mENCODER_LINENUM[mAxesId] );

    //! transfer
    ui->spinSlowGear->setValue( ( pModel->mMOTOR_GEARRATIODEN[mAxesId]) );
    ui->spinSlowMotor->setValue( ( pModel->mMOTOR_GEARRATIONUM[mAxesId]) );

    ui->spinDistance->setValue( ( pModel->mMOTOR_LEAD[mAxesId]) );
    ui->spinInvertGap->setValue( ( pModel->mMOTOR_BACKLASH[mAxesId]) );

    return 0;
}
