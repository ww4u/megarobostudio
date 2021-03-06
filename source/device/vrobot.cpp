#include "vrobot.h"

#include "../inst/instmgr.h"

#include "../model/tpvgroup.h"

#include "../device/mrq/deviceMRQ.h"
#include "../com/robomsg.h"

double VRobot::_mTBase = 1.0;
double VRobot::_mPBase = 1.0;
double VRobot::_mVBase = 1.0;

QString VRobot::_tempPath;

SysPara * VRobot::_mSysPara = NULL;

void VRobot::setTpvBase( double tBase,
                double pBase,
                double vBase )
{
    VRobot::_mTBase = tBase;
    VRobot::_mPBase = pBase;
    VRobot::_mVBase = vBase;
}

void VRobot::setTempPath( const QString &tempPath )
{ _tempPath = tempPath; }
QString VRobot::tempPath()
{ return _tempPath; }

void VRobot::attachSysPara( SysPara *pSysPara )
{
    Q_ASSERT( NULL != pSysPara );
    VRobot::_mSysPara = pSysPara;
}

//! true -- need move
bool VRobot::motionPredict( float t, float angle )
{
    Q_ASSERT( NULL != _mSysPara );

    do
    {
        //! no need to rotate
        if ( qAbs(angle) <= _mSysPara->mAngleResolution )
        { return false; }
    }while( 0 );

    return true;
}

VRobot::VRobot()
{
    mClass = "Unk robot";
    mId = robot_unk;

    mCoord = robo_coord_body;
    mbPwr = true;
                                        //! configs
    mAxes = 0;                          //! axes count
    mRegions = 10;                      //! region count for each axes

    mDCAxes = 0;

    mDOs = 0;
    mDIs = 0;
    mISOs = 0;
    mISIs = 0;
    mAINs = 0;
    mMosos = 0;

    mOutputs = 0;
    mInputs = 0;

    mTemperatures = 0;
    mUARTs = 0;
    mUART_Sensors = 0;

    mAbsEncoderAlarms = 0;
    mDistanceAlarms = 0;
    mAlarms = 0;

    //! config
    setTrigSrcs( 2 );

    mPoseCount = 0;
    mAngleType = robo_angle_none;

    mbInterpAble = false;
    mbEncoderAble = true;
    mbRunWaveAble = false;
    mbTunningAble = false;

    mbCoordAble = false;
    mbJointPanelAble = false;
    mbRoboPanelAble = false;
    mbMrpAble = false;

    mDriverId = 0;

    mMicrostepsList<<"256"<<"128"<<"64"<<"32"<<"16"<<"8"<<"4"<<"2"<<"1";
    mMicrostepBase = 0;

    m_pInstMgr = NULL;

    mCanGroupId  = group_id_from;       //! \see modelSysPref

    mSubGroup = 0;
    mSubGroupId = sub_group_id_from;

    mZeroSpeed = 5;
    mZeroTmo = time_s( 60 );
    mZeroTick = time_ms( 500 );

    mBaseCompensation = 0;
    mLengthUnit = 1;
                                        //! reimplent the worker thread
    m_pAxesWorkers = NULL;              //! \note if override the thread , delete the default at first
    m_pRoboWoker = new RoboWorker( this );
    Q_ASSERT( NULL != m_pRoboWoker );

//    m_pRoboTask = new RoboTask();
//    Q_ASSERT( NULL != m_pRoboTask );
    m_pRoboTask = NULL;
                                        //! device
    setType( mcModelObj::model_device );
}

VRobot::~VRobot()
{
    gcWorker();

    m_pRoboWoker->requestInterruption();
    m_pRoboWoker->wait();
    delete m_pRoboWoker;

    if ( NULL != m_pRoboTask )
    {
        if ( m_pRoboTask->isRunning() )
        {
            m_pRoboTask->requestInterruption();
            m_pRoboTask->wait();
        }

        delete m_pRoboTask;
    }
}

void VRobot::postCtor()
{

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
                m_pAxesWorkers[i].requestInterruption();
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

void VRobot::setStatus( int stat, const tpvRegion &region, frameData &data )
{
    mRobotStatus[ region ] = stat;
    Q_ASSERT( mRobotStatus.contains( region ) );
}

void VRobot::assignStatus( int stat, const tpvRegion &region )
{
    mRobotStatus[ region ] = stat;
    Q_ASSERT( mRobotStatus.contains( region ) );
}

int VRobot::status( const tpvRegion &region )
{
    if ( mRobotStatus.contains( region ) )
    { return mRobotStatus[ region ]; }
    else
    { return MRQ_MOTION_STATE_2_POWERON; }
}

int VRobot::state( const tpvRegion &region, int inTask )
{ return 0; }

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

//! CH1@name
MegaDevice::deviceMRQ *VRobot::findDevice( const QString &name,
                                   int *pAxes )
{
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
    mAxesConnectionName.clear();
    for ( int i = 0; i < axesCnt; i++ )
    {
        mAxesConnectionName<<QString("CH%1@device1").arg( i+1 );
    }
}

void VRobot::setJointName( int axesCnt )
{
    mJointName.clear();
    for ( int i = 0; i < axesCnt; i++ )
    {
        mJointName<<QString("Joint:%1").arg( i+1 );
    }
}

VRobot * VRobot::subRobot( int index, int *pAxes )
{
    Q_ASSERT( NULL != m_pInstMgr );
    Q_ASSERT( NULL != pAxes );

    *pAxes = 0;

    return m_pInstMgr->findRobot( mAxesConnectionName.at(index) );
}

int VRobot::checkRoboTask()
{
    Q_ASSERT( NULL != m_pRoboTask );
    if ( m_pRoboTask->isRunning() )
    { return -1; }
    else
    { return 0; }
}
