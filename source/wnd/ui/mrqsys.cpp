#include "mrqsys.h"
#include "ui_mrqsys.h"

#define otp_temprature_unit (0.1f)

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

int mrqSys::setApply()
{
    return apply();
}

void mrqSys::modelChanged()
{ updateUi(); }

void mrqSys::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkOtp,
    };

    QLineEdit *edits[]={
        ui->edtAlias,
    };

    QSpinBox *spinBoxes[]={
    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinOtpTemperature,
    };

    QComboBox *comboxes[]={
        ui->cmbAction,
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
//    pDevice->setOTP_STATE( (MRQ_SYSTEM_REPORTSWITCH)ui->chkOtp->isChecked() );
//    pDevice->setOTP_THRESHOLD( ui->spinOtpTemperature->value()/otp_temprature_unit );
//    pDevice->setOTP_RESPONSE(
//                (MRQ_OUTOFSTEP_LINERESPONSE)ui->cmbAction->currentIndex()
//                );

    //! name
    pDevice->setName(
                 ui->edtAlias->text()
                );
    return 0;

}

int mrqSys::updateUi()
{
    //! otp
//    ui->chkOtp->setChecked( (bool)m_pMrqModel->mOTP_STATE );
//    ui->spinOtpTemperature->setValue( ( m_pMrqModel->mOTP_THRESHOLD)*otp_temprature_unit );
//    ui->cmbAction->setCurrentIndex( m_pMrqModel->mOTP_RESPONSE );

    Q_ASSERT( NULL != m_pMrqModel );

    //! name
    ui->edtAlias->setText( m_pMrqModel->getName() );

    //! \todo warning

    return 0;
}

void mrqSys::on_chkLed_clicked(bool checked)
{
    Q_ASSERT( getDevice() != NULL );

    getDevice()->setCAN_NETMANAGELED( (MRQ_CAN_NETMANAGELED)checked );
}
