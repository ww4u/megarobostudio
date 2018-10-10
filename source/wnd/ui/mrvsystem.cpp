#include "mrvsystem.h"
#include "ui_mrvsystem.h"

MrvSystem::MrvSystem(QWidget *parent) :
    mrvView(parent),
    ui(new Ui::MrvSystem)
{
    ui->setupUi(this);
}

MrvSystem::~MrvSystem()
{
    delete ui;
}

int MrvSystem::setApply()
{
    return apply();
}

void MrvSystem::modelChanged()
{
    updateUi();
}

QString MrvSystem::deviceName()
{ return ui->edtAlias->text(); }

void MrvSystem::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {

    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={
        ui->edtAlias
    };

    QSpinBox *spinBoxes[]={
        ui->spinPwm
    };
    QDoubleSpinBox *doubleSpinBoxes[]={

    };

    QComboBox *comboxes[]={
        ui->cmbPwr,
    };

    install_spy();
}

int MrvSystem::apply()
{
    Q_ASSERT( NULL != m_pMRV );

    int ret;
    checked_call( m_pMRV->setSYSTEM_POWERON( (MRV_SYSTEM_POWERON)ui->cmbPwr->currentIndex() ) );
    checked_call( m_pMRV->setVALVECTRL_PWMFREQ( (uint32)ui->spinPwm->value() ) );

    return 0;
}
int MrvSystem::updateUi()
{
    Q_ASSERT( NULL != m_pMRV );

    ui->edtAlias->setText( m_pMRV->name() );
    ui->cmbPwr->setCurrentIndex( (int)m_pMRV->mSYSTEM_POWERON );
    ui->spinPwm->setValue( m_pMRV->mVALVECTRL_PWMFREQ );    //! 1Hz

    return 0;
}

