#include "h2pref.h"
#include "ui_h2pref.h"

#include "megamessagebox.h"
#include "../../com/comassist.h"

#define time_unit   (1.0e-6f)

H2Pref::H2Pref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::H2Pref)
{
    ui->setupUi(this);

    connect( this, SIGNAL(signal_joint_zero(int, bool)),
             this, SLOT(slot_joint_zero(int,bool)) );

    spyEdited();
}

H2Pref::~H2Pref()
{
    delete ui;
}

void H2Pref::changeEvent( QEvent *event )
{
    modelView::changeEvent( event );

    if ( event->type() == QEvent::LanguageChange )
    { ui->retranslateUi( this ); }
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

        ui->spinGapTime,
        ui->spinGapDist,
    };

    QComboBox *comboxes[]={
        ui->cmbZeroCorner,
        ui->cmbZeroMovement
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

//    //! ccw
//    if ( ui->chkCcwX->isVisible() )
//    { pRobo->setJointZeroCcw( 0, ui->chkCcwX->isChecked() ); }
//    if ( ui->chkCcwY->isVisible() )
//    { pRobo->setJointZeroCcw( 1, ui->chkCcwY->isChecked() ); }
//    if ( ui->chkCcw->isVisible() )
//    { pRobo->setJointZeroCcw( 2, ui->chkCcw->isChecked() ); }

    //! zero movement, corner
    pRobo->setZeroCorner( ui->cmbZeroCorner->currentIndex() );
    pRobo->setZeroMovement( ui->cmbZeroMovement->currentIndex() );

    //! zero speed
    pRobo->setZeroPref( ui->spinZeroSpeed->value(),
                        comAssist::align( ui->spinZeroTmo->value(), time_unit ),
                        comAssist::align( ui->spinZeroTick->value(), time_unit )
                        );
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

//    //! check
//    if ( ui->chkCcwX->isVisible() )
//    { ui->chkCcwX->setChecked( pRobo->jointZeroCcwList().at(0) ); }
//    if ( ui->chkCcwY->isVisible() )
//    { ui->chkCcwY->setChecked( pRobo->jointZeroCcwList().at(1) ); }
//    if ( ui->chkCcw->isVisible() )
//    { ui->chkCcw->setChecked( pRobo->jointZeroCcwList().at(2) ); }

    //! movement, corner
    int index;
    index = pRobo->zeroCorner();
    ui->cmbZeroCorner->setCurrentIndex( index );

    index = pRobo->zeroMovement();
    ui->cmbZeroMovement->setCurrentIndex( index );

    //! zero speed
    double spd;
    int zeroTmo, zeroTick;
    pBase->zeroPref( spd, zeroTmo, zeroTick );
    ui->spinZeroSpeed->setValue( spd );
    ui->spinZeroTmo->setValue( zeroTmo * time_unit );
    ui->spinZeroTick->setValue( zeroTick * time_unit );

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

//    //! x,y,ccw visible
//    ui->chkCcwX->setVisible( pBase->jointZeroCcwVisibleList().at(0) );
//    ui->chkCcwY->setVisible( pBase->jointZeroCcwVisibleList().at(1) );

//    ui->chkCcwX->setChecked( pBase->jointZeroCcwList().at(0) );
//    ui->chkCcwY->setChecked( pBase->jointZeroCcwList().at(1) );

    //! z enable
//    ui->labelZ->setVisible( bZ );
    ui->btnZeroZ->setVisible( bZ );
//    ui->chkCcw->setVisible( bZ );
    ui->gpZGap->setVisible( bZ );

    //! ccw list
//    if ( bZ )
//    {
//        ui->chkCcw->setChecked( pBase->jointZeroCcwList().at(2) );
//    }

//    ui->label_3->setPixmap( QPixmap(strPixmapGeo) );
}

void H2Pref::zeroJoint( int jointId, bool bCcw )
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    if ( !pBase->checkLink() )
    {
        sysPrompt( tr("Invalid connection") );
        return;
    }

    pBase->goZero( tpvRegion(0,ui->widget->page()), jointId, bCcw );
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
    if ( ui->btnZeroZ->isVisible() )
    {
        jList<<0<<1<<2;

        ccwList<<false
               <<false
               <<false;
    }
    else
    {
        jList<<0<<1;

        ccwList<<false
               <<false;

    }
    pBase->goZero( tpvRegion(0,ui->widget->page()),
                   jList,
                   ccwList
                   );
}

void H2Pref::on_btnZeroZ_clicked()
{
    sig_joint( 2, false );
}

//void H2Pref::on_chkCcwX_clicked(bool checked)
//{
//    Q_ASSERT( m_pModelObj != NULL );
//    VRobot *pBase = ( VRobot *)m_pModelObj;
//    Q_ASSERT( NULL != pBase );

//    pBase->setJointZeroCcw( 0, checked );
//}

//void H2Pref::on_chkCcwY_clicked(bool checked)
//{
//    Q_ASSERT( m_pModelObj != NULL );
//    VRobot *pBase = ( VRobot *)m_pModelObj;
//    Q_ASSERT( NULL != pBase );

//    pBase->setJointZeroCcw( 1, checked );
//}

//void H2Pref::on_chkCcw_clicked(bool checked)
//{
//    Q_ASSERT( m_pModelObj != NULL );
//    VRobot *pBase = ( VRobot *)m_pModelObj;
//    Q_ASSERT( NULL != pBase );

//    pBase->setJointZeroCcw( 2, checked );
//}
