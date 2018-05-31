#include "roboaxes.h"
#include "ui_roboaxes.h"

#include "../../../include/mcstd.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../widget/megamessagebox.h"
roboAxes::roboAxes(mcModel *pModel,
                   const QString &roboName,
                   QWidget *parent) :
    DlgView( pModel, parent ),
    ui(new Ui::roboAxes)
{
    ui->setupUi(this);

    setWindowTitle( roboName );

    setAttribute( Qt::WA_DeleteOnClose );

    connect( &mTimer, SIGNAL(timeout()),
             this, SLOT(slot_timeout()) );

    mJoints.append( ui->jointTab0 );
    mJoints.append( ui->jointTab1 );
    mJoints.append( ui->jointTab2 );
    mJoints.append( ui->jointTab3 );
    mJoints.append( ui->jointTab4 );
    mJoints.append( ui->jointTab5 );

    //! joint id
    for ( int i = 0; i < mJoints.size(); i++ )
    {
        mJoints.at(i)->setId( i );
    }

    //! connection
    foreach( RoboJoint *p, mJoints )
    {
        connect( p, SIGNAL(signal_actionChanged(int,float,float)),
                 this, SLOT(slot_joint_action(int,float,float)) );
        connect( p, SIGNAL(signal_zeroClicked(int,bool)),
                 this, SLOT(slot_joint_zero(int,bool)));
    }

    setModal( false );

    mTimer.setInterval( ui->sampleTimer->value() );

    //! by model data
    slot_robo_changed( roboName );
}

roboAxes::~roboAxes()
{
    delete ui;
}

void roboAxes::on_chkAngle_toggled(bool checked)
{
    if ( checked )
    {
        mTimer.stop();
        mTimer.start( ui->sampleTimer->value() );
    }
    else
    {
        mTimer.stop();
    }
}

void roboAxes::on_sampleTimer_valueChanged( int val )
{
    on_chkAngle_toggled( ui->chkAngle->isChecked() );
}

//! sample angle
void roboAxes::slot_timeout()
{
    if ( isVisible() )
    {}
    else
    { return; }

    VRobot *pRobo = Robot();
    if ( pRobo == NULL )
    { return; }

    //! sample the angle for joint 0~3
    int subAx;
    MegaDevice::deviceMRQ *pMrq;
    for ( int jointId = 0; jointId < 4; jointId++ )
    {
        pMrq = pRobo->jointDevice( jointId, &subAx );
        if ( NULL == pMrq )
        {
            sysError( tr("Invalid device"), __FUNCTION__,QString::number(__LINE__) );
            return;
        }

        //! now for angle
        float angle;
        angle = pMrq->getAbsAngle( subAx );

        Q_ASSERT( jointId < mJoints.size() );
        mJoints.at( jointId )->setAngle( angle );
    }

    //! robo pos
    float pos[16];
    if ( pRobo->poseCount() > 0 )
    {
        if ( pRobo->getPOSE( pos ) == 0 )
        {
            ui->lcdPosX->display( pos[0] );
            ui->lcdPosY->display( pos[1] );
            ui->lcdPosZ->display( pos[2] );
        }
        else
        {}
    }
}

void roboAxes::slot_joint_action( int id, float dt, float angle )
{
    rotate( id,
            0, 0,
            dt, angle );
}
void roboAxes::slot_joint_zero( int id, bool bCcw )
{
//    logDbg()<<id<<bCcw;
    zero( id, bCcw );
}

void roboAxes::slot_robo_changed( const QString &roboName )
{
    //! find robo
    VRobot *pRobo = Robot();
    if ( pRobo == NULL )
    {
        setEnabled(false);
        return;
    }

    //! enalbed
    setEnabled(true);

    //! update the ui by robo
    adapteUiToRobot( pRobo );

    setWindowIcon( QIcon( QPixmap::fromImage(pRobo->mImage)) );
}

void roboAxes::slot_device_changed()
{
    close();
}

VRobot *roboAxes::Robot()
{
    Q_ASSERT( NULL != m_pMcModel );
//    return m_pMcModel->m_pInstMgr->findRobot( m_pMcModel->mConn.getRoboName() );
    return m_pMcModel->m_pInstMgr->findRobot( windowTitle() );
}

void roboAxes::adapteUiToRobot( VRobot *pRobo )
{
    Q_ASSERT( NULL != pRobo );

    //! enable/visible
    int jointId;
    for ( jointId = 0; jointId < pRobo->axes(); jointId++ )
    {
        Q_ASSERT( jointId < mJoints.size() );
        mJoints[jointId]->setVisible( true );
    }
    for ( ; jointId < mJoints.size(); jointId++ )
    {
        mJoints[jointId]->setVisible( false );
    }

    //! joint name
    for ( jointId = 0; jointId < pRobo->axes(); jointId++ )
    {
        Q_ASSERT( jointId < mJoints.size() );
        Q_ASSERT( jointId < pRobo->mJointName.size() );

        mJoints[jointId]->setTitle( pRobo->mJointName[jointId] );
    }

    //! joint angle
    int angleCnt = 0;
    for ( jointId = 0; jointId < pRobo->axes(); jointId++ )
    {
        Q_ASSERT( jointId < pRobo->mJointAngleMask.size() );
        mJoints[jointId]->setAngleVisible( pRobo->mJointAngleMask[jointId] );
        if ( pRobo->mJointAngleMask[jointId] )
        { angleCnt++; }

        mJoints[jointId]->setCcwVisible( pRobo->mJointCcwMask[jointId] );
    }

    //! ccw checked
    QList<bool> ccwList = pRobo->jointZeroCcwList();
    for ( int i = 0; i < ccwList.size(); i++ )
    {
        mJoints[i]->setCcwChecked( ccwList.at(i) );
    }

    ui->groupBox_3->setVisible( angleCnt > 0 );

    ui->gpLcdPos->setVisible( pRobo->poseCount() > 0 );
}

void roboAxes::buildConnection()
{
}

//! convert the time by t
#define time_to_s( t )  ((t)/1000.0)
void roboAxes::rotate( int jointId,
                       float t1, float a1,
                       float t2, float a2 )
{
    logDbg()<<jointId<<t1<<a1<<t2<<a2;

    VRobot *pRobo = Robot();
    if ( NULL == pRobo )
    {
        sysError( tr("Invalid robot") );
        return;
    }

    Q_ASSERT( NULL != pRobo );

    int subAx;
    MegaDevice::deviceMRQ *pMrq = pRobo->jointDevice( jointId, &subAx );
    if ( NULL == pMrq )
    {
        sysError( tr("Invalid device"), __FUNCTION__,QString::number(__LINE__) );
        return;
    }

    logDbg()<<subAx<<pMrq->name();

    //! only one time
    pMrq->setMOTIONPLAN_CYCLENUM( subAx, MRQ_MOTION_SWITCH_1_MAIN, 1 );

    pMrq->pvtWrite( tpvRegion(subAx,0),
                    time_to_s(t1), a1,
                    time_to_s(t2), a2
                    );
    pMrq->run( tpvRegion(subAx,0) );
}

void roboAxes::zero( int jointId,
                     bool bCcw )
{
    VRobot *pRobo = Robot();
    if ( NULL == pRobo )
    {
        sysError( tr("Invalid robot") );
        return;
    }

    Q_ASSERT( NULL != pRobo );

    pRobo->goZero( jointId, bCcw );
}

void roboAxes::on_btnZero_clicked()
{
    VRobot *pRobo = Robot();
    if ( NULL == pRobo )
    {
        sysError( tr("Invalid robot") );
        return;
    }

    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        pRobo->goZero();
    }
}
