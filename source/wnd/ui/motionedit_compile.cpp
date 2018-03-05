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

    //! pre build
    if ( NULL != m_pJointsTrace )
    {
        delete []m_pJointsTrace;
        m_pJointsTrace = NULL;
    }

    if ( NULL != m_pTracePoint )
    {
        delete []m_pTracePoint;
        m_pTracePoint = NULL;
    }

    return 0;
}

int motionEdit::postCompileTrace( int ret )
{
    //! fail clean the resource
    if ( ret != 0 )
    {
        mHandTpvGroup.clear();

        //! pre build
        if ( NULL != m_pJointsTrace )
        {
            delete []m_pJointsTrace;
            m_pJointsTrace = NULL;
        }

        if ( NULL != m_pTracePoint )
        {
            delete []m_pTracePoint;
            m_pTracePoint = NULL;
        }
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
        sysError( "invalid robot name",  m_pmcModel->mConn.getRoboName() );
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
        sysError( "less than 2 points" );
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
        sysError( "invalid robot", pRobot->name() );
        return ERR_INVALID_ROBOT_NAME;
    }

    handActionModel *pHandModel = (handActionModel*)pModel;
    if ( NULL == pHandModel )
    {
        sysError( "invalid robot", pRobot->name() );
        return ERR_INVALID_ROBOT_NAME;
    }

    QMap< QString, double > actionMap;
    actionMap = pHandModel->actionMap();
    if ( actionMap.size() < 1 )
    {
        sysError( "invalid hand action" );
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
            sysWarn( QString("invalid cmd at line %1").arg(id) );
        }

        if ( !actionMap.contains( pItem->getCmd() ) )
        {
            ret = ERR_INVALID_TPV_CMD;
            sysWarn( QString("invalid cmd at line %1").arg(id) );
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
    int dataSize = mMotionGroup->mItems.size();

    //! data cache
    endPoint *pEndPoints = new endPoint[ dataSize ];
    if ( NULL == pEndPoints )
    { return ERR_ALLOC_FAIL; }
    memset( pEndPoints, 1, sizeof(endPoint)*dataSize );     //! interpd by all items

    int count = mMotionGroup->fetech( &pEndPoints->datas + offsetof_double(endPoint,t), sizeof_double(endPoint),
                                      &pEndPoints->datas + offsetof_double(endPoint,x), sizeof_double(endPoint),
                                      &pEndPoints->datas + offsetof_double(endPoint,y), sizeof_double(endPoint),
                                      &pEndPoints->datas + offsetof_double(endPoint,z), sizeof_double(endPoint),
                                      dataSize );
    if ( count != dataSize )
    {
        delete []pEndPoints;
        return ERR_FETCH_FAIL;
    }

    //! trace plan
    int xyzResLen;
    int ret = ns_pathplan::GetPvtLen( &pEndPoints->datas,
                                      count,
                                      ui->spinStep->value(),
                                      ui->cmbInterp->currentIndex(),
                                      &xyzResLen );
    if ( ret != 0 )
    {
        delete []pEndPoints;
        return ERR_FETCH_FAIL;
    }
    delete []pEndPoints;

    //! trace size
    mTracePointSize = xyzResLen * sizeof(double) / sizeof(tracePoint);
    if ( mTracePointSize > 1 )
    { }
    else
    { return ERR_PLAN_FAIL; }
    m_pTracePoint = new tracePoint[ mTracePointSize ];
    if ( NULL == m_pTracePoint )
    { return ERR_ALLOC_FAIL; }

    ret = ns_pathplan::GetPvtInfo( &m_pTracePoint->datas, xyzResLen );
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
    jointsAngle angle = {
                            DEG_TO_RAD( pRobot->mRefAngles.at(0) ),
                            DEG_TO_RAD( pRobot->mRefAngles.at(1) ),
                            DEG_TO_RAD( pRobot->mRefAngles.at(2) ),
                            DEG_TO_RAD( pRobot->mRefAngles.at(3) ),
                            };

    m_pJointsTrace = new jointsTrace[ mTracePointSize ];
    if ( NULL == m_pJointsTrace )
    {
        delete []m_pTracePoint;
        m_pTracePoint = NULL;
        return ERR_ALLOC_FAIL;
    }

    //! split
    ret = ns_kinematic::GetArmPosition( (double*)(&angle),

                    &m_pTracePoint->datas + offsetof_double( tracePoint, x ), sizeof_double(tracePoint),
                    &m_pTracePoint->datas + offsetof_double( tracePoint, vx ),sizeof_double(tracePoint),
                    &m_pTracePoint->datas + offsetof_double( tracePoint, t ), sizeof_double(tracePoint),

                    mTracePointSize,
                    &m_pJointsTrace->datas
                    );
    if ( ret <= 0 )
    {
        mJointsTraceSize = 0;
        delete []m_pJointsTrace;
        m_pJointsTrace = NULL;

        delete []m_pTracePoint;
        m_pTracePoint = NULL;

        return ERR_PLAN_SPLIT_FAIL;
    }
    else
    {
        mJointsTraceSize = ret;

        //! rotate by cal
        jointsRotate( m_pJointsTrace, mJointsTraceSize );

        return 0;
    }
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
