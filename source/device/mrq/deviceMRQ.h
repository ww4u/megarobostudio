
#ifndef _DEVICE_MRQ_H_
#define _DEVICE_MRQ_H_

#include "../../../include/mcstd.h"
#include "../vdevice.h"
#include "../board/MRQ.h"
#include "../motor/motor.h"

#include "../../com/basetype.h"

#include "./tpvdownloader.h"
#include "./datauploader.h"

#include "devicemrq_state.h"

#include "devicemrq_msg.h"

#define pvt_region        const tpvRegion &region
#define pvt_region_p      region

#define DELOAD_REGION()   int ax = region.axes();\
                        int page = region.page();

#define pvt_page_p  ax, (MRQ_MOTION_SWITCH_1)page

#define foreach_page( )     for ( byte _i = 0; _i < axes(); _i++ )\
                            { \
                                MRQ_MOTION_SWITCH_1 _axPage=MRQ_MOTION_SWITCH_1_MAIN; \
                                for ( int _j = 0; \
                                      ( _axPage = (MRQ_MOTION_SWITCH_1)_j, _j < regions() ); \
                                      _j++ )


#define end_foreach_page()  }

#define MRQ_AX_PAGE     MRQ_MOTION_SWITCH_1

#define MRQ_MOTION_STATE            MRQ_MOTION_STATE_2
#define MRQ_MOTION_STATE_POWERON    MRQ_MOTION_STATE_2_POWERON
#define MRQ_MOTION_STATE_IDLE       MRQ_MOTION_STATE_2_IDLE
#define MRQ_MOTION_STATE_CALCING    MRQ_MOTION_STATE_2_CALCING
#define MRQ_MOTION_STATE_CALCEND    MRQ_MOTION_STATE_2_CALCEND
#define MRQ_MOTION_STATE_STANDBY    MRQ_MOTION_STATE_2_STANDBY
#define MRQ_MOTION_STATE_RUNNING    MRQ_MOTION_STATE_2_RUNNING
#define MRQ_MOTION_STATE_ERROR      MRQ_MOTION_STATE_2_ERROR

#define MRQ_DATA_XANGLE     0
#define MRQ_DATA_ABSANGLE   1

#define MRQ_DATA_DISTANCE   16


//! bit
enum AxesZeroOp
{
    axes_zero_op_none = 0,
    axes_zero_clr_cnt = 1,
    axes_zero_lose_step = 2,
};

#define combine_zero_op2( a, b )        (AxesZeroOp)( (a) | (b) )
#define combine_zero_op3( a, b, c )     (AxesZeroOp)( (a) | (b) | (c) )

namespace MegaDevice
{

class MrqTaskThread;

class deviceMRQ : public MRQ
{
public:
    typedef int (deviceMRQ::*apiMrqRequest)( void *pArg );

public:
    static float absAngleToValue( quint32 angle );
    static quint32 valueToAbsAngle( float val );

    static float incAngleToValue( quint32 angle );
    static quint32 valueToIncAngle( float val );

    static QString toString( mrqState sta );
    static QStringList _mrqStateList;

    static DataUploader *_pUploader;
public:
    deviceMRQ();
    ~deviceMRQ();

    virtual void postCtor();
    //! override
public:
    virtual const void* loadScpiCmd();

    virtual int applyDeviceId(DeviceId &id );

    virtual int setDeviceId(DeviceId &id );

public:
    //! overwrite
    void setName( const QString &strName );

public:
    MRQ_model *getModel();

public:
    virtual int uploadSetting();
    virtual int applySetting();

protected:
    int _uploadSetting();
    int _applySetting();

protected:
    int applyIds( );
    int verifyIds( );

public:
    virtual void rst();
    virtual int uploadDesc();
    virtual int uploadBaseInfo();
    virtual int uploadIDs();
    virtual int upload( EnumDeviceContent content );

    virtual QList<int> deviceIds(); //! receive/send/group
    virtual QList<int> subIDs( int ch );
    virtual QString deviceFullDesc();

public:
    int testAdd( int a, int b );

public:
    //! prop
    QString loadDesc();
    QString loadSN();
    QString loadSwVer();
    QString loadHwVer();
    QString loadFwVer();
    QString loadBtVer();

    QString loadSeqVer();

    int loadPwms();
    int loadFanPwm();
    int loadLedPwm();

    int loadEncoderZero();

    QString loadName();

    int setSN( const QString &sn );
    int loadSensorUartData( MRQ_SENSORUART_BAUD u,
                            MRQ_IDENTITY_LABEL_1 s,
                            QByteArray &ary );
    //! overwrite
public:
    int getREPORT_DATA_( byte val0
    ,MRQ_REPORT_STATE val1, quint8 * val2, bool bQuery=true );

    int getREPORT_DATA_( byte val0
    ,MRQ_REPORT_STATE val1, quint16 * val2, bool bQuery=true );

    int getREPORT_DATA_( byte val0
    ,MRQ_REPORT_STATE val1, quint32 * val2, bool bQuery=true );

    QMetaType::Type getREPORT_TYPE( MRQ_REPORT_STATE stat );

    //! ca 02
    int getSeqVer( quint8 *p1, quint8 *p2, quint8 *p3, quint8 *p4, bool bQuery = true );

    //! 1~100
    int setFanDuty( int duty );
    int setLedDuty( int ax, int duty );

    int setFanFreq( int freq );
    int setLedFreq( int ax, int freq );

    int setFan( int duty, int freq );
    int setLed( int ax, int duty, int freq );

    int hRst();

    //! request
    int requestPDM_MICSTEPDATA( byte val0
    ,uint16 val1
    ,uint16 val2 );

    int micUpload( int ch,
                   const QString &fileName );

public:
    float getIncAngle( int ax );
    float getAbsAngle( int ax );
    float getDist( int ax );

    float getSensor( int ax, int dataId );

    bool  getEncoderZeroValid();
    int   setEncoderZero( int ax, float zero );
    float getEncoderZero( int ax );

public:
    //! pvt ops
    int loadTpvCap();
    int getTpvCap( pvt_region );
    int getTpvBuf( pvt_region );

    int loadMotorBasic();

    int beginTpvDownload( pvt_region );
    int tpvDownload(
                     pvt_region,
                     int index,
                     f32 t,
                     f32 p,
                     f32 v );
    int tpvDownload( pvt_region,
                     QList<tpvRow *> &list,
                     int from,
                     int len );

    int tpvDownload( pvt_region,
                     tpvRow *pItem );

    int endTpvDownload( pvt_region );

    int tpvDownloadMission( pvt_region,
                            QList<tpvRow *> &list,
                            int from,
                            int len );
    int pvtWrite( pvt_region,
                  QList<tpvRow *> &list,
                  int from = 0,
                  int len = -1 );

    int pvtWrite( pvt_region,
                  float t1, float p1,
                  float t2, float p2,
                  float endV = 0);

    int pvtWrite( pvt_region,
                  float dT,
                  float dAngle,
                  float endV = 0);
    int pvtWrite( pvt_region,
                  tpvRow *pRows,
                  int n
                  );
    int pvtWrite( pvt_region,
                  QList<tpvRow> &rows );

    void setTpvIndex( pvt_region, int index );
    int  getTpvIndex( pvt_region );
    void accTpvIndex( pvt_region );

    bool pvtVerify( pvt_region,
                    QList<tpvRow *> &list );

    void terminate( pvt_region );

    int requestMotionState( pvt_region );

    void acquireDownloader();
    void releaseDownloader();

public:
    int call( int n, pvt_region );
    int rotate( pvt_region, float t, float ang, float endV = 0 );
    int preRotate( pvt_region, float t, float ang, float endV = 0 );
    int syncRotate( pvt_region, float t, float ang, float endV, int tmo, int tick );

    int movej( pvt_region, float ang, float t, float angJ, float tj, float endV = 0 );
    int preMovej( pvt_region, float ang, float t, float angJ, float tj, float endV = 0 );

    int lightCouplingZero( pvt_region,
                           float t, float angle, float endV = 0 );

    int lightCouplingZero( pvt_region,
                           float t, float angle, float endV,
                           float invT, float invAngle,
//                           bool bClrCnt,
                           AxesZeroOp zOp,
                           int tmous, int tickus );

    int taskLightCouplingZero( void *pArg );

    int fsmState( pvt_region, int iTask = 0 );
    int waitFsm( pvt_region,
                 int dstState,
                 int tmo,
                 int tick
                 );

    qint64 busFrames();

    int ioOut( int id, int val );

public:
    virtual int run( const tpvRegion &reg=tpvRegion() );
    virtual int stop( const tpvRegion &reg=tpvRegion() );

    virtual int setLoop( int n, const tpvRegion &region=0 );

    virtual void setStatus( int stat, const tpvRegion &reg, frameData &data );
    //! p1:ax, p2:page
    virtual bool checkLink( int p1=0, int p2 = 0 );
public:
    virtual void onMsg( int subAxes, RoboMsg &msg );
    virtual void onTimer( void *pContext, int id );

    virtual void attachCondition( const tpvRegion &region,
                                  RoboCondition *pCond );
    virtual bool waitCondition( const tpvRegion &region,
                                RoboCondition *pCond,
                                int tmoms=-1 );

    RoboFsm *Fsm( const tpvRegion &region );

    deviceProxyMotor *Motor( const tpvRegion &region );
    tpvDownloader *downloader( const tpvRegion &region );

protected:
    float slowRatio( int ax );
    float stepAngle( int ax );
    float microStep( int ax );

protected:
    QMap< tpvRegion, int > mSlewCache;

    QMap< tpvRegion, int > mTpvIndexes;
    QMap< tpvRegion, int > mTpvCaps;
    QMap< tpvRegion, int > mTpvBufferSizes;

    QMap< tpvRegion, tpvDownloader * > mDownloaders;

    QSemaphore mDownloaderSema;

    QMap< tpvRegion, MrqFsm* > mMrqFsms;

    QMap< tpvRegion, deviceProxyMotor * > mProxyMotors;

    QList< MrqTaskThread *> mTaskThread;      //! for each axes
};

struct ArgLightCoupZero : public RoboTaskArgument
{
    int mAx, mPage;
//    bool mbClrCnt;
    AxesZeroOp mZOp;
    float mT, mAngle, mEndV;
    float mInvT, mInvAngle;

    ArgLightCoupZero();
};

class MrqTaskThread : public RoboTask
{
public:
    MrqTaskThread(  QObject *parent = NULL );

protected:
    virtual void run();

};

#define MRQ_PROGRESS( prog, info )      sysProgress( prog, info ); \
                                    sysProgress( true );


#define MRQ_PROGRESS_HIDE()             \
                                    sysProgress( false );
}

#endif
