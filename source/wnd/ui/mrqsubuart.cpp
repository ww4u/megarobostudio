#include "mrqsubuart.h"
#include "ui_mrqsubuart.h"

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

//! dir = true :: to screen
#define exchange_check( control, val, dir )  \
                                        if ( dir )\
                                        { ui->control->setChecked( val ); } \
                                        else \
                                        { val = ui->control->isChecked(); }


#define exchange_spin( control, val, dir )  \
                                        if ( dir )\
                                        { ui->control->setValue( val ); } \
                                        else \
                                        { val = ui->control->value(); }

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
