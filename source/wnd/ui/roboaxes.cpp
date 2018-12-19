#include "roboaxes.h"
#include "ui_roboaxes.h"

#include "../../../include/mcstd.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../widget/megamessagebox.h"
roboAxes::roboAxes(mcModel *pModel,
                   VRobot *pRobo,
                   const QString &roboName,
                   QWidget *parent) :
                    RoboView( pRobo, RoboView::view_joint, 0,
                              pModel, parent ),
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

    //! lcd
    mPoseLcds.append( ui->lcdPosX );
    mPoseLcds.append( ui->lcdPosY );
    mPoseLcds.append( ui->lcdPosZ );

    //! joint id
    for ( int i = 0; i < mJoints.size(); i++ )
    {
        mJoints.at(i)->setId( i );
    }

    //! connection
    buildConnection();

    setModal( false );

    mTimer.setInterval( ui->sampleTimer->value() );

    //! by model data
    slot_robo_changed( roboName );
}

roboAxes::~roboAxes()
{
    delete ui;
}

void roboAxes::changeEvent(QEvent * event)
{
    RoboView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
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

    float angle, dAngle;

    for ( int jointId = 0; jointId < pRobo->mJointAngleMask.size(); jointId++ )
    {
        if ( pRobo->mJointAngleMask.at(jointId) )
        {}
        else
        { continue; }

        pMrq = pRobo->jointDevice( jointId, &subAx );
        if ( NULL == pMrq )
        {
            sysError( tr("Invalid device"), __FUNCTION__,QString::number(__LINE__) );
            return;
        }

        //! now for angle
        if ( pRobo->angleType() == robo_angle_abs )
        { angle = pMrq->getAbsAngle( subAx ); }
        else if ( pRobo->angleType() == robo_angle_inc )
        { angle = pMrq->getIncAngle( subAx ); }
        else
        {
            sysError( tr("Invalid angle") );
            return;
        }

        //! convert delta angle
        if ( pRobo->mJointDeltaAngleMask.at(jointId) )
        { dAngle = pRobo->toDeltaAngle( jointId, angle ); }
        else
        { dAngle = 0; }

        Q_ASSERT( jointId < mJoints.size() );
        mJoints.at( jointId )->setAngle( angle, dAngle );
    }

    //! robo pos
    float pos[16];
    if ( pRobo->poseCount() > 0 )
    {
        //! read success
        if ( pRobo->getPOSE( pos ) == 0 )
        {
            for ( int i = 0; i < pRobo->poseCount(); i++ )
            { mPoseLcds.at(i)->display( ( pos[i] ) ); }
        }
        else
        {}
    }
}

void roboAxes::slot_joint_action( int id, float dt, float angle, float ev )
{
    rotate( id,
            0, 0,
            dt, angle, ev );
}

void roboAxes::slot_joint_stop( int id )
{
    stop( id );
}

void roboAxes::slot_joint_zero( int id, bool bCcw )
{
    zero( id, bCcw );
}

void roboAxes::slot_joint_align( int jId )
{
    align( jId );
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
        mJoints[jointId]->setAngleVisible( pRobo->mJointAngleMask[jointId],
                                           pRobo->mJointDeltaAngleMask[jointId] );
        if ( pRobo->mJointAngleMask[jointId] )
        { angleCnt++; }

        mJoints[jointId]->setCcwVisible( pRobo->mJointCcwMask[jointId] );
    }

    //! align able
    for ( jointId = 0; jointId < pRobo->axes(); jointId++ )
    {
        Q_ASSERT( jointId < pRobo->mJointAlignAble.size() );

        mJoints[ jointId ]->setAlignAble( pRobo->mJointAlignAble.at( jointId ) );
    }

    //! ccw checked
    QList<bool> ccwList = pRobo->jointZeroCcwList();
    for ( int i = 0; i < ccwList.size(); i++ )
    {
        mJoints[i]->setCcwChecked( ccwList.at(i) );
    }

    ui->groupBox_3->setVisible( angleCnt > 0 || pRobo->poseCount() > 0 );

    //! lcd
    {
        ui->gpLcdPos->setVisible( pRobo->poseCount() > 0 );
        ui->gpLcdPos->setTitle( pRobo->poseTitles().join(",") );

        for ( int i = 0; i < pRobo->poseCount(); i++ )
        { mPoseLcds.at(i)->setVisible( true ); }
        for ( int i = pRobo->poseCount(); i < mPoseLcds.size(); i++ )
        { mPoseLcds.at(i)->setVisible( false ); }
    }

    //! coordable
    ui->coordOption->setVisible( pRobo->coordAble() );
    ui->coordOption->setOption( (int)pRobo->getCoord() );

    //! adapt ui
    on_chkSingle_clicked( ui->chkSingle->isChecked() );
    on_spinStepTime_valueChanged( ui->spinStepTime->value() );
}

void roboAxes::buildConnection()
{
    foreach( RoboJoint *p, mJoints )
    {
        connect( p, SIGNAL(signal_actionChanged(int,float,float, float)),
                 this, SLOT(slot_joint_action(int,float,float,float)) );

        connect( p, SIGNAL(signal_stop(int)),
                 this, SLOT(slot_joint_stop(int)) );

        connect( p, SIGNAL(signal_zeroClicked(int,bool)),
                 this, SLOT(slot_joint_zero(int,bool)));

        connect( p, SIGNAL(signal_align_clicked(int)),
                 this, SLOT(slot_joint_align(int)) );
    }

    connect( ui->widget->getComb(), SIGNAL(currentIndexChanged(int)),
             this, SLOT(slot_comboBox_currentIndexChanged(int)) );

    connect( ui->coordOption, SIGNAL(signal_option_changed(int)),
             this, SLOT(slot_coord_option_changed(int)) );
}

//! convert the time by t
#define time_to_s( t )  ((t)/1000.0)
void roboAxes::rotate( int jointId,
                       float t1, float a1,
                       float t2, float a2,
                       float ev )
{
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

    //! only one time
    pMrq->setMOTIONPLAN_CYCLENUM( subAx, (MRQ_MOTION_SWITCH_1)ui->widget->page(), 1 );

    pMrq->pvtWrite( tpvRegion(subAx,ui->widget->page() ),
                    (t1), a1,
                    (t2), a2,
                    ev
                    );

    pMrq->run( tpvRegion(subAx, ui->widget->page() ) );
}

void roboAxes::stop( int jointId )
{
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

    pMrq->stop( tpvRegion(subAx, ui->widget->page() ) );
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

    pRobo->goZero( tpvRegion(0, ui->widget->page() ), jointId, bCcw );
}

void roboAxes::align( int jId )
{
    VRobot *pRobo = Robot();
    if ( NULL == pRobo )
    {
        sysError( tr("Invalid robot") );
        return;
    }

    pRobo->align( tpvRegion( jId, ui->widget->page() ) );
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
        //! foreach joint ccw
        for ( int jointId = 0; jointId < pRobo->axes(); jointId++ )
        {
            Q_ASSERT( jointId < mJoints.size() );
            if ( mJoints[jointId]->isCcwVisible() )
            { pRobo->setJointZeroCcw( jointId, mJoints[jointId]->isCcwChecked()); }
        }

        //! zero
        pRobo->goZero( tpvRegion(0, ui->widget->page() ) );
    }
}

void roboAxes::on_spinStepTime_valueChanged(double arg1)
{
    foreach (RoboJoint*pJ, mJoints)
    {
        Q_ASSERT( NULL != pJ );
        pJ->setStepTime( arg1 );
    }
}

void roboAxes::slot_comboBox_currentIndexChanged(int index)
{
    VRobot *pRobo = Robot();
    if ( NULL == pRobo )
    {
        sysError( tr("Invalid robot") );
        return;
    }

    int status = pRobo->state( tpvRegion( 0, index), 0 );

    ui->labelStat->clear();

    ui->labelStat->setText( MegaDevice::deviceMRQ::toString( (MegaDevice::mrqState)status ) );
}

void roboAxes::slot_coord_option_changed( int index )
{
    VRobot *pRobo = Robot();
    if ( NULL == pRobo )
    {
        sysError( tr("Invalid robot") );
        return;
    }

    //! config coord
    if ( pRobo->coordAble() )
    { pRobo->setCoord( (eRoboCoord)index ); }
}

void roboAxes::on_toolButton_clicked()
{
    slot_comboBox_currentIndexChanged( ui->widget->page() );
}

void roboAxes::on_chkSingle_clicked(bool checked)
{
    foreach (RoboJoint*pJ, mJoints)
    {
        Q_ASSERT( NULL != pJ );
        pJ->setSingle( checked );
    }
}
