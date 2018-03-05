#include "mrqsys.h"
#include "ui_mrqsys.h"

mrqSys::mrqSys(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqSys)
{
    ui->setupUi(this);
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
{ updateData(); }

void mrqSys::setupUi()
{}
void mrqSys::desetupUi()
{}

int mrqSys::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    pDevice->setOTP_STATE( (MRQ_SYSTEM_REPORTSWITCH)ui->chkOtp->isChecked() );
    pDevice->setOTP_THRESHOLD( ui->cmbTemp->currentText().toFloat());
    pDevice->setOTP_RESPONSE(
                (MRQ_OUTOFSTEP_LINERESPONSE)ui->cmbAction->currentIndex()
                );
    pDevice->setName(
                 ui->edtAlias->text()
                );
    return 0;

}

int mrqSys::updateData()
{
    ui->chkOtp->setChecked( (bool)m_pMrqModel->mOTP_STATE );
    ui->cmbTemp->setCurrentText( QString::number( m_pMrqModel->mOTP_THRESHOLD));
    ui->cmbAction->setCurrentIndex( m_pMrqModel->mOTP_RESPONSE );
    ui->edtAlias->setText( m_pMrqModel->getName() );
    return 0;

}

void mrqSys::on_chkLed_toggled( bool b )
{
    m_pMrqModel->setCAN_NETMANAGELED( (MRQ_SYSTEM_REPORTSWITCH)b );
}
