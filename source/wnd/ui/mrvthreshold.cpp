#include "mrvthreshold.h"
#include "ui_mrvthreshold.h"

#include "../../com/comassist.h"

#define pressure_unit   (0.001f)
#define current_unit    (0.001f)
#define time_unit       (0.001f)

MrvThreshold::MrvThreshold(QWidget *parent) :
    mrvView(parent),
    ui(new Ui::MrvThreshold)
{
    ui->setupUi(this);
}

MrvThreshold::~MrvThreshold()
{
    delete ui;
}

int MrvThreshold::setApply()
{
    return apply();
}

void MrvThreshold::modelChanged()
{
//    adaptUi();

    updateUi();
}

void MrvThreshold::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {

    };
    QRadioButton *radBoxes[]=
    {

    };

    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={
        ui->spinOnNumLimit,
    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinHighPLimit,
        ui->spinLowPLimit,

        ui->spinHighCurrentLimit,
        ui->spinLowCurrentLimit,

        ui->spinTimeLimit,
    };

    QComboBox *comboxes[]={
        ui->cmbHighPAction,
        ui->cmbLowPAction,

        ui->cmbHighCurrentAction,
        ui->cmbLowCurrentAction,

        ui->cmbOnNumAction,
        ui->cmbTimeAction,
    };

    install_spy();
}

//! view->model
int MrvThreshold::apply()
{
    Q_ASSERT( NULL != m_pMRV );

    //! pressure
    m_pMRV->setTHRESHOLD_HIPRESSUREACTION( mAxesId, (MRV_THRESHOLD_HIPRESSUREACTION)ui->cmbHighPAction->currentIndex() );
    m_pMRV->setTHRESHOLD_HIGHPRESSURE( mAxesId, comAssist::align( ui->spinHighPLimit->value(), pressure_unit ) );

    m_pMRV->setTHRESHOLD_LOWPRESSUREACTION( mAxesId, (MRV_THRESHOLD_HIPRESSUREACTION)ui->cmbLowPAction->currentIndex() );
    m_pMRV->setTHRESHOLD_LOWPRESSURE( mAxesId, comAssist::align( ui->spinLowPLimit->value(), pressure_unit ) );

    //! current
    m_pMRV->setTHRESHOLD_HICURRENTACTION( mAxesId, (MRV_THRESHOLD_HIPRESSUREACTION)ui->cmbHighCurrentAction->currentIndex() );
    m_pMRV->setTHRESHOLD_HIGHCURRENT( mAxesId, comAssist::align( ui->spinHighCurrentLimit->value(), current_unit ) );

    m_pMRV->setTHRESHOLD_LOWCURRENTACTION( mAxesId, (MRV_THRESHOLD_HIPRESSUREACTION)ui->cmbLowCurrentAction->currentIndex() );
    m_pMRV->setTHRESHOLD_LOWCURRENT( mAxesId, comAssist::align( ui->spinLowCurrentLimit->value(), current_unit ) );

    //! on num
    m_pMRV->setTHRESHOLD_NUMSACTION( mAxesId, (MRV_THRESHOLD_HIPRESSUREACTION)ui->cmbOnNumAction->currentIndex() );
    m_pMRV->setTHRESHOLD_ONNUMS( mAxesId, ui->spinOnNumLimit->value() );

    m_pMRV->setTHRESHOLD_TIMEACTION( mAxesId, (MRV_THRESHOLD_HIPRESSUREACTION)ui->cmbTimeAction->currentIndex() );
    m_pMRV->setTHRESHOLD_TIME( mAxesId, comAssist::align( ui->spinTimeLimit->value(), time_unit ) );

    return 0;
}

//! model -> view
int MrvThreshold::updateUi()
{
    //! pressure
    ui->cmbHighPAction->setCurrentIndex( m_pMRV->mTHRESHOLD_HIPRESSUREACTION[mAxesId] );
    ui->spinHighPLimit->setValue( m_pMRV->mTHRESHOLD_HIGHPRESSURE[mAxesId] * pressure_unit );

    ui->cmbLowPAction->setCurrentIndex( m_pMRV->mTHRESHOLD_LOWPRESSUREACTION[mAxesId] );
    ui->spinLowPLimit->setValue( m_pMRV->mTHRESHOLD_LOWPRESSURE[mAxesId] * pressure_unit );

    //! current
    ui->cmbHighCurrentAction->setCurrentIndex( m_pMRV->mTHRESHOLD_HICURRENTACTION[mAxesId] );
    ui->spinHighCurrentLimit->setValue( m_pMRV->mTHRESHOLD_HIGHCURRENT[mAxesId] * current_unit );

    ui->cmbLowCurrentAction->setCurrentIndex( m_pMRV->mTHRESHOLD_LOWCURRENTACTION[mAxesId] );
    ui->spinLowCurrentLimit->setValue( m_pMRV->mTHRESHOLD_LOWCURRENT[mAxesId] * current_unit );

    //! on num
    int val = 0;
    if ( m_pMRV->mTHRESHOLD_ONNUMS[mAxesId] > INT_MAX )
    {  val = INT_MAX; }
    else
    { val = ( m_pMRV->mTHRESHOLD_ONNUMS[mAxesId] ); }
    ui->spinOnNumLimit->setValue( val );

    ui->cmbOnNumAction->setCurrentIndex( m_pMRV->mTHRESHOLD_NUMSACTION[mAxesId] );

    //! time
    ui->spinTimeLimit->setValue( m_pMRV->mTHRESHOLD_TIME[mAxesId] * time_unit );
    ui->cmbTimeAction->setCurrentIndex( m_pMRV->mTHRESHOLD_TIMEACTION[mAxesId] );

    return 0;
}
