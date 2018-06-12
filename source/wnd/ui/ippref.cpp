#include "ippref.h"
#include "ui_ippref.h"

IPPref::IPPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::IPPref)
{
    ui->setupUi(this);

    spyEdited();
}

IPPref::~IPPref()
{
    delete ui;
}

void IPPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    adaptUi();

    updateUi();
}
int IPPref::setApply()
{
    updateData();

    return 0;
}

void IPPref::updateScreen()
{
    updateUi();
}

void IPPref::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
    };

    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };

    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinZeroTime,
        ui->spinZeroAngle,
//        ui->spinZeroSpeed,

        ui->spinVGapTime,
        ui->spinVGapDist,

        ui->spinIGapTime,
        ui->spinIGapDist,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}
void IPPref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotInject *pRobo = (robotInject*)pBase;
    Q_ASSERT( NULL != pRobo );
    pRobo->setZeroAttr( ui->spinZeroTime->value(),
                        ui->spinZeroAngle->value() );

    pRobo->setGap( ui->spinVGapTime->value(),
                   ui->spinVGapDist->value(),
                   ui->spinIGapTime->value(),
                   ui->spinIGapDist->value() );
}
void IPPref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotInject *pRobo = (robotInject*)pBase;
    Q_ASSERT( NULL != pRobo );

    double zeroTime, zeroAngle;
    pRobo->zeroAttr( zeroTime, zeroAngle );
    ui->spinZeroTime->setValue( zeroTime );
    ui->spinZeroAngle->setValue( zeroAngle );

    double vGapT, vGapD, iGapT, iGapD;
    pRobo->gap( vGapT, vGapD,
                iGapT, iGapD);
    ui->spinVGapTime->setValue( vGapT );
    ui->spinVGapDist->setValue( vGapD );

    ui->spinIGapTime->setValue( iGapT );
    ui->spinIGapDist->setValue( iGapD );
}

void IPPref::adaptUi()
{}

void IPPref::zeroJoint( int jointId, bool bCcw )
{}

void IPPref::on_btnZeroX_clicked()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotInject *pRobo = (robotInject*)pBase;
    Q_ASSERT( NULL != pRobo );

    if ( pRobo->checkLink() )
    {}
    else
    { return; }

    pRobo->goZero( tpvRegion(0,0), 0, ui->chkCcwValve->isChecked() );
}

void IPPref::on_btnZeroY_clicked()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotInject *pRobo = (robotInject*)pBase;
    Q_ASSERT( NULL != pRobo );

    if ( pRobo->checkLink() )
    {}
    else
    { return; }

    //! joint 1
    pRobo->goZero( tpvRegion(0,0), 1, ui->chkCcwInj->isChecked() );
}

void IPPref::on_btnZeroBody_clicked()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotInject *pRobo = (robotInject*)pBase;
    Q_ASSERT( NULL != pRobo );

    if ( pRobo->checkLink() )
    {}
    else
    { return; }

    pRobo->goZero( tpvRegion(0,0) );
}
