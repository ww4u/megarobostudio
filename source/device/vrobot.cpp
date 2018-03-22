#include "vrobot.h"

#include "../../inst/instmgr.h"

#include "../../model/tpvgroup.h"
#include "../../model/motiongroup.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../com/robomsg.h"

VRobot::VRobot()
{
    mClass = "Unk robot";
    mId = robot_unk;
    mAxes = 0;
    mPages = 10;

    mDOs = 0;
    mDIs = 0;
    mISOs = 0;
    mISIs = 0;
    mAINs = 0;
    mUARTs = 0;

    m_pInstMgr = NULL;

    mCanGroupId  = group_id_from;       //! \see modelSysPref

    mSubGroup = 0;
    mSubGroupId = sub_group_id_from;

    mTBase = 1.0f;
    mPBase = 1.0f;
    mVBase = 1.0f;

    mBaseCompensation = 0;
    mLengthUnit = 1;
                                        //! reimplent the worker thread
    m_pAxesWorkers = NULL;              //! \note if override the thread , delete the default at first
    m_pRoboWoker = new RoboWorker( this );
    Q_ASSERT( NULL != m_pRoboWoker );
}

VRobot::~VRobot()
{
    gcWorker();

    delete m_pRoboWoker;
}

void VRobot::gcWorker()
{
    //! gc
    if ( NULL != m_pAxesWorkers )
    {
        for( int i = 0; i < axes(); i++ )
        {
            if ( m_pAxesWorkers[i].isRunning() )
            {
                m_pAxesWorkers[i].terminate();
            }
        }
        for( int i = 0; i < axes(); i++ )
        {
            m_pAxesWorkers[i].wait();
        }

        delete []m_pAxesWorkers;
        m_pAxesWorkers = NULL;
    }
}

QAbstractTableModel *VRobot::handActions()
{ return NULL; }

void VRobot::setStatus( int stat,int ch, int page )
{
    mRobotStatus[ ch ] = stat;
    Q_ASSERT( mRobotStatus.contains(ch) );
logDbg()<<ch<<stat;
    sysQueue()->postMsg(
                      e_axes_status,
                      name(),
                      ch,
                      stat
                      );
}
int VRobot::getStatus( int ch )
{
    if ( mRobotStatus.contains( ch ) )
    { return mRobotStatus[ ch ]; }
    else
    { return MRQ_MOTION_STATE_2_POWERON; }
}
int VRobot::status( int ch )
{ return getStatus( ch ); }

void VRobot::setInstMgr( MegaDevice::InstMgr *pMgr )
{
    Q_ASSERT( NULL != pMgr );

    m_pInstMgr = pMgr;
}
MegaDevice::InstMgr *VRobot::getInstMgr()
{
    return m_pInstMgr;
}
MegaDevice::InstMgr *VRobot::instMgr()
{
    return m_pInstMgr;
}

void VRobot::setSubGroup( int sub )
{ mSubGroup = sub; }
int VRobot::getSubGroup()
{ return mSubGroup; }
int VRobot::subGroup()
{ return mSubGroup; }

void VRobot::setSubGroupId( int id )
{ mSubGroupId = id; }
int VRobot::getSubGroupId()
{ return mSubGroupId; }
int VRobot::subGroupId()
{ return mSubGroupId; }

void VRobot::setTPVUnit( float t, float p, float v )
{
    mTBase = t;
    mPBase = p;
    mVBase = v;
}

//! CH1@name
MegaDevice::deviceMRQ *VRobot::findDevice( const QString &name,
                                   int *pAxes )
{
//    *pAxes = -1;
////logDbg()<<name;
//    QStringList strList = name.split("@", QString::SkipEmptyParts );

//    if ( strList.length() < 2 )
//    { return NULL; }

//    //! try convert the ch id
//    if ( strList[0].startsWith( "CH" ) )
//    {}
//    else
//    { return NULL; }

//    QString numStr = strList[0].right( strList[0].length() - 2 );/*logDbg()<<numStr;*/
//    bool bOk;
//    int axesId = numStr.toInt( &bOk );
//    if( !bOk )
//    { return NULL; }

//    if ( axesId < 1 )
//    { return NULL; }

//    *pAxes = axesId - 1;
    Q_ASSERT( NULL != m_pInstMgr );
    return m_pInstMgr->findDevice( name, pAxes );
}

MegaDevice::deviceMRQ *VRobot::jointDevice( int jointId,
                                    int *pAxes )
{
    MegaDevice::deviceMRQ *pMrq;

    Q_ASSERT( jointId >=0 && jointId < mAxesConnectionName.size() );

    pMrq = findDevice( mAxesConnectionName[ jointId ], pAxes );

    return pMrq;
}

void VRobot::setAxesConnectionName( const QStringList &names )
{ mAxesConnectionName = names; }
QStringList VRobot::axesConnectionName()
{ return mAxesConnectionName; }
//! ch@deviceName
QStringList VRobot::axesDeviceName()
{
    QStringList axesNames;
    QStringList splitName;
    for ( int i = 0; i < mAxesConnectionName.size(); i++ )
    {
        splitName = mAxesConnectionName[i].split( "@", QString::SkipEmptyParts );
        Q_ASSERT( splitName.size() > 1 );
        axesNames<<splitName[1];
    }

    return axesNames;
}

void VRobot::setAxesDefName( int axesCnt )
{
    for ( int i = 0; i < axesCnt; i++ )
    {
        mAxesConnectionName<<QString("CH%1@device1").arg( i+1 );
    }
}

void VRobot::setJointName( int axesCnt )
{
    for ( int i = 0; i < axesCnt; i++ )
    {
        mJointName<<QString("Joint:%1").arg( i+1 );
    }
}

VRobot * VRobot::subRobot( int index, int *pAxes )
{
    Q_ASSERT( NULL != m_pInstMgr );
    Q_ASSERT( NULL != pAxes );

    //! \todo
    *pAxes = 0;

    return m_pInstMgr->findRobot( mAxesConnectionName.at(index) );
}
