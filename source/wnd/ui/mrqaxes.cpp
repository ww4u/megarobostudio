#include "mrqaxes.h"
#include "ui_mrqaxes.h"
#include "../../com/comassist.h"

//! convert
#define driver_current_unit (0.1f)

#define motor_volt_unit     (1.0f)
#define motor_current_unit  (0.1f)
#define driver_time_unit    (0.001f)

#define mm_unit_index       MRQ_MOTOR_POSITIONUNIT_MILLIMETER
#define rotate_type_index   MRQ_MOTOR_TYPE_ROTARY

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
    adaptUi();

    updateUi();
}

void mrqAxes::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
//        ui->chkDriverState,
//        ui->chk820,
    };
    QRadioButton *radBoxes[] = {
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

        ui->spin820Current,
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

        ui->cmb820MicroStep,
    };

    install_spy();
}

void mrqAxes::setupUi()
{
    mRotateDependList.append( ui->label_19 );
    mRotateDependList.append( ui->spinSlowMotor );
    mRotateDependList.append( ui->spinSlowGear );
    mRotateDependList.append( ui->label_5 );
    mRotateDependList.append( ui->label_8 );

    mLineDependList.append( ui->label_20 );
    mLineDependList.append( ui->spinDistance );
}
void mrqAxes::desetupUi()
{}

void mrqAxes::adaptUi()
{
    if ( ui->cmbDrvVernier->count() > 0 )
    {}
    else
    {
        //! microstep
        //! fill the micro step
        QStringList microList;
        int base;

        m_pMrqModel->microStepAttr( microList, base );
        for ( int i = base; i < microList.size(); i++ )
        {
            ui->cmbDrvVernier->addItem( microList.at(i), i );
            ui->cmb820MicroStep->addItem( microList.at(i), i );
        }
    }

    //! encoder able
    ui->groupBox_3->setVisible( m_pMrqModel->encoderAble() );

    //! driver type
    if ( m_pMrqModel->driverId() == VRobot::motor_driver_262 )
    {
        ui->gb_driver->setVisible( true );
        ui->gpSt820->setVisible( false );
    }
    else
    {
        ui->gb_driver->setVisible( false );
        ui->gpSt820->setVisible( true );
    }
}

//! view->model
int mrqAxes::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    //! for each data
    pDevice->setMOTOR_SIZE( mAxesId,
                           (MRQ_MOTOR_SIZE)ui->cmbSize->currentIndex(), DIFF_APPLY );

    pDevice->setMOTOR_STEPANGLE( mAxesId,
                                 (MRQ_MOTOR_STEPANGLE)ui->cmbStepAngle->currentIndex(),DIFF_APPLY );

    pDevice->setMOTOR_TYPE( mAxesId,
                                (MRQ_MOTOR_TYPE)ui->cmbMotionType->currentIndex(), DIFF_APPLY );
    if ( (MRQ_MOTOR_TYPE)ui->cmbMotionType->currentIndex() == MRQ_MOTOR_TYPE_ROTARY )
    {
        pDevice->setMOTOR_POSITIONUNIT( mAxesId,
                                        (MRQ_MOTOR_POSITIONUNIT)ui->cmbPosUnit->currentIndex(), DIFF_APPLY );
    }
    else
    {
        pDevice->setMOTOR_POSITIONUNIT( mAxesId,
                                        MRQ_MOTOR_POSITIONUNIT_MILLIMETER, DIFF_APPLY );
    }

    //! volt
    pDevice->setMOTOR_VOLTAGE( mAxesId, comAssist::align( ui->spinMaxVolt->value(),motor_volt_unit), DIFF_APPLY );
    pDevice->setMOTOR_CURRENT( mAxesId, comAssist::align( ui->spinMaxCurrent->value(),motor_current_unit), DIFF_APPLY );
    pDevice->setMOTOR_PEAKSPEED( mAxesId, ui->spinMaxVelocity->value(), DIFF_APPLY );
    pDevice->setMOTOR_PEAKACCELERATION( mAxesId, ui->spinMaxAcc->value(), DIFF_APPLY );

    //! driver
    if ( m_pMrqModel->driverId() == VRobot::motor_driver_262 )
    {
        pDevice->setDRIVER_STATE( mAxesId, (MRQ_SYSTEM_REVMOTION)ui->gb_driver->isChecked(), DIFF_APPLY );
        pDevice->setDRIVER_IDLECURRENT( mAxesId, comAssist::align( ui->spinIdleCurrent->value(),motor_current_unit), DIFF_APPLY );
        pDevice->setDRIVER_SWITCHTIME( mAxesId, comAssist::align( ui->spinIdleTime->value(),driver_time_unit), DIFF_APPLY );

        pDevice->setDRIVER_CURRENT( mAxesId, comAssist::align( ui->dblCurrent->value(),driver_current_unit), DIFF_APPLY );
        pDevice->setDRIVER_MICROSTEPS( mAxesId, (MRQ_DRIVER_MICROSTEPS)ui->cmbDrvVernier->value(), DIFF_APPLY );
    }
    else if ( m_pMrqModel->driverId() == VRobot::motor_driver_820 )
    {
        pDevice->setNEWDRIVER_STATE( mAxesId, (MRQ_SYSTEM_REVMOTION)ui->gpSt820->isChecked(), DIFF_APPLY );

        pDevice->setNEWDRIVER_CURRENT( comAssist::align( ui->spin820Current->value(),driver_current_unit), DIFF_APPLY );
        pDevice->setNEWDRIVER_MICROSTEPS( (MRQ_NEWDRIVER_MICROSTEPS)ui->cmb820MicroStep->value(), DIFF_APPLY );
    }
    else
    {}

    //! encoder
    if ( m_pMrqModel->encoderAble() )
    {
        pDevice->setENCODER_STATE( mAxesId, (MRQ_ENCODER_STATE)ui->cmbEncState->currentIndex(), DIFF_APPLY );
        pDevice->setENCODER_TYPE( mAxesId,
                                      (MRQ_ENCODER_TYPE)ui->cmbEncType->currentIndex(), DIFF_APPLY );
        pDevice->setENCODER_CHANNELNUM( mAxesId,
                                            (MRQ_ENCODER_CHANNELNUM)ui->cmbEncChs->currentIndex(), DIFF_APPLY );
        pDevice->setENCODER_MULTIPLE( mAxesId,
                                          (MRQ_ENCODER_MULTIPLE)ui->cmbEncMul->currentIndex(), DIFF_APPLY );
        pDevice->setENCODER_LINENUM( mAxesId,
                                     ui->spinEncLine->value(), DIFF_APPLY );

        pDevice->setENCODER_FEEDBACKRATIO( mAxesId, ui->spinEncFb->value(), DIFF_APPLY );
    }

    //! lead
    pDevice->setMOTOR_GEARRATIONUM( mAxesId, ui->spinSlowMotor->value(), DIFF_APPLY );
    pDevice->setMOTOR_GEARRATIODEN( mAxesId, ui->spinSlowGear->value(), DIFF_APPLY );

    pDevice->setMOTOR_LEAD( mAxesId, ui->spinDistance->value(), DIFF_APPLY );

    return 0;
}

//! model -> view
int mrqAxes::updateUi()
{
    Q_ASSERT( NULL != m_pMrqModel );

    MegaDevice::MRQ_model *pModel;
    pModel = m_pMrqModel->getModel();

    //! motor
    ui->cmbSize->setCurrentIndex( pModel->mMOTOR_SIZE[ mAxesId ] );
    ui->cmbStepAngle->setCurrentIndex( pModel->mMOTOR_STEPANGLE[mAxesId] );
    ui->cmbMotionType->setCurrentIndex( pModel->mMOTOR_TYPE[mAxesId] ); on_cmbMotionType_currentIndexChanged( pModel->mMOTOR_TYPE[mAxesId] );
    if ( pModel->mMOTOR_TYPE[mAxesId] == rotate_type_index )
    {
        if ( pModel->mMOTOR_POSITIONUNIT[mAxesId] > MRQ_MOTOR_POSITIONUNIT_RADIAN )
        { ui->cmbPosUnit->setCurrentIndex( MRQ_MOTOR_POSITIONUNIT_ANGLE); }
        else
        { ui->cmbPosUnit->setCurrentIndex( pModel->mMOTOR_POSITIONUNIT[mAxesId] ); }
    }
    else
    { }

    //! volt
    ui->spinMaxVolt->setValue( pModel->mMOTOR_VOLTAGE[mAxesId]*motor_volt_unit );
    ui->spinMaxCurrent->setValue( ( pModel->mMOTOR_CURRENT[mAxesId]*motor_current_unit ) );
    ui->spinMaxVelocity->setValue( ( pModel->mMOTOR_PEAKSPEED[mAxesId] ) );
    ui->spinMaxAcc->setValue( ( pModel->mMOTOR_PEAKACCELERATION[mAxesId] ) );

    //! driver
    if ( m_pMrqModel->driverId() == VRobot::motor_driver_262 )
    {
        ui->gb_driver->setChecked( pModel->mDRIVER_STATE[mAxesId] );
        ui->spinIdleCurrent->setValue( pModel->mDRIVER_IDLECURRENT[mAxesId]*driver_current_unit );
        ui->spinIdleTime->setValue( pModel->mDRIVER_SWITCHTIME[mAxesId]*driver_time_unit );

        ui->dblCurrent->setValue( pModel->mDRIVER_CURRENT[mAxesId]*driver_current_unit );
        ui->cmbDrvVernier->setValue( pModel->mDRIVER_MICROSTEPS[mAxesId] );
    }
    else if ( m_pMrqModel->driverId() == VRobot::motor_driver_820 )
    {
        ui->gpSt820->setChecked( pModel->mNEWDRIVER_STATE[mAxesId] );
        ui->spin820Current->setValue( pModel->mNEWDRIVER_CURRENT * driver_current_unit );
        ui->cmb820MicroStep->setValue( pModel->mNEWDRIVER_MICROSTEPS );
    }
    else
    {}

    //! encoder
    if ( pModel->encoderAble() )
    {
        ui->cmbEncState->setCurrentIndex( pModel->mENCODER_STATE[mAxesId] );
        ui->cmbEncType->setCurrentIndex( pModel->mENCODER_TYPE[mAxesId] );
        ui->cmbEncChs->setCurrentIndex( pModel->mENCODER_CHANNELNUM[mAxesId] );
        ui->cmbEncMul->setCurrentIndex( pModel->mENCODER_MULTIPLE[mAxesId] );
        ui->spinEncLine->setValue( pModel->mENCODER_LINENUM[mAxesId] );
        ui->spinEncFb->setValue( pModel->mENCODER_FEEDBACKRATIO[mAxesId] );
    }

    //! transfer
    ui->spinSlowGear->setValue( ( pModel->mMOTOR_GEARRATIODEN[mAxesId]) );
    ui->spinSlowMotor->setValue( ( pModel->mMOTOR_GEARRATIONUM[mAxesId]) );

    ui->spinDistance->setValue( ( pModel->mMOTOR_LEAD[mAxesId]) );

    return 0;
}

void mrqAxes::on_cmbMotionType_currentIndexChanged(int index)
{
    bool bRotate;

    if ( index == 0 )
    { bRotate = true; }
    else
    { bRotate = false; }

    //! iter
    foreach( QWidget *pWig, mRotateDependList )
    {
        pWig->setVisible( bRotate );
    }

    foreach( QWidget *pWig, mLineDependList )
    {
        pWig->setVisible( !bRotate );
    }

    if ( bRotate )
    {                                           //! degree
        ui->cmbPosUnit->setVisible( true );
        ui->cmbPosUnitmm->setVisible( false );
    }
    else
    {                                           //! mm
        ui->cmbPosUnit->setVisible( false );
        ui->cmbPosUnitmm->setVisible( true );
    }
}
