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
    int ret = apply();

    if ( ret != 0 )
    { sysError( tr("Sensor apply fail" )); }
    else
    { sysLog( tr("Sensor apply success") ); }

    return ret;
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

    int ret;
    uartConfig uCfg;
    subUartConfig suCfg;
    for ( int i = 0; i < 2; i++ )
    {
        ((MrqSensorPage*)ui->tabWidget->widget( i ))->getUartConfig( uCfg );

        sensPort = (MRQ_SENSORUART_BAUD)i;

        //! port
        checked_call( pDevice->setSENSORUART_BAUD( sensPort,
                                     (MRQ_SENSORUART_BAUD_1)uCfg.mBaudInd ) );

        checked_call( pDevice->setSENSORUART_WORDLEN( sensPort,
                                        (MRQ_RS232_WORDLEN)uCfg.mDataInd ) );

        checked_call( pDevice->setSENSORUART_PARITY( sensPort,
                                       (MRQ_RS232_PARITY)uCfg.mParityInd ) );
        checked_call( pDevice->setSENSORUART_STOPBIT( sensPort,
                                        (MRQ_RS232_STOPBIT)uCfg.mStopInd ) );
        checked_call( pDevice->setSENSORUART_FLOWCTL( sensPort,
                                        (MRQ_RS232_FLOWCTL)uCfg.mFlowInd ) );

        for ( int j = 0; j < 4; j++ )
        {
            ((MrqSensorPage*)ui->tabWidget->widget( i ))->getSubUartConfig( j, suCfg );

            subSens = (MRQ_IDENTITY_LABEL_1)j;

            //! sens
            checked_call( pDevice->setSENSORUART_SOF( sensPort, subSens, suCfg.mSof ) );
            checked_call( pDevice->setSENSORUART_FRAMELEN( sensPort, subSens, suCfg.mLength ) );
            checked_call( pDevice->setSENSORUART_RECEIVENUM( sensPort, subSens, suCfg.mReceiveNum ) );
            checked_call( pDevice->setSENSORUART_SWITCHTIME( sensPort, subSens,
                                                             comAssist::align( suCfg.mInterval, interval_unit_time) ) );

            checked_call( pDevice->setSENSORUART_STATE( sensPort,
                                                        subSens,
                                                        (MRQ_CAN_NETMANAGELED)suCfg.mbOnOff ) );

        }

        checked_call( pDevice->setSENSORUART_APPLYPARA( sensPort ) );
    }

    return ret;
}

int mrqSensor::updateUi()
{
    Q_ASSERT( m_pMrqModel != NULL );
    MegaDevice::deviceMRQ *pDevice;
    pDevice = m_pMrqModel;

    MegaDevice::MRQ_model *pModel = pDevice->getModel();

    uartConfig uCfg;
    subUartConfig suCfg;
    for ( int i = 0; i < 2; i++ )
    {
        //! uart
        uCfg.mBaudInd = pModel->mSENSORUART_BAUD[i];
        uCfg.mDataInd = pModel->mSENSORUART_WORDLEN[i];
        uCfg.mParityInd = pModel->mSENSORUART_PARITY[i];
        uCfg.mStopInd = pModel->mSENSORUART_STOPBIT[i];

        uCfg.mFlowInd = pModel->mSENSORUART_FLOWCTL[i];

        ((MrqSensorPage*)ui->tabWidget->widget( i ))->setUartConfig( uCfg );

        //! sub uart
        for ( int j = 0; j < 4; j++ )
        {
            suCfg.mbOnOff = pModel->mSENSORUART_STATE[i][j];
            suCfg.mSof = pModel->mSENSORUART_SOF[i][j];
            suCfg.mLength = pModel->mSENSORUART_FRAMELEN[i][j];
            suCfg.mReceiveNum = pModel->mSENSORUART_RECEIVENUM[i][j];

            suCfg.mInterval = pModel->mSENSORUART_SWITCHTIME[i][j]*interval_unit_time;

            ((MrqSensorPage*)ui->tabWidget->widget( i ))->setSubUartConfig( j, suCfg );
        }
    }

    return 0;
}

