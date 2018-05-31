#include "deviceMRQ.h"

#include "scpi/scpi.h"


#define DEF_TPV_CAP             256
#define DEF_WAV_BUF_SIZE        (125*1024)      //! bytes

#define ABS_ANGLE_TO_DEG( angle )   (360.0f*(angle))/((1<<18)-1)
#define INC_ANGLE_TO_DEG( angle )   (360.0f*(angle))/(1<<18)

#define VALUE_TO_ABS_ANGLE( val )   (quint32)( (val) * ((1<<18)-1) / 360.0f )
#define VALUE_TO_INC_ANGLE( val )   (quint32)( (val) * ((1<<18)) / 360.0f )
#include "../../bus/canbus.h"
namespace MegaDevice
{

float deviceMRQ::absAngleToValue( quint32 angle )
{ return ABS_ANGLE_TO_DEG( angle ); }
quint32 deviceMRQ::valueToAbsAngle( float val )
{ return VALUE_TO_ABS_ANGLE(val); }

float deviceMRQ::incAngleToValue( quint32 angle )
{ return INC_ANGLE_TO_DEG( angle ); }
quint32 deviceMRQ::valueToIncAngle( float val )
{ return VALUE_TO_INC_ANGLE(val); }

deviceMRQ::deviceMRQ()
{
    //! downloader in ctor

    //! fsm in post ctor

    //! proxy motor in post ctor

    //! \note only a sema for one device
//    mDownloaderSema.release( 1 );
}

deviceMRQ::~deviceMRQ()
{
    //! stop
    foreach( tpvDownloader *value, mDownloaders )
    {
        Q_ASSERT( NULL != value );
        if ( value->isRunning() )
        {
            value->terminate();
            value->wait();
        }
    }

    //! fsm
    foreach (MrqFsm * value, mMrqFsms )
    {
        Q_ASSERT( NULL != value );
        delete value;
    }
    mMrqFsms.clear();

    //! proxy motor
    foreach(deviceProxyMotor *value, mProxyMotors )
    {
        Q_ASSERT( NULL != value );
        delete value;
    }

    //! downloader
    foreach( tpvDownloader *value, mDownloaders )
    {
        Q_ASSERT( NULL != value );
        delete value;
    }

    //! tasks
    foreach( MrqTaskThread *pThread, mTaskThread )
    {
        Q_ASSERT( NULL != pThread );
        delete pThread;
    }
}

void deviceMRQ::postCtor()
{
    tpvRegion region;
    //! fsm
    MrqFsm *pFsm;
    deviceProxyMotor *pProxyMotor;
    tpvDownloader *pLoader;
    MrqTaskThread *pTask;
    for ( int i = 0; i < axes(); i++ )
    {
        for ( int j = 0; j < regions(); j++ )
        {
            region.setRegion( i, j );

            //! fsm
            pFsm = new MrqFsm();
            Q_ASSERT( NULL != pFsm );

            //! set region
            pFsm->setRegion( i, j );

            pFsm->build();
            pFsm->setContext( this );
            pFsm->setId( (UINT_PTR)this, i, j );
            mMrqFsms.insert( region, pFsm );

            //! motor proxy
            pProxyMotor = new deviceProxyMotor( this,
                                                region );
            Q_ASSERT( NULL != pProxyMotor );
            mProxyMotors.insert( region, pProxyMotor );

            //! tpv caps
            mTpvCaps.insert( region, DEF_TPV_CAP );//logDbg()<<region.axes()<<region.page();
            mTpvIndexes.insert( region, 0 );

            //! buf caps
            mTpvBufferSizes.insert( region, DEF_WAV_BUF_SIZE );//logDbg()<<region.axes()<<region.page();

            //! loader for each region
            pLoader = new tpvDownloader();
            Q_ASSERT( NULL != pLoader );
            pLoader->attachDevice( this, tpvRegion(i,j) );
            mDownloaders.insert( tpvRegion(i,j), pLoader );
        }

        //! foreach axes
        pTask = new MrqTaskThread();
        Q_ASSERT( NULL != pTask );
        mTaskThread.append( pTask );
    }

    //! \note downloader for each axes
    mDownloaderSema.release( axes() );
}

MRQ_model *deviceMRQ::getModel()
{
    return this;
}

int deviceMRQ::getREPORT_DATA_( byte val0
,MRQ_REPORT_STATE val1, quint8 * val2, bool bQuery )
{
    //! 60 4
    int ret = 0;

    uint8 lval0 = 0;
    ret = m_pBus->read( DEVICE_RECEIVE_ID, mc_REPORT, sc_REPORT_DATA_Q , val0, (byte)val1, &lval0, bQuery);
    if ( ret != 0){ log_device(); }
    if ( ret != 0) return ret;
    *val2 = lval0;
    return 0;
}

int deviceMRQ::getREPORT_DATA_( byte val0
,MRQ_REPORT_STATE val1, quint16 * val2, bool bQuery )
{
    //! 60 4
    int ret = 0;

    uint16 lval0 = 0;
    ret = m_pBus->read( DEVICE_RECEIVE_ID, mc_REPORT, sc_REPORT_DATA_Q , val0, (byte)val1, &lval0, bQuery);
    if ( ret != 0){ log_device(); }
    if ( ret != 0) return ret;
    *val2 = lval0;
    return 0;
}

int deviceMRQ::getREPORT_DATA_( byte val0
,MRQ_REPORT_STATE val1, quint32 * val2, bool bQuery )
{
    //! 60 4
    int ret = 0;

    uint32 lval0 = 0;
    ret = m_pBus->read( DEVICE_RECEIVE_ID, mc_REPORT, sc_REPORT_DATA_Q , val0, (byte)val1, &lval0, bQuery);
    if ( ret != 0){ log_device(); }
    if ( ret != 0) return ret;
    *val2 = lval0;
    return 0;
}

struct reportType
{
    MRQ_REPORT_STATE mStat;
    QMetaType::Type mType;
};

//! const special type
static reportType _report_types[]=
{
    { MRQ_REPORT_STATE_DIST, QMetaType::UShort },

};

QMetaType::Type deviceMRQ::getREPORT_TYPE( MRQ_REPORT_STATE stat )
{
    for( int i = 0; i < sizeof_array(_report_types); i++ )
    {
        if ( stat == _report_types[i].mStat )
        { return _report_types[i].mType; }
    }

    return QMetaType::UInt;
}

//!
#define mc_SEQ      0xca
#define mc_qVER     0x02
int deviceMRQ::getSeqVer( quint8 *p1, quint8 *p2, quint8 *p3, quint8 *p4, bool bQuery )
{
    int ret = 0;

    ret = m_pBus->read( DEVICE_RECEIVE_ID, mc_SEQ, mc_qVER , p1, p2, p3, p4, bQuery);
    if ( ret != 0){ log_device(); }
    if ( ret != 0) return ret;

    return 0;
}

int deviceMRQ::setFanDuty( int duty )
{
    mFanInfo.mDuty = duty;

    return m_pBus->write( DEVICE_RECEIVE_ID,
                    (byte)mc_SYSTEM,
                   (byte)sc_SYSTEM_FANPARA,
                   mFanInfo.mDuty,
                   mFanInfo.mFreq );
}
int deviceMRQ::setLedDuty( int i, int duty )
{
    Q_ASSERT( i >= 0 && i < 4 );

    mLedInfo[i].mDuty = duty;

    return m_pBus->write( DEVICE_RECEIVE_ID,
                          (byte)mc_SYSTEM,
                           (byte)sc_SYSTEM_ARMLEDPARA,
                           (byte)i,
                           mLedInfo[i].mDuty,
                           mLedInfo[i].mFreq );
}

int deviceMRQ::setFanFreq( int freq )
{
    mFanInfo.mFreq = freq;

    return m_pBus->write( DEVICE_RECEIVE_ID,
                          (byte)mc_SYSTEM,
                           (byte)sc_SYSTEM_FANPARA,
                           mFanInfo.mDuty,
                           mFanInfo.mFreq );
}
int deviceMRQ::setLedFreq( int ax, int freq )
{
    Q_ASSERT( ax >= 0 && ax < 4 );

    mLedInfo[ax].mFreq = freq;

    return m_pBus->write( DEVICE_RECEIVE_ID,
                          (byte)mc_SYSTEM,
                   (byte)sc_SYSTEM_ARMLEDPARA,
                   (byte)ax,
                   mFanInfo.mDuty,
                   mLedInfo[ax].mFreq );
}

int deviceMRQ::setFan( int duty, int freq )
{
    mFanInfo.mDuty = duty;
    mFanInfo.mFreq = freq;

    return m_pBus->write( DEVICE_RECEIVE_ID,
                          (byte)mc_SYSTEM,
                   (byte)sc_SYSTEM_FANPARA,
                   mFanInfo.mDuty,
                   mFanInfo.mFreq );
}
int deviceMRQ::setLed( int i, int duty, int freq )
{
    Q_ASSERT( i >= 0 && i < 4 );
    mLedInfo[i].mDuty = duty;
    mLedInfo[i].mFreq = freq;

    return m_pBus->write( DEVICE_RECEIVE_ID,
                          (byte)mc_SYSTEM,
                   (byte)sc_SYSTEM_ARMLEDPARA,
                   (byte)i,
                   mLedInfo[i].mDuty,
                   mLedInfo[i].mFreq );
}

int deviceMRQ::hRst()
{
    Q_ASSERT( NULL != m_pBus );

    return m_pBus->write( DEVICE_RECEIVE_ID,
                          (byte)0XCA,
                          (byte)0X24,
                          (byte)0XA5 );
}

//! [0~360)
float deviceMRQ::getIncAngle( int ax )
{
    int ret;
    quint32 xangle;

    ret = MRQ::getREPORT_DATA( ax, MRQ_REPORT_STATE_XANGLE, &xangle );
    if ( ret != 0 )
    { return -1; }
    else
    {
        return INC_ANGLE_TO_DEG( xangle );
    }
}

//! [0~360)
float deviceMRQ::getAbsAngle( int ax )
{
    int ret;
    quint32 xangle;

    ret = MRQ::getREPORT_DATA( ax, MRQ_REPORT_STATE_ABSENC, &xangle );
    if ( ret != 0 )
    { return -1; }
    else
    {logDbg()<<QString::number(xangle,16);
        return ABS_ANGLE_TO_DEG( (xangle&0x0ffffff) );  //! only 24 bit
    }
}

//! mm
float deviceMRQ::getDist( int ax )
{
    int ret;
    quint32 dist;

    ret = getREPORT_DATA_( ax, MRQ_REPORT_STATE_DIST, &dist );
    if ( ret != 0 )
    { return -1; }
    else
    {
        //! only lw
        dist = dist & 0xffff;
        return dist;
    }
}

float deviceMRQ::getSensor( int ax, int dataId )
{
    if ( dataId == MRQ_REPORT_STATE_DIST )
    { return getDist( ax); }
    else if ( dataId == MRQ_REPORT_STATE_XANGLE )
    { return getIncAngle( ax ); }
    else if ( dataId == MRQ_REPORT_STATE_ABSENC )
    { return getAbsAngle( ax ); }
    else
    { return 0; }
}

bool  deviceMRQ::getEncoderZeroValid()
{
    //! check all are 0
    for ( int i = 0; i < 4; i++ )
    {
        if ( mABSENCALARM_ZEROVALUE[i] != 0 )
        { return true; }
    }

    return false;
}
int   deviceMRQ::setEncoderZero( int ax, float zero )
{
    quint32 zeroEnc = valueToAbsAngle( zero );

    return setABSENCALARM_ZEROVALUE( (MRQ_IDENTITY_LABEL_1)ax, zeroEnc );
}
float deviceMRQ::getEncoderZero( int ax )
{
    if ( ax < 0 || ax > 3 )
    { return 0; }

    return absAngleToValue( mABSENCALARM_ZEROVALUE[ax] );
}

//! only write no read
int deviceMRQ::requestMotionState( pvt_region )
{
    int ret;

    DELOAD_REGION();

    ret = m_pBus->write( DEVICE_RECEIVE_ID,
                         mc_MOTION,
                         sc_MOTION_STATE_Q,
                         (byte)ax,
                         (byte)page );

    return ret;
}

void deviceMRQ::terminate( pvt_region )
{
    tpvDownloader *pLoader;
    pLoader = downloader( region );

    pLoader->terminate();
    pLoader->wait();


    sysQueue()->postMsg( e_download_terminated,
                         pLoader->name(),
                         pLoader->axes() );

}
void deviceMRQ::acquireDownloader()
{
    mDownloaderSema.acquire();
}
void deviceMRQ::releaseDownloader()
{
    mDownloaderSema.release();
}

//! assist api
int deviceMRQ::call( const tpvRegion &region )
{
    lpc( region.axes() )->postMsg(
                            (eRoboMsg)mrq_msg_call,
                            region
                        );
    logDbg();
    return 0;
}

int deviceMRQ::rotate( pvt_region, float t, float ang, float endV )
{
    run( pvt_region_p );

    return pvtWrite( pvt_region_p, t, ang, endV );
}

int deviceMRQ::preRotate( pvt_region, float t, float ang, float endV )
{
    return pvtWrite( pvt_region_p, t, ang, endV );
}

int deviceMRQ::syncRotate( pvt_region, float t, float ang, float endV, int tmo, int tick )
{
    //! check angle
    if ( VRobot::motionPredict( t, ang ) )
    {}
    else
    { return 0; }

    int ret = rotate( region, t, ang, endV );
    if ( ret != 0 )
    {
        sysError( QObject::tr("rotate fail") );
        return ret;
    }

    ret = waitFsm( region, mrq_state_idle, tmo, tick );
    if ( ret != 0 )
    {
        sysError( QObject::tr("wait idle fail") );
        return ret;
    }

    return 0;
}

int deviceMRQ::movej( pvt_region, float ang, float t, float angJ, float tj, float endV )
{
    run( pvt_region_p );

    return preMovej( pvt_region_p, ang, t, angJ, tj, endV );
}

int deviceMRQ::preMovej( pvt_region, float ang, float t, float angJ, float tj, float endV )
{
    tpvRow row;

    QList<tpvRow> rows;

    //! pt0
    row.mT = 0;
    row.mP = 0;
    row.mV = 0;
    rows.append( row );

    //! pt1
    row.mT = t;
    row.mP = ang;
    row.mV = 0;
    rows.append( row );

    //! pt2
    row.mT = t + tj;
    row.mP = angJ;
    row.mV = endV;
    rows.append( row );

    return pvtWrite( pvt_region_p, rows );
}

int deviceMRQ::lightCouplingZero( pvt_region, float t, float angle, float endV )
{
    return rotate( pvt_region_p, t, angle, endV );
}

int deviceMRQ::lightCouplingZero( pvt_region,
                                  float t, float angle, float endV,
                                  float invT, float invAngle,
                                  int tmous, int tickus )
{
    Q_ASSERT( mTaskThread.at(region.axes()) != NULL );

    //! check busy
    if ( mTaskThread.at(region.axes())->isRunning() )
    {
        sysError( QObject::tr("busy, can not run") );
        return -1;
    }

    //! post do
    ArgLightCoupZero *pArg = new ArgLightCoupZero();
    if ( NULL == pArg )
    { return ERR_ALLOC_FAIL; }

    //! arg
    pArg->mAx = region.axes();
    pArg->mPage = region.page();

    pArg->mT = t;
    pArg->mAngle = angle;
    pArg->mEndV = endV;

    pArg->mInvT = invT;
    pArg->mInvAngle = invAngle;

    //! tmo
    pArg->mTmo = tmous;
    pArg->mTick = tickus;

    //! request
    RoboTaskRequest *pReq = new RoboTaskRequest();
    Q_ASSERT( NULL != pReq );

    pReq->request( this,
                   (VRobot::apiTaskRequest)(this->taskLightCouplingZero),
                   pArg );

    //! start the thread
    mTaskThread.at(region.axes())->setRequest( pReq );

    mTaskThread.at( region.axes())->start();

    return 0;
}

int deviceMRQ::taskLightCouplingZero( void *pArg )
{
    int ret;
    Q_ASSERT( NULL != pArg );

    ArgLightCoupZero *pLightZero;
    pLightZero = (ArgLightCoupZero*)pArg;

    ret = 0;
    do
    {
        tpvRegion region( pLightZero->mAx, pLightZero->mPage );
        //! 1. rotate
        ret = syncRotate( region,
                          pLightZero->mT, pLightZero->mAngle, pLightZero->mEndV,
                          pLightZero->mTmo, pLightZero->mTick );
        if ( ret != 0 )
        { break; }

        //! 2. inverse rotate
        if ( pLightZero->mInvAngle != 0 )
        {
            ret = syncRotate( region,
                              pLightZero->mInvT, pLightZero->mInvAngle, 0,
                              pLightZero->mTmo, pLightZero->mTick );
            if ( ret != 0 )
            { break; }
        }
    }while( 0 );

    //! delete the arg
    delete pLightZero;

    return ret;
}

int deviceMRQ::fsmState( pvt_region, int iTask )
{
    Q_ASSERT( mMrqFsms.contains( region ) );

    if ( iTask > 0 )        //! is internal task, real status
    {
    }
    else                    //! not internal task
    {
        if ( mTaskThread.at( region.axes() )->isRunning() )
        { return mrq_state_running; }
        else
        {}
    }

    return mMrqFsms[ region ]->state();
}

//! only for internal task
int deviceMRQ::waitFsm( pvt_region,
             int dstState,
             int tmo,
             int tick
             )
{
    do
    {
        Q_ASSERT( tick > 0 );
        QThread::usleep( tick );

        if ( fsmState( region, 1 ) == dstState )
        { return 0; }

        tmo -= tick;
    }while( tmo > 0 );

    return -1;
}

qint64 deviceMRQ::busFrames()
{
    return ((MegaDevice::CANBus*)m_pBus)->frames();
}

int deviceMRQ::ioOut( int id, int val  )
{
    Q_ASSERT( m_pBus != NULL );

    int ret = 0;

    checked_call( m_pBus->doSend( QString("DEBUG:FACTORY:IOCONFIG %1,1\n").arg(id) ) );
    checked_call( m_pBus->doSend( QString("DEBUG:FACTORY:IOSET %1,%2\n").arg(id).arg(val) ) );

    return 0;
}

}
