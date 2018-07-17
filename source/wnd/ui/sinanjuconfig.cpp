#include "sinanjuconfig.h"
#include "ui_sinanjuconfig.h"

SinanjuConfig::SinanjuConfig(QWidget *parent) :
    modelView(parent),
    ui(new Ui::SinanjuConfig)
{
    ui->setupUi(this);

    spyEdited();
}

SinanjuConfig::~SinanjuConfig()
{
    delete ui;
}

void SinanjuConfig::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj(pObj);

    updateUi();
}

int SinanjuConfig::setApply()
{
    updateData();

    return 0;
}

void SinanjuConfig::updateScreen()
{
    updateUi();
}

void SinanjuConfig::spyEdited()
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
        ui->spinBaseLen,
        ui->spinBigLen,
        ui->spinLittleLen,

        ui->spinX,
        ui->spinY,
        ui->spinZ,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}

void SinanjuConfig::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    robotSinanju *pRobo = ( robotSinanju *)m_pModelObj;
    Q_ASSERT( NULL != pRobo );

    pRobo->mArmLengths[0] = ui->spinBaseLen->value();
    pRobo->mArmLengths[1] = ui->spinBigLen->value();
    pRobo->mArmLengths[2] = ui->spinLittleLen->value();

    pRobo->mInitPos[0] = ui->spinX->value();
    pRobo->mInitPos[1] = ui->spinY->value();
    pRobo->mInitPos[2] = ui->spinZ->value();

    pRobo->setJointFactoryAngle( 0, ui->angleFactBase->value() );
    pRobo->setJointFactoryAngle( 1, ui->angleFactBa->value() );
    pRobo->setJointFactoryAngle( 2, ui->angleFactLa->value() );
    pRobo->setJointFactoryAngle( 3, ui->angleFactWr->value() );
}
void SinanjuConfig::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    robotSinanju *pRobo = ( robotSinanju *)m_pModelObj;
    Q_ASSERT( NULL != pRobo );

    Q_ASSERT( pRobo->mArmLengths.size() >= 3 );

    ui->spinBaseLen->setValue( pRobo->mArmLengths.at(0) );
    ui->spinBigLen->setValue( pRobo->mArmLengths.at(1) );
    ui->spinLittleLen->setValue( pRobo->mArmLengths.at(2) );

    ui->spinX->setValue( pRobo->mInitPos.at(0) );
    ui->spinY->setValue( pRobo->mInitPos.at(1) );
    ui->spinZ->setValue( pRobo->mInitPos.at(2) );

    ui->angleFactBase->setValue( pRobo->jointFactoryAngle(0) );
    ui->angleFactBa->setValue( pRobo->jointFactoryAngle(1) );
    ui->angleFactLa->setValue( pRobo->jointFactoryAngle(2) );
    ui->angleFactWr->setValue( pRobo->jointFactoryAngle(3) );
}
