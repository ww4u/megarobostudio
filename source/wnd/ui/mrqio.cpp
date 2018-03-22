#include "mrqio.h"
#include "ui_mrqio.h"
#include "../../com/comassist.h"

#define peri_unit   0.001f
#define duty_unit   0.001f

mrqIo::mrqIo(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqIo)
{
    ui->setupUi(this);

    setupUi();
}

mrqIo::~mrqIo()
{
    delete ui;

    desetupUi();
}

int mrqIo::setApply()
{
    return apply();
}

void mrqIo::modelChanged()
{ updateUi(); }

void mrqIo::setupUi()
{}
void mrqIo::desetupUi()
{}

int mrqIo::apply()
{
    return 0;
}

int mrqIo::updateUi()
{ return 0; }

int mrqIo::isoApply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    int ret;

    //! iso
    MRQ_ISOLATOROUTPUT_STATE isoCH;

    isoCH = (MRQ_ISOLATOROUTPUT_STATE)ui->cmbIsoCH->currentIndex();

    checked_call( pDevice->setISOLATOROUTPUT_STATE( isoCH,
                                      (MRQ_DIGITALOUTPUT_STATE_1)ui->chkIsoCH->isEnabled() ) );
    checked_call( pDevice->setISOLATOROUTPUT_SOURCE( isoCH,
                                       ui->cmbIsoSource->currentIndex() ) );
    checked_call( pDevice->setISOLATOROUTPUT_CONDITION( isoCH,
                                          (MRQ_DIGITALOUTPUT_CONDITION_1)ui->cmbIsoCondition->currentIndex() ) );
    checked_call( pDevice->setISOLATOROUTPUT_RESPONSE( isoCH,
                                         (MRQ_ISOLATOROUTPUT_RESPONSE_1)ui->cmbIsoAction->currentIndex() ) );

    return ret;
}
int mrqIo::doApply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    int ret;

    //! do
    MRQ_DIGITALOUTPUT_STATE doCH;
    doCH = (MRQ_DIGITALOUTPUT_STATE)ui->cmbDo->currentIndex();

    checked_call( pDevice->setDIGITALOUTPUT_STATE( doCH,
                                  (MRQ_DIGITALOUTPUT_STATE_1)ui->chkDo->isEnabled() ) );
    checked_call( pDevice->setDIGITALOUTPUT_POLARITY( doCH,
                                     (MRQ_DIGITALOUTPUT_POLARITY_1)ui->cmbDoPolarity->currentIndex() ) );
    checked_call( pDevice->setDIGITALOUTPUT_PERIOD( doCH, comAssist::align( ui->spinDoPeri->value(), peri_unit) ) );
    checked_call( pDevice->setDIGITALOUTPUT_DUTY( doCH, comAssist::align( ui->spinDoDuty->value(), duty_unit) ) );

    checked_call( pDevice->setDIGITALOUTPUT_SOURCE( doCH,
                                                    ui->cmbDoSource->currentIndex() ) );
    checked_call( pDevice->setDIGITALOUTPUT_CONDITION( doCH,
                                      (MRQ_DIGITALOUTPUT_CONDITION_1)ui->cmbDoCondition->currentIndex() ) );
    checked_call( pDevice->setDIGITALOUTPUT_SIGNAL( doCH,
                                   (MRQ_DIGITALOUTPUT_SIGNAL_1)ui->cmbDoSignal->currentIndex() ) );

    return ret;
}
int mrqIo::aiApply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    int ret = 0;

    //! ai
//    checked_call( pDevice->setANALOGIN_STATE( (MRQ_SYSTEM_REPORTSWITCH)ui->chkAi->isEnabled() ) );
//    checked_call( pDevice->setANALOGIN_THRESHOLDH( ui->spinHighLevel->value() ) );
//    checked_call( pDevice->setANALOGIN_THRESHOLDL( ui->spinLowLevel->value() ) );

//    checked_call( pDevice->setANALOGIN_RESPONSEH( (MRQ_OUTOFSTEP_LINERESPONSE)ui->cmbHighResp->currentIndex() ) );
//    checked_call( pDevice->setANALOGIN_RESPONSEL( (MRQ_OUTOFSTEP_LINERESPONSE)ui->cmbLowResp->currentIndex() ) );

    return ret;
}


void mrqIo::on_btnIsoApply_clicked()
{
    int ret;

    ret = isoApply();

    if ( ret != 0 )
    { sysError( tr("Iso apply fail"));}
    else
    { sysLog( tr("Iso apply success") ) ;}
}

void mrqIo::on_btnDoApply_clicked()
{
    int ret;

    ret = doApply();

    if ( ret != 0 )
    { sysError(tr("Do apply fail"));}
    else
    { sysLog(tr("Do apply success"));}
}

void mrqIo::on_btnAiApply_clicked()
{
    int ret;

    ret = isoApply();

    if ( ret != 0 )
    { sysError(tr("Ai apply fail"));}
    else
    { sysLog(tr("Ai apply success"));}
}
