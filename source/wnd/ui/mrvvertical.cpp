#include "mrvvertical.h"
#include "ui_mrvvertical.h"

#include "../../com/comassist.h"

#define time_unit   (0.001f)
#define duty_unit   (0.001f)

MrvVertical::MrvVertical(QWidget *parent) :
    mrvView(parent),
    ui(new Ui::MrvVertical)
{
    ui->setupUi(this);
}

MrvVertical::~MrvVertical()
{
    delete ui;
}

int MrvVertical::setApply()
{
    return apply();
}

void MrvVertical::modelChanged()
{
//    adaptUi();

    updateUi();
}

void MrvVertical::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkLed,
    };
    QRadioButton *radBoxes[]=
    {
    };

    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinDelayTime,
        ui->spinOpenTime,

        ui->spinOpenDuty,
        ui->spinHoldDuty,
        ui->spinIdleDuty,
    };

    QComboBox *comboxes[]={
        ui->cmbDevice,
        ui->cmbAction,

    };

    install_spy();
}

//! view->model
int MrvVertical::apply()
{
    m_pMRV->setVALVECTRL_DEVICE( mAxesId, (MRV_VALVECTRL_DEVICE)ui->cmbDevice->currentIndex() );

    m_pMRV->setGLOBAL_DISTINGUISH( mAxesId, (MRV_CAN_NETMANAGELED)ui->chkLed->isChecked() );
    m_pMRV->setVALVECTRL_ACTION( mAxesId, (MRV_VALVECTRL_ACTION)ui->cmbAction->currentIndex() );

    m_pMRV->setVALVECTRL_HOLDDUTY( mAxesId, comAssist::align( ui->spinHoldDuty->value(), duty_unit ) );
    m_pMRV->setVALVECTRL_OPENDUTY( mAxesId, comAssist::align( ui->spinOpenDuty->value(), duty_unit ) );
    m_pMRV->setVALVECTRL_IDLEDUTY( mAxesId, comAssist::align( ui->spinHoldDuty->value(), duty_unit ) );

    m_pMRV->setVALVECTRL_OPENTIME( mAxesId, comAssist::align( ui->spinOpenTime->value(), time_unit ) );
    m_pMRV->setVALVECTRL_OPENDLYTIME( mAxesId, comAssist::align( ui->spinDelayTime->value(), time_unit ) );

    return 0;
}

//! model -> view
int MrvVertical::updateUi()
{
    ui->cmbDevice->setCurrentIndex( m_pMRV->mVALVECTRL_DEVICE[mAxesId] );

    ui->chkLed->setChecked( m_pMRV->mGLOBAL_DISTINGUISH[mAxesId] );
    ui->cmbAction->setCurrentIndex( m_pMRV->mVALVECTRL_ACTION[mAxesId] );

    ui->spinIdleDuty->setValue( m_pMRV->mVALVECTRL_IDLEDUTY[mAxesId] * duty_unit );
    ui->spinOpenDuty->setValue( m_pMRV->mVALVECTRL_OPENDUTY[mAxesId] * duty_unit );
    ui->spinHoldDuty->setValue( m_pMRV->mVALVECTRL_HOLDDUTY[mAxesId] * duty_unit );

    ui->spinOpenTime->setValue( m_pMRV->mVALVECTRL_OPENTIME[mAxesId] * time_unit );
    ui->spinDelayTime->setValue( m_pMRV->mVALVECTRL_OPENDLYTIME[mAxesId] * time_unit );

    return 0;
}
