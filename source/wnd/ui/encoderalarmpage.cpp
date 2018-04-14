#include "encoderalarmpage.h"
#include "ui_encoderalarmpage.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../com/comassist.h"
EncoderAlarmPage::EncoderAlarmPage(QWidget *parent) :
    modelView(parent),
    ui(new Ui::EncoderAlarmPage)
{
    ui->setupUi(this);

    spyEdited();
}

EncoderAlarmPage::~EncoderAlarmPage()
{
    delete ui;
}

#define angle_error     (1e-6)
//#define align_angle_value( angle )    \
                                        ( comAssist::align( \
                                         MegaDevice::deviceMRQ::absAngleToValue( angle),\
                                         angle_error)*angle_error )

//#define align_value_angle( value )     ( comAssist::align ( \
                                        MegaDevice::deviceMRQ::valueToAbsAngle( value ), \
                                        angle_error)*angle_error )

#define align_angle_value( angle )    \
                                        ( ( \
                                         MegaDevice::deviceMRQ::absAngleToValue( angle)\
                                         ) )

#define align_value_angle( value )     ( ( \
                                        MegaDevice::deviceMRQ::valueToAbsAngle( value ) \
                                        ) )

void EncoderAlarmPage::setData( EncoderAlarmConfig &config )
{
    ui->checkBox->setChecked( config.mbEn );
    ui->doubleSpinBox->setValue( align_angle_value(config.mUpLimit) );
    ui->doubleSpinBox_2->setValue( align_angle_value( config.mDownLimit) );
    ui->doubleSpinBox_3->setValue( align_angle_value( config.mZero) );
    ui->comboBox->setCurrentIndex( config.mZeroPos );
}
void EncoderAlarmPage::data( EncoderAlarmConfig &config )
{
    config.mbEn = ui->checkBox->isChecked();
    config.mUpLimit =  align_value_angle( ui->doubleSpinBox->value() );
    config.mDownLimit =  align_value_angle( ui->doubleSpinBox_2->value() );

    config.mZero =  align_value_angle( ui->doubleSpinBox_3->value() );
    config.mZeroPos = ui->comboBox->currentIndex();
}

void EncoderAlarmPage::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->checkBox,
    };

    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->doubleSpinBox,
        ui->doubleSpinBox_2,
        ui->doubleSpinBox_3,
    };

    QComboBox *comboxes[]={
        ui->comboBox,
    };

    install_spy();
}
