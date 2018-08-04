#include "h2pref.h"
#include "ui_h2pref.h"

#include "megamessagebox.h"

H2Pref::H2Pref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::H2Pref)
{
    ui->setupUi(this);

    connect( this, SIGNAL(signal_joint_zero(int, bool)),
             this, SLOT(slot_joint_zero(int,bool)) );


    //! post change
//    slot_body_changed();
//    slot_ccw_changed();

    spyEdited();
}

H2Pref::~H2Pref()
{
    delete ui;
}

void H2Pref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    adaptUi();

    updateUi();
}

int H2Pref::setApply()
{
    updateData();

    return 0;
}
void H2Pref::updateScreen()
{
    updateUi();
}
void H2Pref::spyEdited()
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
        ui->spinZeroTime,
        ui->spinZeroAngle,
//        ui->spinZeroSpeed,

        ui->spinGapTime,
        ui->spinGapDist,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}

void H2Pref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotH2 *pRobo = (robotH2*)pBase;
    Q_ASSERT( NULL != pRobo );
    pRobo->setZeroAttr( ui->spinZeroTime->value(),
                        ui->spinZeroAngle->value() );

    pRobo->setGap( ui->spinGapTime->value(),
                   ui->spinGapDist->value(),
                   ui->spinGapZTime->value(),
                   ui->spinGapZDist->value() );
}

void H2Pref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotH2 *pRobo = (robotH2*)pBase;
    Q_ASSERT( NULL != pRobo );

    double time, dist;
    pRobo->zeroAttr( time, dist );

    ui->spinZeroTime->setValue( time );
    ui->spinZeroAngle->setValue( dist );

    double gapTime, gapDistance, gapZTime, gapZDistance;
    pRobo->gap( gapTime, gapDistance, gapZTime, gapZDistance );
    ui->spinGapTime->setValue( gapTime );
    ui->spinGapDist->setValue( gapDistance );

    ui->spinGapZTime->setValue( gapZTime );
    ui->spinGapZDist->setValue( gapZDistance );

//    ui->chkCcw->setChecked( pRobo->jointZeroCcwList().at(2) );
}

void H2Pref::adaptUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    bool bZ = false;
    QString strPixmapGeo;
    if ( pBase->getId() == VRobot::robot_h2
         || pBase->getId() == VRobot::robot_h2_m )
    {
        bZ = false;
        strPixmapGeo = ":/res/image/joint/mrx-h2_geo.png";
    }
    else if ( pBase->getId() == VRobot::robot_h2z )
    {
        bZ = true;
        strPixmapGeo = ":/res/image/joint/mrx-h2z_geo.png";
    }
    else
    {}

    //! z enable
    ui->labelZ->setVisible( bZ );
    ui->btnZeroZ->setVisible( bZ );
    ui->chkCcw->setVisible( bZ );
    ui->gpZGap->setVisible( bZ );

    //! ccw list
    if ( bZ )
    {
        ui->chkCcw->setChecked( pBase->jointZeroCcwList().at(2) );
    }

    ui->label_3->setPixmap( QPixmap(strPixmapGeo) );
}

void H2Pref::zeroJoint( int jointId, bool bCcw )
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

void H2Pref::slot_joint_zero( int jId, bool bCcw )
{
    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    { zeroJoint( jId, bCcw ); }
}

#define sig_joint( id, bccw )    emit signal_joint_zero( id, bccw );
void H2Pref::on_btnZeroX_clicked()
{
    sig_joint( 0, false );
}

void H2Pref::on_btnZeroY_clicked()
{
    sig_joint( 1, false );
}

void H2Pref::on_btnZeroBody_clicked()
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
    if ( ui->chkCcw->isVisible() )
    {
        jList<<0<<1<<2;
        ccwList<<false<<false<<ui->chkCcw->isChecked();
    }
    else
    {
        jList<<0<<1;
        ccwList<<false<<false;
    }
    pBase->goZero( tpvRegion(0,0),
                   jList,
                   ccwList
                   );
}

void H2Pref::on_btnZeroZ_clicked()
{
    sig_joint( 2, ui->chkCcw->isChecked() );
}
