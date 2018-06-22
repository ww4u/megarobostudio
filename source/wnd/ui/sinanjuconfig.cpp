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
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    pBase->mArmLengths[0] = ui->spinBaseLen->value();
    pBase->mArmLengths[1] = ui->spinBigLen->value();
    pBase->mArmLengths[2] = ui->spinLittleLen->value();

    pBase->mInitPos[0] = ui->spinX->value();
    pBase->mInitPos[1] = ui->spinY->value();
    pBase->mInitPos[2] = ui->spinZ->value();

}
void SinanjuConfig::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    Q_ASSERT( pBase->mArmLengths.size() >= 3 );

    ui->spinBaseLen->setValue( pBase->mArmLengths.at(0) );
    ui->spinBigLen->setValue( pBase->mArmLengths.at(1) );
    ui->spinLittleLen->setValue( pBase->mArmLengths.at(2) );

    ui->spinX->setValue( pBase->mInitPos.at(0) );
    ui->spinY->setValue( pBase->mInitPos.at(1) );
    ui->spinZ->setValue( pBase->mInitPos.at(2) );
}
