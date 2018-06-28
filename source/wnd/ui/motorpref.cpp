#include "motorpref.h"
#include "ui_motorpref.h"

#include "megamessagebox.h"

MotorPref::MotorPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::MotorPref)
{
    ui->setupUi(this);

    connect( this, SIGNAL(signal_joint_zero(int, bool)),
             this, SLOT(slot_joint_zero(int,bool)) );

    spyEdited();
}

MotorPref::~MotorPref()
{
    delete ui;
}

void MotorPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    adaptUi();

    updateUi();
}

int MotorPref::setApply()
{
    updateData();

    return 0;
}
void MotorPref::updateScreen()
{
    updateUi();
}
void MotorPref::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkGapPos,
        ui->chkZeroPos,
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };

    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinZeroTime,
        ui->spinZeroAngle,
//        ui->spinZeroSpeed,

        ui->spinGapTime,
        ui->spinGapDist,
    };

    QComboBox *comboxes[]={
        ui->cmbPosIndex
    };

    install_spy();
}

void MotorPref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotMotor *pRobo = (robotMotor*)pBase;
    Q_ASSERT( NULL != pRobo );
    pRobo->setZeroAttr( ui->spinZeroTime->value(),
                        ui->spinZeroAngle->value(),
                        ui->cmbPosIndex->currentIndex(),
                        ui->chkZeroPos->isChecked() );

    pRobo->setGap( ui->spinGapTime->value(),
                   ui->spinGapDist->value(),
                   ui->chkGapPos->isChecked() );
}

void MotorPref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotMotor *pRobo = (robotMotor*)pBase;
    Q_ASSERT( NULL != pRobo );

    double time, dist;
    int posIndex;
    bool bPosEn;
    pRobo->zeroAttr( time, dist, posIndex, bPosEn );

    ui->spinZeroTime->setValue( time );
    ui->spinZeroAngle->setValue( dist );
    ui->cmbPosIndex->setCurrentIndex( posIndex );
    ui->chkZeroPos->setChecked( bPosEn );

    double gapTime, gapDistance;
    pRobo->gap( gapTime, gapDistance, bPosEn );
    ui->spinGapTime->setValue( gapTime );
    ui->spinGapDist->setValue( gapDistance );
    ui->chkGapPos->setChecked( bPosEn );

    ui->chkCcw->setChecked( pRobo->jointZeroCcwList().at(0) );
}

void MotorPref::adaptUi()
{

}

void MotorPref::zeroJoint( int jointId, bool bCcw )
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    if ( !pBase->checkLink() )
    {
        sysPrompt( tr("Invalid conection") );
        return;
    }

    pBase->goZero( tpvRegion(0,0), jointId, bCcw );
}

void MotorPref::slot_joint_zero( int jId, bool bCcw )
{
    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    { zeroJoint( jId, bCcw ); }
}

#define sig_joint( id, bccw )    emit signal_joint_zero( id, bccw );

void MotorPref::on_btnZeroBody_clicked()
{
    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {}
    else
    { return; }

    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    //! body zero
    //! x + y
    QList<int> jList;
    QList<bool> ccwList;

    jList<<0;
    ccwList<<ui->chkCcw->isChecked();

    pBase->goZero( tpvRegion(0,0),
                   jList,
                   ccwList
                   );
}

void MotorPref::on_btnZeroMotor_clicked()
{
    sig_joint( 0, ui->chkCcw->isChecked() );
}
