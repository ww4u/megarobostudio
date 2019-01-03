#include "ipconfig.h"
#include "ui_ipconfig.h"

IPConfig::IPConfig(QWidget *parent) :
    modelView(parent),
    ui(new Ui::IPConfig)
{
    ui->setupUi(this);

    ui->spinDoorDist->setSuffix( char_deg );

    spyEdited();
}

IPConfig::~IPConfig()
{
    delete ui;
}

void IPConfig::changeEvent( QEvent *event )
{
    modelView::changeEvent( event );

    if ( event->type() == QEvent::LanguageChange )
    { ui->retranslateUi( this ); }
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
    QGroupBox *gpBox[]=
    {
    };
    QCheckBox *checkBoxes[]=
    {
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={
        ui->spinDoorCount,
    };

    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinDoorTime,
        ui->spinDoorDist,

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
