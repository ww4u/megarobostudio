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

void MrqTrigChan::changeEvent(QEvent * event)
{
    modelView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void MrqTrigChan::setConfig(
                trigLevelConfig &levConfig,
                trigPatternConfig &pattConifg
                )
{
    exchange_check( chkLevel, levConfig.mbOnOff, true );

    exchange_combox( cmbLevelMode, levConfig.mTypeIndex, true );
    exchange_combox( cmbLevelResponse, levConfig.mRespIndex, true );
    on_cmbLevelResponse_currentIndexChanged( levConfig.mRespIndex );

    exchange_spin( spinLevelPeriod, levConfig.mPeriod, true );

    ui->widget->setPage( levConfig.mRunIndex );
}
void MrqTrigChan::getConfig(
                trigLevelConfig &levConfig,
                trigPatternConfig &pattConifg )
{
    exchange_check( chkLevel, levConfig.mbOnOff, false );

    exchange_combox( cmbLevelMode, levConfig.mTypeIndex, false );
    exchange_combox( cmbLevelResponse, levConfig.mRespIndex, false );

    exchange_spin( spinLevelPeriod, levConfig.mPeriod, false );

    levConfig.mRunIndex = ui->widget->page();
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

void MrqTrigChan::on_cmbLevelResponse_currentIndexChanged(int index)
{
    //! the last
    if ( index == ui->cmbLevelResponse->count() - 1 )
    { ui->widget->setVisible( true ); }
    else
    { ui->widget->setVisible( false ); }
}
