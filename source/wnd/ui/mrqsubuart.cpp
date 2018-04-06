#include "mrqsubuart.h"
#include "ui_mrqsubuart.h"
#include "modelview.h"

MrqSubUart::MrqSubUart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MrqSubUart)
{
    ui->setupUi(this);
}

MrqSubUart::~MrqSubUart()
{
    delete ui;
}

void MrqSubUart::setConfig(
             subUartConfig &cfg )
{
    exchange_check( chkSensor, cfg.mbOnOff, true )
    exchange_spin( spinSof, cfg.mSof, true )
    exchange_spin( spinFrameLen, cfg.mLength, true )
    exchange_spin( spinReceiveLen, cfg.mReceiveNum, true )

    exchange_spin( spinInvterval, cfg.mInterval, true )
}

void MrqSubUart::getConfig( subUartConfig &cfg )
{
    exchange_check( chkSensor, cfg.mbOnOff, false )
    exchange_spin( spinSof, cfg.mSof, false )
    exchange_spin( spinFrameLen, cfg.mLength, false )
    exchange_spin( spinReceiveLen, cfg.mReceiveNum, false )

    exchange_spin( spinInvterval, cfg.mInterval, false )
}
