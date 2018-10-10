#include "mrvswitch.h"
#include "ui_mrvswitch.h"

#include "../../com/comassist.h"

#define delay_unit  (0.001f)

MrvSwitch::MrvSwitch(QWidget *parent) :
    modelView(parent),
    ui(new Ui::MrvSwitch)
{
    ui->setupUi(this);
}

MrvSwitch::~MrvSwitch()
{
    delete ui;
}

void MrvSwitch::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={
        ui->spinFilterCounter,
    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinFilterDelay
    };

    QComboBox *comboxes[]={
        ui->cmbTriggerEdge,

    };

    install_spy();
}

void MrvSwitch::setConfig( switchConfig &cfg )
{
    ui->spinFilterDelay->setValue( cfg.mFilterDelay * delay_unit );
    ui->spinFilterCounter->setValue( cfg.mFilterCount );
    ui->cmbTriggerEdge->setCurrentIndex( cfg.mTrigEdge );
}
void MrvSwitch::getConifig( switchConfig &cfg )
{
    cfg.mFilterDelay = comAssist::align( ui->spinFilterDelay->value(), delay_unit );
    cfg.mFilterCount = ui->spinFilterCounter->value();
    cfg.mTrigEdge = ui->cmbTriggerEdge->currentIndex();
}
