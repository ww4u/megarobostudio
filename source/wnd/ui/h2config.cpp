#include "h2config.h"
#include "ui_h2config.h"

H2Config::H2Config(QWidget *parent) :
    modelView(parent),
    ui(new Ui::H2Config)
{
    ui->setupUi(this);
}

H2Config::~H2Config()
{
    delete ui;
}

void H2Config::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}

int H2Config::setApply()
{
    updateData();

    return 0;
}
void H2Config::updateScreen()
{
    updateUi();
}
void H2Config::spyEdited()
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
        ui->spinArm0,
        ui->spinArm1,
        ui->spinArm2,
        ui->spinArm3,
        ui->spinArm4,
        ui->spinArm5,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}

void H2Config::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotH2 *pRobo = (robotH2*)pBase;
    Q_ASSERT( NULL != pRobo );

    pRobo->mArmLengths[0] = ui->spinArm0->value();
    pRobo->mArmLengths[1] = ui->spinArm1->value();
    pRobo->mArmLengths[2] = ui->spinArm2->value();
    pRobo->mArmLengths[3] = ui->spinArm3->value();

    pRobo->mArmLengths[4] = ui->spinArm4->value();
    pRobo->mArmLengths[5] = ui->spinArm5->value();
}

void H2Config::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotH2 *pRobo = (robotH2*)pBase;
    Q_ASSERT( NULL != pRobo );

    ui->spinArm0->setValue( pRobo->mArmLengths[0] );
    ui->spinArm1->setValue( pRobo->mArmLengths[1] );
    ui->spinArm2->setValue( pRobo->mArmLengths[2] );
    ui->spinArm3->setValue( pRobo->mArmLengths[3] );

    ui->spinArm4->setValue( pRobo->mArmLengths[4] );
    ui->spinArm5->setValue( pRobo->mArmLengths[5] );
}
