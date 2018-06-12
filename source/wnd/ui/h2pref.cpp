#include "h2pref.h"
#include "ui_h2pref.h"

#include "megamessagebox.h"

H2Pref::H2Pref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::H2Pref)
{
    ui->setupUi(this);

    connect( this, SIGNAL(signal_joint_zero(int)),
             this, SLOT(slot_joint_zero(int)) );


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
                   ui->spinGapDist->value() );
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

    double gapTime, gapDistance;
    pRobo->gap( gapTime, gapDistance );
    ui->spinGapTime->setValue( gapTime );
    ui->spinGapDist->setValue( gapDistance );
}

void H2Pref::adaptUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    bool bZ = false;
    QString strPixmapGeo;
    if ( pBase->getId() == VRobot::robot_h2 )
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

void H2Pref::slot_joint_zero( int jId )
{
    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    { zeroJoint( jId, false ); }    //! \note no use for joint
}

#define sig_joint( id )    emit signal_joint_zero( id );
void H2Pref::on_btnZeroX_clicked()
{
    sig_joint( 0 );
}

void H2Pref::on_btnZeroY_clicked()
{
    sig_joint(1 );
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
    pBase->goZero( tpvRegion(0,0) );
}

void H2Pref::on_btnZeroZ_clicked()
{
    sig_joint( 2 );
}
