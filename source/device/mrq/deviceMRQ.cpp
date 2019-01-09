#include "deviceMRQ.h"

#include "scpi/scpi.h"

#include "../../bus/canbus.h"

#include "../../arith/pvt/pvt.h"

#define DEF_TPV_CAP             256
#define DEF_WAV_BUF_SIZE        (125*1024)                  //! bytes


#define ABS_ANGLE_TO_DEG( angle )   (360.0f*(angle))/((1<<18)-1)
#define INC_ANGLE_TO_DEG( angle )   (360.0f*(angle))/(1<<18)

#define VALUE_TO_ABS_ANGLE( val )   (quint32)( (val) * ((1<<18)-1) / 360.0f )
#define VALUE_TO_INC_ANGLE( val )   (quint32)( (val) * ((1<<18)) / 360.0f )



namespace MegaDevice
{

QStringList deviceMRQ::_mrqStateList;
DataUploader *deviceMRQ::_pUploader = NULL;

float deviceMRQ::absAngleToValue( quint32 angle )
{ return ABS_ANGLE_TO_DEG( angle ); }
quint32 deviceMRQ::valueToAbsAngle( float val )
{ return VALUE_TO_ABS_ANGLE(val); }

float deviceMRQ::s_absAngleToValue( qint32 angle )
{
    if ( angle < 0  )
    {
        return -absAngleToValue( -angle );
    }
    else
    {
        return absAngleToValue( angle );
    }
}
qint32 deviceMRQ::s_valueToAbsAngle( float val )
{
    if ( val < 0 )
    { return -valueToAbsAngle( -val ); }
    else
    { return valueToAbsAngle(val); }
}

float deviceMRQ::incAngleToValue( quint32 angle )
{ return INC_ANGLE_TO_DEG( angle ); }
quint32 deviceMRQ::valueToIncAngle( float val )
{ return VALUE_TO_INC_ANGLE(val); }

//mrq_state_idle,
//mrq_state_run_reqed,
//mrq_state_program,

//mrq_state_calcing,
//mrq_state_calcend,
//mrq_state_standby,

//mrq_state_prerun,
//mrq_state_running,
//mrq_state_prestop,

QString deviceMRQ::toString( mrqState sta )
{
    //! init at first
    if (_mrqStateList.size() < 1 )
    {
        deviceMRQ::_mrqStateList<<"IDLE"
                                <<"RUN_REQED"
                                <<"PROGRAM"
                                <<"CALCING"
                                <<"CALCEND"
                                <<"STANDBY"
                                <<"PRE_RUN"
                                <<"RUNNING"
                                <<"PRE_STOP";
    }

    Q_ASSERT( _mrqStateList.size() >= (int)sta );

    if ( (int)sta < 0 )
    { return "NONE"; }

    return _mrqStateList.at( (int)sta );
}

deviceMRQ::deviceMRQ()
{
    //! downloader in ctor

    //! fsm in post ctor

    //! proxy motor in post ctor

    //! uploader
    if ( NULL == deviceMRQ::_pUploader )
    {
        deviceMRQ::_pUploader = new DataUploader();
        Q_ASSERT( NULL != deviceMRQ::_pUploader );
    }
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

    mAccList.clear();
    mDecList.clear();
    mAngleDir.clear();
    mPOSAble.clear();

    //! foreach axes
    for ( int i = 0; i < axes(); i++ )
    {
        mPOSAble<<true;
        mAngleDir<<true;

        //! default acc/dec list
        mAccList<<DEF_ACC_SCALE;
        mDecList<<DEF_DEC_SCALE;

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

            //! slew cache
            mSlewCache.insert( region, -1 );
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
    ret = m_pBus->read( DEVICE_RECEIVE_ID, MRQ_mc_REPORT, MRQ_sc_REPORT_DATA_Q , val0, (byte)val1, &lval0, bQuery);
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
    ret = m_pBus->read( DEVICE_RECEIVE_ID, MRQ_mc_REPORT, MRQ_sc_REPORT_DATA_Q , val0, (byte)val1, &lval0, bQuery);
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
    ret = m_pBus->read( DEVICE_RECEIVE_ID, MRQ_mc_REPORT, MRQ_sc_REPORT_DATA_Q , val0, (byte)val1, &lval0, bQuery);
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
                    (byte)MRQ_mc_SYSTEM,
                   (byte)MRQ_sc_SYSTEM_FANPARA,
                   mFanInfo.mDuty,
                   mFanInfo.mFreq );
}
int deviceMRQ::setLedDuty( int i, int duty )
{
    Q_ASSERT( i >= 0 && i < 4 );

    mLedInfo[i].mDuty = duty;

    return m_pBus->write( DEVICE_RECEIVE_ID,
                          (byte)MRQ_mc_SYSTEM,
                           (byte)MRQ_sc_SYSTEM_ARMLEDPARA,
                           (byte)i,
                           mLedInfo[i].mDuty,
                           mLedInfo[i].mFreq );
}

int deviceMRQ::setFanFreq( int freq )
{
    mFanInfo.mFreq = freq;

    return m_pBus->write( DEVICE_RECEIVE_ID,
                          (byte)MRQ_mc_SYSTEM,
                           (byte)MRQ_sc_SYSTEM_FANPARA,
                           mFanInfo.mDuty,
                           mFanInfo.mFreq );
}
int deviceMRQ::setLedFreq( int ax, int freq )
{
    Q_ASSERT( ax >= 0 && ax < 4 );

    mLedInfo[ax].mFreq = freq;

    return m_pBus->write( DEVICE_RECEIVE_ID,
                          (byte)MRQ_mc_SYSTEM,
                   (byte)MRQ_sc_SYSTEM_ARMLEDPARA,
                   (byte)ax,
                   mFanInfo.mDuty,
                   mLedInfo[ax].mFreq );
}

int deviceMRQ::setFan( int duty, int freq )
{
    mFanInfo.mDuty = duty;
    mFanInfo.mFreq = freq;

    return m_pBus->write( DEVICE_RECEIVE_ID,
                          (byte)MRQ_mc_SYSTEM,
                   (byte)MRQ_sc_SYSTEM_FANPARA,
                   mFanInfo.mDuty,
                   mFanInfo.mFreq );
}
int deviceMRQ::setLed( int i, int duty, int freq )
{
    Q_ASSERT( i >= 0 && i < 4 );
    mLedInfo[i].mDuty = duty;
    mLedInfo[i].mFreq = freq;

    return m_pBus->write( DEVICE_RECEIVE_ID,
                          (byte)MRQ_mc_SYSTEM,
                   (byte)MRQ_sc_SYSTEM_ARMLEDPARA,
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

//! ch, start, len
int deviceMRQ::requestPDM_MICSTEPDATA( byte val0
,uint16 val1
,uint16 val2 )
{
    //! 76 5
    int ret = 0;

    ret = m_pBus->write( DEVICE_RECEIVE_ID,
                         (byte)MRQ_mc_PDM,
                         (byte)MRQ_sc_PDM_MICSTEPDATA_Q,
                         val0,
                         val1,
                         val2 );
    if ( ret != 0){ log_device(); }
    if ( ret != 0) return ret;

    return 0;
}

int deviceMRQ::micUpload( int ch, const QString &fileName )
{
    Q_ASSERT( NULL!=deviceMRQ::_pUploader );

    if ( deviceMRQ::_pUploader->isRunning() )
    {
        sysError( QObject::tr("Can not upload") );
        return -1;
    }

    deviceMRQ::_pUploader->requestLoad( m_pBus->receiveProxy(),
                                        this,
                                        ch,
                                        fileName );

    deviceMRQ::_pUploader->kickoff();

    return 0;
}

//! [0~360)
float deviceMRQ::getIncAngle( int ax )
{
    //! get lines
    int ret;
    int xLines;
    ret = MRQ::getMOTION_ABCOUNT( ax, &xLines );
    if ( ret != 0 )
    { return 0; }

    //! try the line number
    if ( mENCODER_LINENUM[ax] < 1 )
    { getENCODER_LINENUM( ax, mENCODER_LINENUM + ax ); }
    if ( mENCODER_LINENUM[ax] < 1 )
    {
        sysWarn( QObject::tr("Invalid encoder line number") );
        return 0;
    }

    return xLines*360.0/mENCODER_LINENUM[ax];
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
    {/*logDbg()<<QString::number(xangle,16);*/
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
    if ( dataId == MRQ_DATA_DISTANCE )
    { return getDist( ax); }
    else if ( dataId == MRQ_DATA_XANGLE )
    { return getIncAngle( ax ); }
    else if ( dataId == MRQ_DATA_ABSANGLE )
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
int deviceMRQ::setEncoderZero( int ax, float zero )
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
                         MRQ_mc_MOTION,
                         MRQ_sc_MOTION_STATE_Q,
                         (byte)ax,
                         (byte)page );

    return ret;
}

void deviceMRQ::terminate( pvt_region )
{
    tpvDownloader *pLoader;
    pLoader = downloader( region );
    Q_ASSERT( NULL != pLoader );

    pLoader->requestInterruption();
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
int deviceMRQ::call( int n, const tpvRegion &region )
{
    //! set loop
    setMOTIONPLAN_CYCLENUM( region.axes(),
                            (MRQ_MOTION_SWITCH_1)region.page(),
                             n );
    //! valid
    if ( region.motionMode() >= 0 )
    {
        //! set motion mode
        setMOTIONPLAN_MOTIONMODE( region.axes(),
                                  (MRQ_MOTION_SWITCH_1)region.page(),
                                  (MRQ_MOTIONPLAN_MOTIONMODE_1)region.motionMode()
                                  );
    }

    Q_ASSERT( mMrqFsms.contains( region) );
    mMrqFsms[ region ]->setState( mrq_state_calcend );

    //! set mode
    lpc( region.axes() )->postMsg(
                                 (eRoboMsg)mrq_msg_call,
                                 region
                                );

    return 0;
}

int deviceMRQ::rotate( pvt_region, float t, float ang, float endV )
{
    //! set loop 1
    setMOTIONPLAN_CYCLENUM( region.axes(),
                            (MRQ_MOTION_SWITCH_1)region.page(),
                             1 );

    Q_ASSERT( mMrqFsms.contains( region) );
    mMrqFsms[ region ]->setState( mrq_state_run_reqed );

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

//! 0, 0
//! t, ang
//! t+tj, angJ
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

    //! slope lize


    return pvtWrite( pvt_region_p, rows );
}

int deviceMRQ::lightCouplingZero( pvt_region, float t, float angle, float endV )
{
    return rotate( pvt_region_p, t, angle, endV );
}

int deviceMRQ::lightCouplingZero( pvt_region,
                                  float t, float angle, float endV,
                                  float invT, float invAngle,
                                  AxesZeroOp zOp,
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
    pArg->mZOp = zOp;

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
                   (VRobot::apiTaskRequest)(&deviceMRQ::taskLightCouplingZero),
                   pArg );

    //! start the thread
    mTaskThread.at( region.axes() )->setRequest( pReq );

    mTaskThread.at( region.axes() )->start();

    return 0;
}

int deviceMRQ::taskLightCouplingZero( void *pArg )
{
    int ret, retRot;
    Q_ASSERT( NULL != pArg );

    ArgLightCoupZero *pLightZero;
    pLightZero = (ArgLightCoupZero*)pArg;

    ret = 0;
    do
    {
        tpvRegion region( pLightZero->mAx, pLightZero->mPage );

        //! 0. lose step
        if ( is_mask( pLightZero->mZOp, axes_zero_lose_step) )
        {
            ret = setMOTIONPLAN_OOSLINESTATE( pLightZero->mAx,
                                        (MRQ_MOTION_SWITCH_1)pLightZero->mPage,
                                        MRQ_SYSTEM_REVMOTION_ON );
            if ( ret != 0 )
            { return -1; }
        }

        //! 1. rotate
        retRot = syncRotate( region,
                          pLightZero->mT, pLightZero->mAngle, pLightZero->mEndV,
                          pLightZero->mTmo, pLightZero->mTick );

        //! 2. lose step
        if ( is_mask( pLightZero->mZOp, axes_zero_lose_step) )
        {
            ret = setMOTIONPLAN_OOSLINESTATE( pLightZero->mAx,
                                        (MRQ_MOTION_SWITCH_1)pLightZero->mPage,
                                        MRQ_SYSTEM_REVMOTION_OFF );
            if ( ret != 0 )
            { return -1; }
        }

        if ( retRot != 0 )
        { break; }

        //! 3. inverse rotate
        if ( pLightZero->mInvAngle != 0 )
        {
            ret = syncRotate( region,
                              pLightZero->mInvT, pLightZero->mInvAngle, 0,
                              pLightZero->mTmo, pLightZero->mTick );
            if ( ret != 0 )
            { break; }
        }

        //! 4. clr cnt
        if ( is_mask( pLightZero->mZOp, axes_zero_clr_cnt ) )
        {
            ret = setMOTION_ABCOUNTCLEAR( region.axes() );
            if ( ret != 0 )
            { return ret; }
        }

    }while( 0 );

//    //! delete the arg
//    delete pLightZero;
    //! \note the arg is deleted in the thread run()

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

        if ( QThread::currentThread()->isInterruptionRequested() )
        { return -1; }

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

int deviceMRQ::slopelize( QList<tpvRow> &curve,
                          int mode,
                          float rise,
                          float fall,
                          int &errRow )
{
    int errIndex;
    int ret;
    double slopes[2]={ rise,fall };

    errRow = -1;
    for ( int i = 0; i < curve.size() - 1 ; i++ )
    {
        ret = pvtSlope( curve.at(i).mP, curve.at(i).mV, curve.at(i).mT,
                        curve.at(i+1).mP, curve.at(i+1).mV, curve.at(i+1).mT,
                        mode,
                        slopes,
                        &errIndex
                  );
        if ( ret != 0 )
        {
            errRow = i;
            return ret;
        }

        //! save to the next
        curve[i+1].mRise = slopes[0];
        curve[i+1].mFall = slopes[1];
    }

    return 0;

}

}


