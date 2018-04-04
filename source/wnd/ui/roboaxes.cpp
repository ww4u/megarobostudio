#include "roboaxes.h"
#include "ui_roboaxes.h"

#include "../../../include/mcstd.h"

#include "../../device/mrq/deviceMRQ.h"

roboAxes::roboAxes(mcModel *pModel,
                   QWidget *parent) :
    DlgView( pModel, parent ),
    ui(new Ui::roboAxes)
{
    ui->setupUi(this);

    connect( &mTimer, SIGNAL(timeout()),
             this, SLOT(slot_timeout()) );


    mJoints.append( ui->jointTab0 );
    mJoints.append( ui->jointTab1 );
    mJoints.append( ui->jointTab2 );
    mJoints.append( ui->jointTab3 );
    mJoints.append( ui->jointTab4 );
    mJoints.append( ui->jointTab5 );

//    //! title
//    mJoints[0]->setTitle( tr("Basement") );
//    mJoints[1]->setTitle( tr("Big Arm") );
//    mJoints[2]->setTitle( tr("Little Arm") );
//    mJoints[3]->setTitle( tr("Wrist") );
//    mJoints[4]->setTitle( tr("Hand") );
//    mJoints[5]->setTitle( tr("Hand") );

//    //! angle
//    mJoints[4]->setAngleVisible( false );

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
    slot_robo_changed("");
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
            sysError( tr("Invalid device") );
            return;
        }

        //! now for angle
        float angle;
        angle = pMrq->getAbsAngle( subAx );

        Q_ASSERT( jointId < mJoints.size() );
        mJoints.at( jointId )->setAngle( angle );
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
}

VRobot *roboAxes::Robot()
{
    Q_ASSERT( NULL != m_pMcModel );
    return m_pMcModel->m_pInstMgr->findRobot( m_pMcModel->mConn.getRoboName() );
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
    for ( jointId = 0; jointId < pRobo->axes(); jointId++ )
    {
        Q_ASSERT( jointId < pRobo->mJointAngleMask.size() );
        mJoints[jointId]->setAngleVisible( pRobo->mJointAngleMask[jointId] );
    }
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
        sysError( tr("Invalid device") );
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

    QMessageBox msgBox;
    msgBox.setText( tr("Sure to zero?") );
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel );
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        pRobo->goZero();
    }
}
