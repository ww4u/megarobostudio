#include "ipconfig.h"
#include "ui_ipconfig.h"

IPConfig::IPConfig(QWidget *parent) :
    modelView(parent),
    ui(new Ui::IPConfig)
{
    ui->setupUi(this);

    spyEdited();
}

IPConfig::~IPConfig()
{
    delete ui;
}

void IPConfig::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    adaptUi();

    updateUi();
}
int IPConfig::setApply()
{
    updateData();

    return 0;
}

void IPConfig::updateScreen()
{
    updateUi();
}

void IPConfig::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
    };

    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={
        ui->spinDoorCount,
    };

    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinDoorTime,
        ui->spinDoorDist,
//        ui->spinZeroSpeed,

        ui->spinDrinkTime,
        ui->spinDrinkDist,

        ui->spinSpitTime,
        ui->spinSpitDist,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}
void IPConfig::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotInject *pRobo = (robotInject*)pBase;
    Q_ASSERT( NULL != pRobo );
//    pRobo->setZeroAttr( ui->spinZeroTime->value(),
//                        ui->spinZeroAngle->value() );

//    pRobo->setGap( ui->spinVGapTime->value(),
//                   ui->spinVGapDist->value(),
//                   ui->spinIGapTime->value(),
//                   ui->spinIGapDist->value() );

    InjProp lProp;

    //! get pre
    pRobo->injAttr( lProp );

    lProp.mDoor.mTime = ui->spinDoorTime->value();
    lProp.mDoor.mDist = ui->spinDoorDist->value();

    lProp.mDrink.mTime = ui->spinDrinkTime->value();
    lProp.mDrink.mDist = ui->spinDrinkDist->value();

    lProp.mSpit.mTime = ui->spinSpitTime->value();
    lProp.mSpit.mDist = ui->spinSpitDist->value();

    pRobo->setInjAttr( lProp );
}

void IPConfig::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotInject *pRobo = (robotInject*)pBase;
    Q_ASSERT( NULL != pRobo );

    InjProp lProp;
    pRobo->injAttr( lProp );

    ui->spinDoorCount->setValue( lProp.mDoors );
    ui->spinDoorTime->setValue( lProp.mDoor.mTime );
    ui->spinDoorDist->setValue( lProp.mDoor.mDist );

    ui->spinDrinkTime->setValue( lProp.mDrink.mTime );
    ui->spinDrinkDist->setValue( lProp.mDrink.mDist );

    ui->spinSpitTime->setValue( lProp.mSpit.mTime );
    ui->spinSpitDist->setValue( lProp.mSpit.mDist );
}

void IPConfig::adaptUi()
{}
