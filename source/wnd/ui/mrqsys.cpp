#include "mrqsys.h"
#include "ui_mrqsys.h"
#include "../../com/comassist.h"

#define otp_temprature_unit (0.1f)
#define driver_current_unit  (0.1f)

mrqSys::mrqSys(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqSys)
{
    ui->setupUi(this);

    spyEdited();
}

mrqSys::~mrqSys()
{
    delete ui;
}

void mrqSys::changeEvent(QEvent * event)
{
    mrqView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

int mrqSys::setApply()
{
    return apply();
}

void mrqSys::modelChanged()
{
    adaptUi();

    updateUi();
}

QString mrqSys::deviceName()
{ return ui->edtAlias->text(); }

void mrqSys::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkOtp,
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={
        ui->edtAlias,
    };

    QSpinBox *spinBoxes[]={
    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinHighT
    };

    QComboBox *comboxes[]={
        ui->cmbTResp,
    };

    install_spy();
}
void mrqSys::setupUi()
{}
void mrqSys::desetupUi()
{}

int mrqSys::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    //! otp
    if ( m_pMrqModel->temperatures() > 0 )
    {
        pDevice->setOTP_STATE( (MRQ_SYSTEM_REVMOTION)ui->chkOtp->isChecked(), DIFF_APPLY );
        pDevice->setOTP_THRESHOLD( comAssist::align(  ui->spinHighT->value(),otp_temprature_unit), DIFF_APPLY );
        pDevice->setOTP_RESPONSE(
                    (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)ui->cmbTResp->currentIndex(), DIFF_APPLY
                    );
    }

    return 0;
}

int mrqSys::updateUi()
{
    Q_ASSERT( NULL != m_pMrqModel );

    //! otp
    if ( m_pMrqModel->temperatures() > 0 )
    {
        ui->chkOtp->setChecked( (bool)m_pMrqModel->mOTP_STATE );
        ui->spinHighT->setValue( ( m_pMrqModel->mOTP_THRESHOLD)*otp_temprature_unit );
        ui->cmbTResp->setCurrentIndex( m_pMrqModel->mOTP_RESPONSE );
    }

    //! name
    ui->edtAlias->setText( m_pMrqModel->getName() );

    return 0;
}

void mrqSys::adaptUi()
{
    Q_ASSERT( NULL != m_pMrqModel );

    if ( m_pMrqModel->temperatures() > 0 )
    { ui->groupBox_2->setVisible( true ); }
    else
    { ui->groupBox_2->setVisible( false ); }

    {
        ui->gp820->setVisible( false );
    }
}

void mrqSys::on_edtAlias_textEdited(const QString &arg1)
{
    if ( arg1.length() < 1 )
    {
        sysPrompt( tr("Invalid name") );
        ui->edtAlias->setText( m_pMrqModel->getName() );
    }
}
