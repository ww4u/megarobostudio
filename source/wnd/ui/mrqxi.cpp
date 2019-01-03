#include "mrqxi.h"
#include "ui_mrqxi.h"

MrqXI::MrqXI(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqXI)
{
    ui->setupUi(this);

    setupUi();

    spyEdited();
}

MrqXI::~MrqXI()
{
    delete ui;
}

void MrqXI::changeEvent(QEvent * event)
{
    mrqView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void MrqXI::setConfig( XiConfig &cfg )
{
    ui->chkIsiEn->setChecked( cfg.mbEn );

    ui->cmbIsiType->setCurrentIndex( cfg.mTypeIndex );
    ui->cmbIsiCh->setCurrentIndex( cfg.mRespCH );
    ui->cmbIsiResp->setCurrentIndex( cfg.mRespIndex );

    ui->spinIsiPeriod->setValue( cfg.mPeri );
}

void MrqXI::getConfig( XiConfig &cfg )
{
    cfg.mbEn = ui->chkIsiEn->isChecked();

    cfg.mTypeIndex = ui->cmbIsiType->currentIndex();
    cfg.mRespCH = ui->cmbIsiCh->currentIndex();
    cfg.mRespIndex = ui->cmbIsiResp->currentIndex();

    cfg.mPeri = ui->spinIsiPeriod->value();
}

void MrqXI::setupUi()
{}
void MrqXI::spyEdited()
{
    QGroupBox *gpBox[]=
    {
    };
    QCheckBox *checkBoxes[]=
    {
        ui->chkIsiEn,
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinIsiPeriod
    };

    QComboBox *comboxes[]={
        ui->cmbIsiType,
        ui->cmbIsiCh,
        ui->cmbIsiResp,
    };

    install_spy();
}
