#include "mrqtrigchan.h"
#include "ui_mrqtrigchan.h"
#include "modelview.h"
MrqTrigChan::MrqTrigChan(QWidget *parent) :
    modelView(parent),
    ui(new Ui::MrqTrigChan)
{
    ui->setupUi(this);

    spyEdited();
}

MrqTrigChan::~MrqTrigChan()
{
    delete ui;
}

void MrqTrigChan::setConfig(
                trigLevelConfig &levConfig,
                trigPatternConfig &pattConifg
                )
{
    exchange_check( chkLevel, levConfig.mbOnOff, true );

    exchange_combox( cmbLevelMode, levConfig.mTypeIndex, true );
    exchange_combox( cmbLevelResponse, levConfig.mRespIndex, true );

    exchange_spin( spinLevelPeriod, levConfig.mPeriod, true );
}
void MrqTrigChan::getConfig(
                trigLevelConfig &levConfig,
                trigPatternConfig &pattConifg )
{
    exchange_check( chkLevel, levConfig.mbOnOff, false );

    exchange_combox( cmbLevelMode, levConfig.mTypeIndex, false );
    exchange_combox( cmbLevelResponse, levConfig.mRespIndex, false );

    exchange_spin( spinLevelPeriod, levConfig.mPeriod, false );
}

void MrqTrigChan::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkLevel,
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={
    };

    QSpinBox *spinBoxes[]={
    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinLevelPeriod,
    };

    QComboBox *comboxes[]={
        ui->cmbLevelMode,
        ui->cmbLevelResponse,
    };

    install_spy();
}
