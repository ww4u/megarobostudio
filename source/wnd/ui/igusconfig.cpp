#include "igusconfig.h"
#include "ui_igusconfig.h"

IgusConfig::IgusConfig(QWidget *parent) :
    modelView(parent),
    ui(new Ui::IgusConfig)
{
    ui->setupUi(this);
}

IgusConfig::~IgusConfig()
{
    delete ui;
}

void IgusConfig::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}
int IgusConfig::setApply()
{
    updateData();

    return 0;
}

void IgusConfig::updateScreen()
{
    updateUi();
}

void IgusConfig::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkInvert,
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };

    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinArmLen0,
        ui->spinArmLen1,
        ui->spinArmLen2,
        ui->spinArmLen3,

        ui->spinOffset0,
        ui->spinOffset1,

        ui->spinP00,
        ui->spinP01,
        ui->spinP02,

        ui->spinPosLim0,
        ui->spinPosLim1,

        ui->spinScal,
        ui->spinvM,

        ui->spinAngleToDist,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}
void IgusConfig::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotIgusDelta *pRobo = (robotIgusDelta*)pBase;
    Q_ASSERT( NULL != pRobo );

    pRobo->mArmLengths[0] = ui->spinArmLen0->value();
    pRobo->mArmLengths[1] = ui->spinArmLen1->value();
    pRobo->mArmLengths[2] = ui->spinArmLen2->value();
    pRobo->mArmLengths[3] = ui->spinArmLen3->value();

    pRobo->mOffset[0] = ui->spinOffset0->value();
    pRobo->mOffset[1] = ui->spinOffset1->value();

    pRobo->mP0[0] = ui->spinP00->value();
    pRobo->mP0[1] = ui->spinP01->value();
    pRobo->mP0[2] = ui->spinP02->value();

    pRobo->mPosLim[0] = ui->spinPosLim0->value();
    pRobo->mPosLim[1] = ui->spinPosLim1->value();

    pRobo->mScal = ui->spinScal->value();
    pRobo->mVm = ui->spinvM->value();

    pRobo->mInvert = ui->chkInvert->isChecked();
    pRobo->mAngleToDist = ui->spinAngleToDist->value();
}
void IgusConfig::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotIgusDelta *pRobo = (robotIgusDelta*)pBase;
    Q_ASSERT( NULL != pRobo );

    ui->spinArmLen0->setValue( pRobo->mArmLengths.at(0) );
    ui->spinArmLen1->setValue( pRobo->mArmLengths.at(1) );
    ui->spinArmLen2->setValue( pRobo->mArmLengths.at(2) );
    ui->spinArmLen3->setValue( pRobo->mArmLengths.at(3) );

    ui->spinOffset0->setValue( pRobo->mOffset.at(0));
    ui->spinOffset1->setValue( pRobo->mOffset.at(1));

    ui->spinP00->setValue( pRobo->mP0.at(0) );
    ui->spinP01->setValue( pRobo->mP0.at(1) );
    ui->spinP02->setValue( pRobo->mP0.at(2) );

    ui->spinPosLim0->setValue( pRobo->mPosLim.at(0) );
    ui->spinPosLim1->setValue( pRobo->mPosLim.at(1) );

    ui->spinScal->setValue( pRobo->mScal );
    ui->spinvM->setValue( pRobo->mVm );

    ui->chkInvert->setChecked( pRobo->mInvert );
    ui->spinAngleToDist->setValue( pRobo->mAngleToDist );
}
