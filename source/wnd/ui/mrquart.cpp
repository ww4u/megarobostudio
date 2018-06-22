#include "mrquart.h"
#include "ui_mrquart.h"

MrqUart::MrqUart(QWidget *parent) :
    modelView(parent),
    ui(new Ui::MrqUart)
{
    ui->setupUi(this);

    spyEdited();
}

MrqUart::~MrqUart()
{
    delete ui;
}

//! dir = true :: to screen
#define exchange_index( control, val, dir )  \
                                        if ( dir )\
                                        { ui->control->setCurrentIndex( val ); } \
                                        else \
                                        { val = ui->control->currentIndex(); }
void MrqUart::setConfig( uartConfig &cfg )
{
    exchange_index( cmbBaud, cfg.mBaudInd, true );
    exchange_index( cmbDataLen, cfg.mDataInd, true );
    exchange_index( cmbParity, cfg.mParityInd, true );
    exchange_index( cmbStop, cfg.mStopInd, true );

    exchange_index( cmbFlow, cfg.mFlowInd, true );
}

void MrqUart::getConfig( uartConfig &cfg )
{
    exchange_index( cmbBaud, cfg.mBaudInd, false );
    exchange_index( cmbDataLen, cfg.mDataInd, false );
    exchange_index( cmbParity, cfg.mParityInd, false );
    exchange_index( cmbStop, cfg.mStopInd, false );

    exchange_index( cmbFlow, cfg.mFlowInd, false );
}

void MrqUart::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={
    };
    QDoubleSpinBox *doubleSpinBoxes[]={
    };

    QComboBox *comboxes[]={
        ui->cmbBaud,
        ui->cmbDataLen,
        ui->cmbParity,
        ui->cmbStop,

        ui->cmbFlow,
    };

    install_spy();
}
