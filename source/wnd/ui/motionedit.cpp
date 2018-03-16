#include "motionedit.h"

#include "ui_motionedit.h"

#include "../../model/tpvgroup.h"

#include "../../arith/pathplan/pathplan.h"
#include "../../arith/kinematic/kinematic.h"

#include "../../device/mrq/deviceMRQ.h"

motionEdit::motionEdit(QWidget *parent) : tableEdit(parent),
    ui(new Ui::motionEdit)
{
    ui->setupUi(this);

    buildConnection();

    mFilePattern<<motion_desc<<motion_ext;

    m_pRoboAxes = NULL;
    m_pPlot = NULL;

    m_pProgress = NULL;
    m_pActionDelegate = NULL;

    //! delegate
    m_pActionDelegate = new comboxDelegate();
    Q_ASSERT( NULL != m_pActionDelegate );
    ui->tableView->setItemDelegateForColumn( 1, m_pActionDelegate );

    //! debug
    setAgent( "test_2" );
    setLink( true );
}

void motionEdit::buildConnection()
{
    connect( this, SIGNAL(sig_joints_trace_changed()),
             this, SLOT(slot_joints_trace_changed()) );
}

motionEdit::~motionEdit()
{
    delete ui;

    delete m_pActionDelegate;

    delete_all( mJointsTpvGroup );
}

//! get the hand action options
void motionEdit::slot_robo_changed( const QString &roboName )
{
    Q_ASSERT( m_pActionDelegate != NULL );
logDbg();
    do
    {
        VRobot *pRobot = m_pmcModel->m_pInstMgr->findRobot( roboName );
        if ( NULL == pRobot )
        { break; }
logDbg();
        if ( pRobot->handActions() == NULL )
        { break; }
logDbg();
        handActionModel *pModel = (handActionModel*)pRobot->handActions();
        if ( NULL == pModel )
        { break; }
logDbg();
        QMap<QString, double> actionMap = pModel->actionMap();
        if ( actionMap.size() < 1 )
        { break; }
logDbg();
        //! get item
        QList<QString> strList = actionMap.keys();logDbg()<<QStringList( strList );logDbg();
        m_pActionDelegate->setItems( QStringList( strList ) );
        return;
    }while( 0 );

    //! no item
    m_pActionDelegate->setItems( QStringList() );
}

//! update the status by
void motionEdit::slot_joints_trace_changed( )
{
    if ( mJointsPlan.size() > 0 )
    { ui->btnDown->setEnabled(true); }
    else
    { ui->btnDown->setEnabled(false); }

    if ( mJointsPlan.size() > 0 )
    {
        ui->btnGraph->setEnabled(true);

        if ( m_pPlot != NULL && m_pPlot->isVisible() )
        {
            updatePlot();
        }
    }
    else
    { ui->btnGraph->setEnabled(false); }

    ui->btnExport->setEnabled( ui->btnGraph->isEnabled() );
}

void motionEdit::setModelObj( mcModelObj *pObj )
{
    Q_ASSERT( NULL != pObj );

    modelView::setModelObj( pObj );

    mMotionGroup = (motionGroup*)(pObj->getObj());
    Q_ASSERT( NULL != mMotionGroup );
    logDbg()<<mMotionGroup->getName();
    ui->tableView->setModel( mMotionGroup );
}

int motionEdit::save( QString &outFileName )
{
    outFileName = mMotionGroup->getPath() +"/" + mMotionGroup->getName();

    return mMotionGroup->save( outFileName );
}

int motionEdit::saveAs( QString &outFileName )
{
    return mMotionGroup->save( outFileName );
}

void motionEdit::onNetEvent(const QString &name,
                        int axes,
                        RoboMsg &msg)
{
    if ( msg.getMsg() == e_download_started )
    {
        progress()->progressInfo( name, axes, QString("%1:CH%2 %3").arg(name).arg(axes).arg( tr("started") ) );
        progress()->progressProg( name, axes, 0 );
        progress()->progressShow( name, axes, true );

        logDbg()<<name<<axes;
    }
    else if ( msg.getMsg() == e_download_processing )
    {
        int now, total;

        now = msg.at(2).toInt();
        total = msg.at(3).toInt();

//        progress()->setRange( 0, total );
//        progress()->setValue( now );

//        logDbg()<<now<<total;

        logDbg()<<name<<axes;

        progress()->progressInfo( name, axes, QString("%1:CH%2 %3").arg(name).arg(axes).arg( tr("downloading") ) );
        progress()->progressRange( name, axes, 0, total );
        progress()->progressProg( name, axes, now );
    }
    else if ( msg.getMsg() == e_download_completed )
    {
//        progress()->hide();
//        delete progress();
//        m_pProgress = NULL;

//        ui->btnDown->setEnabled( true );

        logDbg()<<name<<axes;

        progress()->progressShow( name, axes, false );
    }
    else if ( msg.getMsg() == e_download_terminated )
    {
//        sysLog( msg.at(0).toString(),
//                QString("%1").arg( msg.at(1).toInt()),
//                tr("Terminated") );

//        ui->btnDown->setEnabled( true );

        logDbg()<<name<<axes;

        progress()->progressShow( name, axes, false );
    }

    else if ( msg.getMsg() == e_robot_status )
    {
        logDbg()<<axes;
        int stat;
        stat = msg.at(1).toInt();

        onMotionStatus( -1, (MRQ_MOTION_STATE)stat );

        logDbg();
    }

    else
    {}
}


#define down_start_stop( bdown, bstart, bstop )     \
                                                ui->btnDown->setEnabled( bdown );\
                                                ui->btnStart->setEnabled( bstart );\
                                                ui->btnStop->setEnabled( bstop );
void motionEdit::onMotionStatus( int axes,
                                 MRQ_MOTION_STATE stat )
{
    logDbg()<<stat;

//    MRQ_MOTION_STATE_POWERON,
//	MRQ_MOTION_STATE_IDLE,
//	MRQ_MOTION_STATE_CALCING,
//	MRQ_MOTION_STATE_CALCEND,
//	MRQ_MOTION_STATE_STANDBY,
//	MRQ_MOTION_STATE_RUNNING,
//	MRQ_MOTION_STATE_ERROR,

//    mrq_state_idle,
//    mrq_state_run_reqed,
//    mrq_state_program,

//    mrq_state_calcing,
//    mrq_state_calcend,
//    mrq_state_standby,

//    mrq_state_prerun,
//    mrq_state_running,
//    mrq_state_prestop,

    //! dis link
    if ( MegaDevice::mrq_msg_idle == stat )
    {
        setLink( false );
        logDbg()<<"dis link";
    }

    if ( MegaDevice::mrq_state_idle == stat
         || MegaDevice::mrq_state_run_reqed == stat )
    {
        down_start_stop( true, false, false );
    }
    else if ( MegaDevice::mrq_state_program == stat )
    {
        down_start_stop( true, false, false );
    }
    else if ( MegaDevice::mrq_state_calcing == stat )
    {
        down_start_stop( true, false, false );
    }
    else if ( MegaDevice::mrq_state_calcend == stat )
    {
        down_start_stop( true, false, false );
    }
    else if ( MegaDevice::mrq_state_standby == stat )
    {
        down_start_stop( true, true, false );

        progress()->hide();
    }
    else if ( MegaDevice::mrq_state_prerun == stat )
    {
        down_start_stop( true, false, true );
    }
    else if ( MegaDevice::mrq_state_running == stat )
    {
        down_start_stop( true, false, true );
    }
    else if ( MegaDevice::mrq_state_prestop == stat )
    {
        down_start_stop( true, false, true );
    }
    else
    { Q_ASSERT(false); }
}

ProgressGroup *motionEdit::progress()
{
    if ( NULL != m_pProgress )
    { }
    else
    {
        m_pProgress = new ProgressGroup(this);

        connect( m_pProgress,
                 SIGNAL(sigCancel( const QString &, int)),
                 this,
                 SLOT(slot_download_cancel( const QString &, int)) );
    }

    Q_ASSERT( NULL != m_pProgress );

    return m_pProgress;
}

VRobot *motionEdit::currentRobot()
{
    return m_pmcModel->m_pInstMgr->findRobot( m_pmcModel->mConn.getRoboName() );
}

int motionEdit::setLoop( int loop )
{
    //! check robo
    VRobot *pRobo;
    pRobo = currentRobot();
    if ( NULL == pRobo )
    { return ERR_INVALID_ROBOT_NAME; }

    return pRobo->setLoop( loop );
}

void motionEdit::testDownload()
{
    //! prepare data
    QList< tpvGroup * > groups;
    groups.append( tpvGroup::createDemoGroup1() );
    groups.append( tpvGroup::createDemoGroup2() );
    groups.append( tpvGroup::createDemoGroup3() );
    groups.append( tpvGroup::createDemoGroup4() );
    groups.append( tpvGroup::createDemoGroup5() );
logDbg();
    QList< int > joints;
    joints.append( 0 );
    joints.append( 1 );
    joints.append( 2 );
    joints.append( 3 );
    joints.append( 4 );
logDbg();

    doDownload( groups, joints );
}

#if 0
int motionEdit::buildTrace()
{ return 0;}
#else
//int motionEdit::buildTrace()
//{
//    //! pre build
//    if ( NULL != m_pJointsTrace )
//    {
//        delete []m_pJointsTrace;
//        m_pJointsTrace = NULL;
//    }

//    if ( NULL != m_pTracePoint )
//    {
//        delete []m_pTracePoint;
//        m_pTracePoint = NULL;
//    }

//    //! data size
//    int dataSize = mMotionGroup->mItems.size();
//    if ( dataSize <  1 )
//    { return -1; }

//    //! data cache
//    endPoint *pEndPoints = new endPoint[ dataSize ];
//    if ( NULL == pEndPoints )
//    { return -2; }
//    memset( pEndPoints, 1, sizeof(endPoint)*dataSize );

//    int count = mMotionGroup->fetech( &pEndPoints->datas + offsetof(endPoint,t)/sizeof(double), sizeof(endPoint)/sizeof(double),
//                                      &pEndPoints->datas + offsetof(endPoint,x)/sizeof(double), sizeof(endPoint)/sizeof(double),
//                                      &pEndPoints->datas + offsetof(endPoint,y)/sizeof(double), sizeof(endPoint)/sizeof(double),
//                                      &pEndPoints->datas + offsetof(endPoint,z)/sizeof(double), sizeof(endPoint)/sizeof(double),
//                                      dataSize );
//    if ( count != dataSize )
//    {
//        delete []pEndPoints;
//        return -3;
//    }

//    //! trace plan
//    int xyzResLen;
//    int ret = ns_pathplan::GetPvtLen( &pEndPoints->datas,
//                                      count,
//                                      ui->spinStep->value(),
//                                      ui->cmbInterp->currentIndex(),
//                                      &xyzResLen );
//    if ( ret != 0 )
//    {
//        delete []pEndPoints;
//        return -4;
//    }
//    delete []pEndPoints;

//    mTracePointSize = xyzResLen * sizeof(double) / sizeof(tracePoint);
//    if ( mTracePointSize > 1 )
//    { }
//    else
//    { return -5; }
//    m_pTracePoint = new tracePoint[ mTracePointSize ];
//    if ( NULL == m_pTracePoint )
//    { return -5; }

//    ret = ns_pathplan::GetPvtInfo( &m_pTracePoint->datas, xyzResLen );
//    if ( ret != 0 )
//    {
//        return -6;
//    }

//    //! trace split
//    //! arm
//    VRobot *pRobot;
//    pRobot = currentRobot();
//    jointsAngle angle = {
//                            DEG_TO_RAD( pRobot->mRefAngles.at(0) ),
//                            DEG_TO_RAD( pRobot->mRefAngles.at(1) ),
//                            DEG_TO_RAD( pRobot->mRefAngles.at(2) ),
//                            DEG_TO_RAD( pRobot->mRefAngles.at(3) ),
//                            };

//    m_pJointsTrace = new jointsTrace[ mTracePointSize ];
//    if ( NULL == m_pJointsTrace )
//    {
//        delete []m_pTracePoint;
//        m_pTracePoint = NULL;
//        return -6;
//    }

//    ret = ns_kinematic::GetArmPosition( (double*)(&angle),

//                    &m_pTracePoint->datas + offsetof( tracePoint, x )/sizeof(double), sizeof(tracePoint)/sizeof(double),
//                    &m_pTracePoint->datas + offsetof( tracePoint, vx )/sizeof(double),sizeof(tracePoint)/sizeof(double),
//                    &m_pTracePoint->datas + offsetof( tracePoint, t )/sizeof(double),sizeof(tracePoint)/sizeof(double),

//                    mTracePointSize,
//                    &m_pJointsTrace->datas
//                    );
//    if ( ret <= 0 )
//    {
//        mJointsTraceSize = 0;
//        delete []m_pJointsTrace;
//        m_pJointsTrace = NULL;

//        delete []m_pTracePoint;
//        m_pTracePoint = NULL;
//    }
//    else
//    {
//        mJointsTraceSize = ret;

//        //! rotate by cal
//        jointsRotate( m_pJointsTrace, mJointsTraceSize );
//    }

//logDbg()<<mTracePointSize<<mJointsTraceSize;
//    return 0;
//}
#endif


void motionEdit::on_btnAdd_clicked()
{
    int curRow;

    //! current
    curRow = ui->tableView->currentIndex().row();

    mMotionGroup->insertRow( curRow + 1  );

    ui->tableView->setCurrentIndex( mMotionGroup->index( curRow + 1, 0) );
}
void motionEdit::on_btnDel_clicked()
{ mMotionGroup->removeRow( ui->tableView->currentIndex().row() ); }
void motionEdit::on_btnClr_clicked()
{
    QMessageBox msgBox;
    msgBox.setText( tr("Sure to delete all?") );
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel );
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        mMotionGroup->removeRows( 0, mMotionGroup->rowCount(QModelIndex()), QModelIndex() );
    }
}

void motionEdit::slot_download_cancel( const QString &name, int id )
{
    //! \todo
}

//! context menu
void motionEdit::context_remove()
{
    on_btnDel_clicked();
}
void motionEdit::context_add_before()
{
    int curRow;

    //! current
    curRow = ui->tableView->currentIndex().row();

    mMotionGroup->insertRow( curRow  );

    ui->tableView->setCurrentIndex( mMotionGroup->index( curRow, 0) );
}
void motionEdit::context_add_below()
{
    on_btnAdd_clicked();
}

void motionEdit::context_clear()
{
    on_btnClr_clicked();
}

void motionEdit::on_btnKnob_clicked()
{
    //! connect to robot
    VRobot *pRobot = currentRobot();
    if ( NULL == pRobot )
    { return; }

    if ( m_pRoboAxes == NULL )
    {
        m_pRoboAxes = new roboAxes(this);

        m_pRoboAxes->attachRobot( pRobot );
    }

    m_pRoboAxes->show();
}

void motionEdit::on_btnDown_clicked()
{
    post_request( msg_download_motion, motionEdit, Download );
}

void motionEdit::on_btnStart_clicked()
{
    //! connect to robot
    VRobot *pRobot = currentRobot();
    if ( NULL == pRobot )
    { return ; }

    //! download motion
    pRobot->run();
}

void motionEdit::on_btnStop_clicked()
{
    //! connect to robot
    VRobot *pRobot = currentRobot();
    if ( NULL == pRobot )
    { return; }

    //! download motion
     pRobot->stop();
}

//! compile
void motionEdit::on_toolButton_clicked()
{
    preCompileTrace();

    int ret;
    ret = compileTrace();

    postCompileTrace( ret );

}

void motionEdit::on_btnPref_clicked()
{
    MotionPref dlg(this);

    dlg.setModal( true );
    dlg.setModel( &mMotionPref );

    dlg.exec();
}

void motionEdit::on_btnExport_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( ("CSV(*.csv)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    QFile fileCsv( fDlg.selectedFiles().first() );
    if ( fileCsv.open( QIODevice::WriteOnly ) )
    {}
    else
    { return; }

    QTextStream outStream( &fileCsv );

    //! switch type
    if ( ui->cmbExport->currentIndex() == 0 )       //! joints
    {
        //! title
        outStream<<"t,p1,v1,p2,v2,p3,v3,p4,v4"<<"\n";

        //! items
        for ( int i = 0; i < mJointsPlan.size(); i++ )
        {
            //! t
            outStream<<QString::number( mJointsPlan.data()[i].t )<<",";

            for ( int j = 0; j < 4; j++ )
            {
                outStream<<QString::number( mJointsPlan.data()[i].p[j] )<<",";
                outStream<<QString::number( mJointsPlan.data()[i].v[j] )<<",";
            }

            outStream<<"\n";
        }
    }
    else if ( ui->cmbExport->currentIndex() == 1 )  //! hand
    {
        //! title
        outStream<<"t,p"<<"\n";

        for ( int i = 0; i < mHandTpvGroup.mItems.size(); i++ )
        {
            outStream<<mHandTpvGroup[i]->getT()<<","<<mHandTpvGroup[i]->getP()<<"\n";
        }
    }
    else if ( ui->cmbExport->currentIndex() == 2 )  //! xyz
    {
        //! title
        outStream<<"t,x,y,z"<<"\n";

        //! items
        for ( int i = 0; i < mTracePlan.size(); i++ )
        {
            //! t
            outStream<<QString::number( mTracePlan.data()[i].t )<<",";

            outStream<<QString::number( mTracePlan.data()[i].x )<<",";
            outStream<<QString::number( mTracePlan.data()[i].y )<<",";
            outStream<<QString::number( mTracePlan.data()[i].z )<<",";

            outStream<<"\n";
        }
    }
    else
    {}

    fileCsv.close();

}
