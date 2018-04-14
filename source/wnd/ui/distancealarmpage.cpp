#include "distancealarmpage.h"
#include "ui_distancealarmpage.h"


DistanceAlarmPage::DistanceAlarmPage(QWidget *parent) :
    modelView(parent),
    ui(new Ui::DistanceAlarmPage)
{
    ui->setupUi(this);

    spyEdited();
}

DistanceAlarmPage::~DistanceAlarmPage()
{
    delete ui;
}

void DistanceAlarmPage::setData( DistAlarmConfig &config )
{
    ui->checkBox->setChecked( config.mbEn );
    ui->spinBox->setValue( config.mDist[0] );
    ui->spinBox_2->setValue( config.mDist[1] );
    ui->spinBox_3->setValue( config.mDist[2] );
}
void DistanceAlarmPage::data( DistAlarmConfig &config )
{
    config.mbEn = ui->checkBox->isChecked();
    config.mDist[0] = ui->spinBox->value();
    config.mDist[1] = ui->spinBox_2->value();
    config.mDist[2] = ui->spinBox_3->value();
}

void DistanceAlarmPage::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->checkBox,
    };

    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };
    QSpinBox *doubleSpinBoxes[]={
        ui->spinBox,
        ui->spinBox_2,
        ui->spinBox_3,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}
