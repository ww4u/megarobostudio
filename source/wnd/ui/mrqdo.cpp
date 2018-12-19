#include "mrqdo.h"
#include "ui_mrqdo.h"

MrqDO::MrqDO(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqDO)
{
    ui->setupUi(this);

    setupUi();

    spyEdited();
}

MrqDO::~MrqDO()
{
    delete ui;
}

void MrqDO::changeEvent(QEvent * event)
{
    mrqView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void MrqDO::setConfig( DoConfig &cfg )
{
    ui->chkDo->setChecked( cfg.mbEn );

    ui->cmbDoSource->setCurrentIndex( cfg.mSrcIndex );
    ui->cmbDoSignal->setCurrentIndex( cfg.mSignalIndex );
    ui->cmbDoCondition->setCurrentIndex( cfg.mCondIndex );
    ui->cmbDoPolarity->setCurrentIndex( cfg.mPolarityIndex );

    ui->spinDoPeri->setValue( cfg.mPeri );
    ui->spinDoDuty->setValue( cfg.mDuty );
}

void MrqDO::getConfig( DoConfig &cfg )
{
    cfg.mbEn = ui->chkDo->isChecked();

    cfg.mSrcIndex = ui->cmbDoSource->currentIndex();
    cfg.mSignalIndex = ui->cmbDoSignal->currentIndex();
    cfg.mCondIndex = ui->cmbDoCondition->currentIndex();
    cfg.mPolarityIndex = ui->cmbDoPolarity->currentIndex();

    cfg.mPeri = ui->spinDoPeri->value();
    cfg.mDuty = ui->spinDoDuty->value();
}

void MrqDO::setupUi()
{
    //! hide
    QWidget *hideWidgets[]=
    {
        ui->label_15,
        ui->label_12,
        ui->label_2,
        ui->label_14,

        ui->cmbDoSource,
        ui->cmbDoCondition,
        ui->spinDoPeri,
        ui->spinDoDuty,
    };

    for ( int i = 0; i < sizeof_array(hideWidgets); i++ )
    { hideWidgets[i]->setVisible(false); }
}

void MrqDO::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkDo,
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={


    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinDoPeri,
        ui->spinDoDuty,
    };

    QComboBox *comboxes[]={
        ui->cmbDoSource,
        ui->cmbDoSignal,
        ui->cmbDoCondition,
        ui->cmbDoPolarity,
    };

    install_spy();
}
