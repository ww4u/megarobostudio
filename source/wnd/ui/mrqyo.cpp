#include "mrqyo.h"
#include "ui_mrqyo.h"

MrqYO::MrqYO(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqYO)
{
    ui->setupUi(this);

    setupUi();

    spyEdited();
}

MrqYO::~MrqYO()
{
    delete ui;
}

void MrqYO::setConfig( YoConfig &cfg )
{
    ui->chkIsoCH->setChecked( cfg.mbEn );
    ui->cmbIsoSource->setCurrentIndex( cfg.mSrcIndex );
    ui->cmbIsoCondition->setCurrentIndex( cfg.mCondIndex );
    ui->cmbIsoAction->setCurrentIndex( cfg.mRespIndex );
}
void MrqYO::getConfig( YoConfig &cfg )
{
    cfg.mbEn = ui->chkIsoCH->isChecked();
    cfg.mSrcIndex = ui->cmbIsoSource->currentIndex();
    cfg.mCondIndex = ui->cmbIsoCondition->currentIndex();
    cfg.mRespIndex = ui->cmbIsoAction->currentIndex();
}

void MrqYO::setupUi()
{
    //! hide
    QWidget *hideWidgets[]=
    {
        ui->label_3,
        ui->label_4,
        ui->cmbIsoSource,
        ui->cmbIsoCondition,
    };

    for ( int i = 0; i < sizeof_array(hideWidgets); i++ )
    { hideWidgets[i]->setVisible(false); }
}

void MrqYO::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkIsoCH,
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
        ui->cmbIsoSource,
        ui->cmbIsoCondition,
        ui->cmbIsoAction,
    };

    install_spy();
}
