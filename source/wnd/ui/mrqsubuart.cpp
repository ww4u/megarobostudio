#include "mrqsubuart.h"
#include "ui_mrqsubuart.h"

MrqSubUart::MrqSubUart(QWidget *parent) :
    modelView(parent),
    ui(new Ui::MrqSubUart)
{
    ui->setupUi(this);

    spyEdited();
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

void MrqSubUart::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkSensor,
    };

    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={
        ui->spinSof,
        ui->spinBox,
        ui->spinFrameLen,
        ui->spinReceiveLen,

    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinInvterval,

    };

    QComboBox *comboxes[]={

    };

    install_spy();
}
