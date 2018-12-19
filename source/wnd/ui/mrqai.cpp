#include "mrqai.h"
#include "ui_mrqai.h"

MrqAi::MrqAi(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqAi)
{
    ui->setupUi(this);

    setupUi();

    spyEdited();
}

MrqAi::~MrqAi()
{
    delete ui;
}

void MrqAi::changeEvent(QEvent * event)
{
    mrqView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void MrqAi::setConfig( AiConfig &cfg )
{
    ui->checkBox->setChecked( cfg.mbEn );

    ui->spinThreH->setValue( cfg.mHT );
    ui->spinThreL->setValue( cfg.mLT );

    ui->cmbRespH->setCurrentIndex( cfg.mHResp );
    ui->cmbRespL->setCurrentIndex( cfg.mLResp );
}
void MrqAi::getConfig( AiConfig &cfg )
{
    cfg.mbEn = ui->checkBox->isChecked();

    cfg.mHT = ui->spinThreH->value();
    cfg.mLT = ui->spinThreL->value();

    cfg.mHResp = ui->cmbRespH->currentIndex();
    cfg.mLResp = ui->cmbRespL->currentIndex();
}

void MrqAi::setupUi()
{}
void MrqAi::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->checkBox,
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinThreL,
        ui->spinThreH,
    };

    QComboBox *comboxes[]={
        ui->cmbRespH,
        ui->cmbRespL,
    };

    install_spy();
}
