#include "mrqsensor.h"
#include "ui_mrqsensor.h"

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
    MRQ_SENSORUART_STATE_1 subSens;

    sensPort = (MRQ_SENSORUART_BAUD)ui->cmbPort->currentIndex();
    subSens = (MRQ_SENSORUART_STATE_1)ui->cmbSensor->currentIndex();

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
    checked_call( pDevice->setSENSORUART_SWITCHTIME( sensPort, subSens, ui->spinInvterval->value() / interval_unit_time ) );

    checked_call( pDevice->setSENSORUART_APPLYPARA( sensPort ) );

    return ret;
}

int mrqSensor::updateUi()
{ return 0; }

void mrqSensor::on_btnApply_clicked()
{
    int ret;

    ret = apply();

    if ( ret != 0 )
    { sysError( "Sensor apply fail" ); }
    else
    { sysLog( "Sensor apply success" ); }
}
