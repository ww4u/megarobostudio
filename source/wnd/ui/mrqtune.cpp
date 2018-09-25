#include "mrqtune.h"
#include "ui_mrqtune.h"
#include "../../com/comassist.h"
#define reg_unit    0.1f    //! %

MrqTune::MrqTune(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqTune)
{
    ui->setupUi(this);
}

MrqTune::~MrqTune()
{
    delete ui;
}

int MrqTune::setApply()
{
    return apply();
}
void MrqTune::modelChanged()
{ updateUi(); }

void MrqTune::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkOnOff,
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };
    QDoubleSpinBox *doubleSpinBoxes[]={

        ui->spinDown,
        ui->spinUp,
    };

    QComboBox *comboxes[]={
        ui->cmbMiniRatio,
        ui->cmbRegFall,
        ui->cmbRegRise

    };

    install_spy();
}
void MrqTune::setupUi()
{}
void MrqTune::desetupUi()
{}

int MrqTune::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    //! tune able
    if ( pDevice->tunningAble() )
    {}
    else
    { return 0; }

    pDevice->setTUNING_STATE( mAxesId, (MRQ_SYSTEM_REVMOTION)ui->chkOnOff->isChecked() );
    pDevice->setTUNING_MINICURRRATIO( mAxesId, (MRQ_DRIVER_MINICURRRATIO)ui->cmbMiniRatio->currentIndex() );

    pDevice->setTUNING_ENERGYEFFIC( mAxesId,
                                    comAssist::align( ui->spinUp->value(), reg_unit ),
                                    comAssist::align( ui->spinDown->value(), reg_unit ) );

    pDevice->setTUNING_CURRREGULATE( mAxesId,
                                     (MRQ_TUNING_CURRREGULATE)ui->cmbRegRise->currentIndex(),
                                     (MRQ_TUNING_CURRREGULATE_1)ui->cmbRegFall->currentIndex() );

    return 0;
}
int MrqTune::updateUi()
{
    Q_ASSERT( NULL != m_pMrqModel );

    ui->chkOnOff->setChecked( m_pMrqModel->mTUNING_STATE[ mAxesId] );
    ui->cmbMiniRatio->setCurrentIndex( m_pMrqModel->mTUNING_MINICURRRATIO[mAxesId] );

    ui->spinUp->setValue( m_pMrqModel->mTUNING_ENERGYEFFIC[mAxesId] * reg_unit );
    ui->spinDown->setValue( m_pMrqModel->mTUNING_ENERGYEFFIC1[mAxesId] * reg_unit );

    ui->cmbRegRise->setCurrentIndex( m_pMrqModel->mTUNING_CURRREGULATE[mAxesId]);
    ui->cmbRegFall->setCurrentIndex( m_pMrqModel->mTUNING_CURRREGULATE1[mAxesId]);

    return 0;
}

void MrqTune::on_spinUp_valueChanged(double arg1)
{
    if ( arg1 <= ui->spinDown->value() )
    { ui->spinUp->setValue( ui->spinDown->value() + 1 ); }
    else
    { }
}

void MrqTune::on_spinDown_valueChanged(double arg1)
{
    if ( arg1 >= ui->spinUp->value() )
    { ui->spinDown->setValue( ui->spinUp->value() - 1); }
    else
    {}
}
