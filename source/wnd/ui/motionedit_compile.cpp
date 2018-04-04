#include "motionedit.h"
#include "ui_motionedit.h"

#include "../../arith/pathplan/pathplan.h"
#include "../../arith/kinematic/kinematic.h"

int motionEdit::compileTrace()
{
    int ret;

    ret = checkRobot();logDbg();
    if ( ret != 0 )
    { return ret; }

    ret = checkTrace();logDbg();
    if ( ret != 0 )
    { return ret; }

    ret = buildTrace();logDbg();
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int motionEdit::preCompileTrace()
{
    mHandTpvGroup.clear();

    mTracePlan.clear();
    mJointsPlan.clear();

    return 0;
}

int motionEdit::postCompileTrace( int ret )
{
    //! fail clean the resource
    if ( ret != 0 )
    {
        mHandTpvGroup.clear();

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

    pRobo = currentRobot();
    if ( NULL == pRobo )
    {
        sysError( tr("invalid robot name"),  m_pmcModel->mConn.getRoboName() );
        return ERR_INVALID_ROBOT_NAME;
    }

    return 0;
}

int motionEdit::checkTrace()
{
    int ret1;
    ret1 = checkJointsTrace();

    int ret2;
    ret2 = checkHandTrace();

    if ( ret1 != 0 )
    { return ret1; }
    else if ( ret2 != 0 )
    { return ret2; }
    else
    { return 0; }

    return 0;
}
int motionEdit::checkJointsTrace()
{
    Q_ASSERT( NULL != mMotionGroup );

    //! > 1 points
    if ( mMotionGroup->mItems.size() < 2 )
    {
        sysError( tr("less than 2 points") );
        return ERR_INVALID_TPV_COUNT;
    }

    //! check t && x,y,z
    int ret = 0;
    int id = 0;
    float tPre;
    tPre = mMotionGroup->mItems[0]->getT();
    foreach( motionItem *pItem, mMotionGroup->mItems )
    {
        Q_ASSERT( NULL != pItem );

        if ( pItem->getT() < 0 )
        {
            ret = ERR_INVALID_TPV_T;
            sysError( QString("invalid time %1 at line %2").arg(pItem->getT()).arg(id)  );
        }

        if ( pItem->getT() < tPre )
        {
            ret = ERR_INVALID_TPV_T;
            sysError( QString("invalid time %1 at line %2").arg(pItem->getT()).arg(id)  );
        }
        else
        { tPre = pItem->getT(); }

        id++;
    }

    return ret;
}
int motionEdit::checkHandTrace()
{
    VRobot *pRobot = currentRobot();
    if ( NULL == pRobot )
    { return ERR_INVALID_ROBOT_NAME; }

    QAbstractTableModel *pModel = pRobot->handActions();
    if ( NULL == pModel )
    {
        sysError( tr("invalid robot"), pRobot->name() );
        return ERR_INVALID_ROBOT_NAME;
    }

    handActionModel *pHandModel = (handActionModel*)pModel;
    if ( NULL == pHandModel )
    {
        sysError( tr("invalid robot"), pRobot->name() );
        return ERR_INVALID_ROBOT_NAME;
    }

    QMap< QString, double > actionMap;
    actionMap = pHandModel->actionMap();
    if ( actionMap.size() < 1 )
    {
        sysError( tr("invalid hand action") );
        return ERR_INVALID_ROBOT_NAME;
    }

    //! check invalid action
    int ret = 0;
    int id = 0;
    foreach( motionItem *pItem, mMotionGroup->mItems )
    {
        if ( pItem->getCmd().isEmpty() )
        {
            ret = ERR_INVALID_TPV_CMD;
            sysWarn( QString( tr("invalid cmd at line %1") ).arg(id) );
        }

        if ( !actionMap.contains( pItem->getCmd() ) )
        {
            ret = ERR_INVALID_TPV_CMD;
            sysWarn( QString( tr("invalid cmd at line %1") ).arg(id) );
        }

        id++;
    }

    return ret;
}

int motionEdit::buildTrace()
{
    int ret1;
    ret1 = buildJointsTrace();logDbg();

    int ret2;
    ret2 = buildHandTrace();logDbg();

    if ( ret1 != 0 )
    { return ret1; }
    else if ( ret2 != 0 )
    { return ret2; }
    else
    { return 0; }
}

int motionEdit::buildJointsTrace()
{
    int ret;
logDbg();
    ret = planJointsTrace();logDbg();
    if ( ret != 0 )
    { return ret; }
logDbg();
    ret = splitJointsTrace();logDbg();
    if ( ret != 0 )
    { return ret; }

    return ret;
}

//! x,y,z --> sub x,y,z
int  motionEdit::planJointsTrace()
{
    //! fetch
    int dataSize = mMotionGroup->mItems.size();
logDbg()<<dataSize;
    xxxGroup<endPoint> endPoints;

    if (  0 != endPoints.alloc( dataSize ) )
    { return ERR_ALLOC_FAIL; }

    //! fill 0
    memset( endPoints.data(), 0, sizeof(endPoints)*dataSize );

    //! interp fill
    for ( int i = 0; i < dataSize; i++ )
    { endPoints.data()[i].flagInterp = 1; }

    double *pBase;
    pBase = &endPoints.data()->datas;
    int count = mMotionGroup->fetech( pBase + offsetof_double(endPoint,t), sizeof_double(endPoint),
                                      pBase + offsetof_double(endPoint,x), sizeof_double(endPoint),
                                      pBase + offsetof_double(endPoint,y), sizeof_double(endPoint),
                                      pBase + offsetof_double(endPoint,z), sizeof_double(endPoint),
                                      dataSize );
    if ( count != dataSize )
    { return ERR_FETCH_FAIL; }
for ( int i = 0; i < dataSize; i++ )
{
    logDbg()<<endPoints.data()[i].t<<endPoints.data()[i].x<<endPoints.data()[i].y<<endPoints.data()[i].z<<endPoints.data()[i].flagInterp;
}
    //! trace plan
    int xyzResLen;
    int ret = ns_pathplan::GetPvtLen( pBase,
                                      count,
                                      ui->spinStep->value(),
                                      ui->cmbInterp->currentIndex(),
                                      &xyzResLen );
    if ( ret != 0 )
    { return ERR_PLAN_FAIL; }

    //! trace size
    int traceSize;
    traceSize = xyzResLen * sizeof(double) / sizeof(tracePoint);
    if ( traceSize > 1 )
    { }
    else
    { return ERR_PLAN_FAIL; }

    mTracePlan.clear();
    if ( 0 != mTracePlan.alloc( traceSize ) )
    { return ERR_ALLOC_FAIL; }
logDbg()<<traceSize;
    ret = ns_pathplan::GetPvtInfo( &mTracePlan.data()->datas, xyzResLen );
    if ( ret != 0 )
    { return ERR_PLAN_FAIL; }

    return 0;
}

//! x,y,z -> joint t,p,v
int  motionEdit::splitJointsTrace()
{
    int ret;

    //! trace split
    //! arm tune
    VRobot *pRobot;
    pRobot = currentRobot();logDbg()<<pRobot->mRefAngles.size();

    mJointsPlan.clear();
    if ( 0 != mJointsPlan.alloc( mTracePlan.size() ) )
    {
        return ERR_ALLOC_FAIL;
    }

    //! init angle
    //! { 0, 90, -90, -90 };
    jointsAngle refAngle={ pRobot->mRefAngles.at(0),
                           pRobot->mRefAngles.at(1),
                           pRobot->mRefAngles.at(2),
                           pRobot->mRefAngles.at(3) };
    jointsAngle convertAngle={ 0, 90, 180, 180 };
    double armLength[]={ 247.75, 255, 250, 0, 0, 0 };
logDbg()<<pRobot->mRefAngles.at(0)<<pRobot->mRefAngles.at(1)<<pRobot->mRefAngles.at(2)<<pRobot->mRefAngles.at(3);
    //! split
    int nRes;
    ret = ns_kinematic::getArmPosition_Size(

                    armLength,sizeof_array(armLength),
                    convertAngle.angles,
                    refAngle.angles,
                    4,

                    &mTracePlan.data()->datas + offsetof_double( tracePoint, x ), sizeof_double(tracePoint),
                    &mTracePlan.data()->datas + offsetof_double( tracePoint, vx ),sizeof_double(tracePoint),
                    &mTracePlan.data()->datas + offsetof_double( tracePoint, t ), sizeof_double(tracePoint),

                    mTracePlan.size(),

                    &nRes
                    );
    if ( ret == 0 && nRes > 0 )
    {}
    else
    { return ERR_PLAN_SLOVE_FAIL; }

    //! get data
    if ( 0 != mJointsPlan.alloc( nRes ) )
    { return ERR_ALLOC_FAIL; }

    ret = ns_kinematic::getArmPosition_Data(

                armLength,sizeof_array(armLength),
                convertAngle.angles,
                refAngle.angles,
                 4,

                &mTracePlan.data()->datas + offsetof_double( tracePoint, x ), sizeof_double(tracePoint),
                &mTracePlan.data()->datas + offsetof_double( tracePoint, vx ),sizeof_double(tracePoint),
                &mTracePlan.data()->datas + offsetof_double( tracePoint, t ), sizeof_double(tracePoint),

                mTracePlan.size(),

                &mJointsPlan.data()->datas,
                mJointsPlan.size()
                );
    if ( ret != 0 )
    { return ERR_PLAN_SLOVE_FAIL; }

    return 0;
}

int motionEdit::buildHandTrace()
{
    //! vars have checked before
    QAbstractTableModel *pModel = currentRobot()->handActions();
    handActionModel *pHandActionModel = (handActionModel*)pModel;
    QMap<QString, double> actionMap = pHandActionModel->actionMap();

    double handVal;
    int ret;
    foreach( motionItem * pItem, mMotionGroup->mItems )
    {
        Q_ASSERT( NULL != pItem );
        handVal = actionMap[ pItem->getCmd() ];

        ret = mHandTpvGroup.addItem(
                    pItem->getT(),
                    handVal,
                    0
                    );
        if ( ret != 0 )
        { return ret; }
    }

    return 0;
}
