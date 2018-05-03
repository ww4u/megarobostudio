#include "motionedit.h"
#include "ui_motionedit.h"

int motionEdit::compileTrace()
{
    int ret;

    ret = checkRobot();logDbg();
    if ( ret != 0 )
    { return ret; }

    ret = currentRobot()->build( m_pMotionGroup,
                                 mTracePlan,
                                 mJointsPlan,
                                 mJointsTpvGroup
                                 );
    if ( ret != 0 )
    { return ret; }

    return ret;
}

int motionEdit::preCompileTrace()
{
    delete_all( mJointsTpvGroup );
//    mJointsTpvGroup.clear();

    mTracePlan.clear();
    mJointsPlan.clear();

    return 0;
}

int motionEdit::postCompileTrace( int ret )
{
    //! fail clean the resource
    if ( ret != 0 )
    {
        delete_all( mJointsTpvGroup );

        mTracePlan.clear();
        mJointsPlan.clear();
        sysError( tr("Build Fail" ) );
    }
    else
    {
        sysLog( tr("Build success"), QString::number( mTracePlan.size()), QString::number(mJointsPlan.size()) );
    }

    emit sig_joints_trace_changed();

    return 0;
}

int motionEdit::checkRobot()
{
    VRobot *pRobo;

    //! check robo
    pRobo = currentRobot();
    if ( NULL == pRobo )
    {
        sysError( tr("invalid robot name"),  m_pmcModel->mConn.getRoboName() );
        return ERR_INVALID_ROBOT_NAME;
    }

    //! model match
    Q_ASSERT( NULL != m_pMotionGroup );
    if ( pRobo->getClass().compare( m_pMotionGroup->className(), Qt::CaseInsensitive ) != 0 )
    {
        sysError( tr("robo do not match"), pRobo->getClass(), m_pMotionGroup->className() );
        return ERR_INVALID_ROBOT_NAME;
    }

    return 0;
}

//int motionEdit::checkTrace()
//{
//    int ret1;
//    ret1 = checkJointsTrace();
//    if ( ret1 != 0 )
//    { return ret1; }

//    return 0;
//}
//int motionEdit::checkJointsTrace()
//{
////    Q_ASSERT( NULL != m_pMotionGroup );

////    if ( m_pMotionGroup->verify() )
////    {}
////    else
////    { return ERR_INVALID_TPV_T; }

//    return 0;
//}

//int motionEdit::buildTrace()
//{
//    int ret1;
//    ret1 = buildJointsTrace();logDbg();

//    int ret2;
//    ret2 = buildHandTrace();logDbg();

//    if ( ret1 != 0 )
//    { return ret1; }
//    else if ( ret2 != 0 )
//    { return ret2; }
//    else
//    { return 0; }
//}

//int motionEdit::buildJointsTrace()
//{
//    int ret;
//logDbg();
//    ret = planJointsTrace();logDbg();
//    if ( ret != 0 )
//    { return ret; }
//logDbg();
//    ret = splitJointsTrace();logDbg();
//    if ( ret != 0 )
//    { return ret; }

//    return ret;
//}

////! x,y,z --> sub x,y,z
//int  motionEdit::planJointsTrace()
//{
////    //! fetch
////    Q_ASSERT( NULL != m_pMotionGroup );
////    int dataSize = m_pMotionGroup->mItems.size();
////logDbg()<<dataSize;
////    xxxGroup<endPoint> endPoints;

////    if (  0 != endPoints.alloc( dataSize ) )
////    { return ERR_ALLOC_FAIL; }

////    //! fill 0
////    memset( endPoints.data(), 0, sizeof(endPoints)*dataSize );

////    //! interp fill
////    for ( int i = 0; i < dataSize; i++ )
////    { endPoints.data()[i].flagInterp = 1; }

////    double *pBase;
////    pBase = &endPoints.data()->datas;
////    int count = mMotionGroup->fetech( pBase + offsetof_double(endPoint,t), sizeof_double(endPoint),
////                                      pBase + offsetof_double(endPoint,x), sizeof_double(endPoint),
////                                      pBase + offsetof_double(endPoint,y), sizeof_double(endPoint),
////                                      pBase + offsetof_double(endPoint,z), sizeof_double(endPoint),
////                                      dataSize );
////    if ( count != dataSize )
////    { return ERR_FETCH_FAIL; }
////for ( int i = 0; i < dataSize; i++ )
////{
////    logDbg()<<endPoints.data()[i].t<<endPoints.data()[i].x<<endPoints.data()[i].y<<endPoints.data()[i].z<<endPoints.data()[i].flagInterp;
////}
////    //! trace plan
////    int xyzResLen;
////    int ret = ns_pathplan::GetPvtLen( pBase,
////                                      count,
////                                      ui->spinStep->value(),
////                                      ui->cmbInterp->currentIndex(),
////                                      &xyzResLen );
////    if ( ret != 0 )
////    { return ERR_PLAN_FAIL; }

////    //! trace size
////    int traceSize;
////    traceSize = xyzResLen * sizeof(double) / sizeof(tracePoint);
////    if ( traceSize > 1 )
////    { }
////    else
////    { return ERR_PLAN_FAIL; }

////    mTracePlan.clear();
////    if ( 0 != mTracePlan.alloc( traceSize ) )
////    { return ERR_ALLOC_FAIL; }
////logDbg()<<traceSize;
////    ret = ns_pathplan::GetPvtInfo( &mTracePlan.data()->datas, xyzResLen );
////    if ( ret != 0 )
////    { return ERR_PLAN_FAIL; }

//    return 0;
//}

////! x,y,z -> joint t,p,v
//int  motionEdit::splitJointsTrace()
//{
//    int ret;

//    //! trace split
//    //! arm tune
//    VRobot *pRobot;
//    pRobot = currentRobot();logDbg()<<pRobot->mRefAngles.size();

//    mJointsPlan.clear();
//    if ( 0 != mJointsPlan.alloc( mTracePlan.size() ) )
//    {
//        return ERR_ALLOC_FAIL;
//    }

//    //! init angle
//    //! { 0, 90, -90, -90 };
//    jointsAngle refAngle={ pRobot->mRefAngles.at(0),
//                           pRobot->mRefAngles.at(1),
//                           pRobot->mRefAngles.at(2),
//                           pRobot->mRefAngles.at(3) };
//    jointsAngle convertAngle={ 0, 90, 180, 180 };
//    double armLength[]={ 247.75, 255, 250, 0, 0, 0 };
//logDbg()<<pRobot->mRefAngles.at(0)<<pRobot->mRefAngles.at(1)<<pRobot->mRefAngles.at(2)<<pRobot->mRefAngles.at(3);
//    //! split
//    int nRes;
//    ret = ns_kinematic::getArmPosition_Size(

//                    armLength,sizeof_array(armLength),
//                    convertAngle.angles,
//                    refAngle.angles,
//                    4,

//                    &mTracePlan.data()->datas + offsetof_double( tracePoint, x ), sizeof_double(tracePoint),
//                    &mTracePlan.data()->datas + offsetof_double( tracePoint, vx ),sizeof_double(tracePoint),
//                    &mTracePlan.data()->datas + offsetof_double( tracePoint, t ), sizeof_double(tracePoint),

//                    mTracePlan.size(),

//                    &nRes
//                    );
//    if ( ret == 0 && nRes > 0 )
//    {}
//    else
//    { return ERR_PLAN_SLOVE_FAIL; }

//    //! get data
//    if ( 0 != mJointsPlan.alloc( nRes ) )
//    { return ERR_ALLOC_FAIL; }

//    ret = ns_kinematic::getArmPosition_Data(

//                armLength,sizeof_array(armLength),
//                convertAngle.angles,
//                refAngle.angles,
//                 4,

//                &mTracePlan.data()->datas + offsetof_double( tracePoint, x ), sizeof_double(tracePoint),
//                &mTracePlan.data()->datas + offsetof_double( tracePoint, vx ),sizeof_double(tracePoint),
//                &mTracePlan.data()->datas + offsetof_double( tracePoint, t ), sizeof_double(tracePoint),

//                mTracePlan.size(),

//                &mJointsPlan.data()->datas,
//                mJointsPlan.size()
//                );
//    if ( ret != 0 )
//    { return ERR_PLAN_SLOVE_FAIL; }

//    return 0;
//}

//int motionEdit::buildHandTrace()
//{

////    //! vars have checked before
////    QAbstractTableModel *pModel = currentRobot()->handActions();
////    handActionModel *pHandActionModel = (handActionModel*)pModel;
////    QMap<QString, double> actionMap = pHandActionModel->actionMap();

////    double handVal;
////    int ret;
////    foreach( SinanjuMotionItem * pItem, mMotionGroup->mItems )
////    {
////        Q_ASSERT( NULL != pItem );
////        handVal = actionMap[ pItem->getCmd() ];

////        ret = mHandTpvGroup.addItem(
////                    pItem->getT(),
////                    handVal,
////                    0
////                    );
////        if ( ret != 0 )
////        { return ret; }
////    }

//    return 0;
//}
