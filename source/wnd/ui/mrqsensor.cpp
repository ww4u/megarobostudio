#include "mrqsensor.h"
#include "ui_mrqsensor.h"
#include "../../com/comassist.h"

//! \todo interval time
#define interval_unit_time  (0.001f)

mrqSensor::mrqSensor(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqSensor)
{
    ui->setupUi(this);

    setupUi();
}

mrqSensor::~mrqSensor()
{
    delete ui;

    desetupUi();
}

int mrqSensor::setApply()
{
//    return apply();
    return 0;
}

void mrqSensor::modelChanged()
{ updateUi(); }

void mrqSensor::setupUi()
{}
void mrqSensor::desetupUi()
{}

int mrqSensor::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    MRQ_SENSORUART_BAUD sensPort;
    MRQ_IDENTITY_LABEL_1 subSens;

    sensPort = (MRQ_SENSORUART_BAUD)ui->cmbPort->currentIndex();
    subSens = (MRQ_IDENTITY_LABEL_1)ui->cmbSensor->currentIndex();

    int ret;
    //! port
    checked_call( pDevice->setSENSORUART_BAUD( sensPort,
                                 (MRQ_RS232_BAUD)ui->cmbBaud->currentIndex() ) );

    checked_call( pDevice->setSENSORUART_WORDLEN( sensPort,
                                    (MRQ_RS232_WORDLEN)ui->cmbDataLen->currentIndex() ) );

    checked_call( pDevice->setSENSORUART_PARITY( sensPort,
                                   (MRQ_RS232_PARITY)ui->cmbParity->currentIndex() ) );
    checked_call( pDevice->setSENSORUART_STOPBIT( sensPort,
                                    (MRQ_RS232_STOPBIT)ui->cmbStop->currentIndex() ) );
    checked_call( pDevice->setSENSORUART_FLOWCTL( sensPort,
                                    (MRQ_RS232_FLOWCTL)ui->cmbFlow->currentIndex() ) );

    //! sens
    checked_call( pDevice->setSENSORUART_SOF( sensPort, subSens, ui->spinSof->value() ) );
    checked_call( pDevice->setSENSORUART_FRAMELEN( sensPort, subSens, ui->spinFrameLen->value() ) );
    checked_call( pDevice->setSENSORUART_RECEIVENUM( sensPort, subSens, ui->spinReceiveLen->value() ) );
    checked_call( pDevice->setSENSORUART_SWITCHTIME( sensPort, subSens,
                                                     comAssist::align( ui->spinInvterval->value(), interval_unit_time) ) );

    checked_call( pDevice->setSENSORUART_STATE( sensPort,
                                                subSens,
                                                (MRQ_SYSTEM_DIOREFREAD)ui->chkSensor->isChecked() ) );

    checked_call( pDevice->setSENSORUART_APPLYPARA( sensPort ) );

    return ret;
}

int mrqSensor::updateUi()
{
    //! update by uart && s
    int uartId, sId;

    uartId = ui->cmbPort->currentIndex();
    sId = ui->cmbSensor->currentIndex();

    //! get device
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    Q_ASSERT( uartId < sizeof_array(pDevice->mSENSORUART_BAUD) );
    Q_ASSERT( sId < sizeof_array(pDevice->mSENSORUART_STATE[0]) );

    MegaDevice::MRQ_model *pModel = pDevice->getModel();

    ui->cmbBaud->setCurrentIndex(  pModel->mSENSORUART_BAUD[uartId]  );
    ui->cmbDataLen->setCurrentIndex( pModel->mSENSORUART_WORDLEN[uartId] );
    ui->cmbParity->setCurrentIndex( pModel->mSENSORUART_PARITY[uartId] );
    ui->cmbStop->setCurrentIndex( pModel->mSENSORUART_STOPBIT[uartId] );
    ui->cmbFlow->setCurrentIndex( pModel->mSENSORUART_FLOWCTL[uartId] );

    ui->chkSensor->setChecked( pModel->mSENSORUART_STATE[uartId][sId] );
    ui->spinSof->setValue( pModel->mSENSORUART_SOF[uartId][sId] );
    ui->spinFrameLen->setValue( pModel->mSENSORUART_FRAMELEN[uartId][sId] );
    ui->spinReceiveLen->setValue( pModel->mSENSORUART_RECEIVENUM[uartId][sId] );
    ui->spinInvterval->setValue( pModel->mSENSORUART_SWITCHTIME[uartId][sId]*interval_unit_time );

    return 0;
}

void mrqSensor::on_btnApply_clicked()
{
    int ret;

    ret = apply();

    if ( ret != 0 )
    { sysError( tr("Sensor apply fail" )); }
    else
    { sysLog( tr("Sensor apply success") ); }
}

void mrqSensor::on_cmbPort_currentIndexChanged(int index)
{
    updateUi();
}

void mrqSensor::on_cmbSensor_currentIndexChanged(int index)
{
    updateUi();
}
