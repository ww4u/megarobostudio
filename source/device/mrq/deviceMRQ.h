
#ifndef _DEVICE_MRQ_H_
#define _DEVICE_MRQ_H_

#include "../../../include/mcstd.h"
#include "../VDevice.h"
#include "../board/MRQ.h"
#include "../motor/motor.h"

#include "../../com/basetype.h"

#include "./tpvdownloader.h"

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

namespace MegaDevice
{

class deviceMRQ : public MRQ
{
public:
    static float absAngleToValue( quint32 angle );
    static quint32 valueToAbsAngle( float val );

    static float incAngleToValue( quint32 angle );
    static quint32 valueToIncAngle( float val );

public:
    deviceMRQ();
    ~deviceMRQ();

    virtual void postCtor();
    //! override
public:
    virtual const void* loadScpiCmd();

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
    virtual int upload();

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

    int loadPwms();
    int loadFanPwm();
    int loadLedPwm();

    QString loadName();

    //! overwrite
public:
    int getREPORT_DATA_( byte val0
    ,MRQ_REPORT_STATE val1, quint8 * val2, bool bQuery=true );

    int getREPORT_DATA_( byte val0
    ,MRQ_REPORT_STATE val1, quint16 * val2, bool bQuery=true );

    int getREPORT_DATA_( byte val0
    ,MRQ_REPORT_STATE val1, quint32 * val2, bool bQuery=true );

    QMetaType::Type getREPORT_TYPE( MRQ_REPORT_STATE stat );

    //! 1~100
    int setFanDuty( int duty );
    int setLedDuty( int ax, int duty );

    int setFanFreq( int freq );
    int setLedFreq( int ax, int freq );

    int setFan( int duty, int freq );
    int setLed( int ax, int duty, int freq );

    int hRst();

public:
    float getIncAngle( int ax );
    float getAbsAngle( int ax );
    float getDist( int ax );

    float getSensor( int ax, int dataId );

public:
    //! pvt ops
    int loadTpvCap();
    int getTpvCap( pvt_region );
    int getTpvBuf( pvt_region );

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

    void setTpvIndex( pvt_region, int index );
    int  getTpvIndex( pvt_region );
    void accTpvIndex( pvt_region );

    void terminate( pvt_region );

    int requestMotionState( pvt_region );

    void acquireDownloader();
    void releaseDownloader();

public:
    int call( pvt_region );
    int rotate( pvt_region, float t, float ang, float endV = 0 );

    int lightCouplingZero( pvt_region,
                           float t, float angle, float endV = 1 );

    int fsmState( pvt_region );

public:
    virtual int run( const tpvRegion &reg=tpvRegion() );
    virtual int stop( const tpvRegion &reg=tpvRegion() );

    virtual void setStatus( int stat, const tpvRegion &reg );
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
    QMap< tpvRegion, int > mTpvIndexes;
    QMap< tpvRegion, int > mTpvCaps;
    QMap< tpvRegion, int > mTpvBufferSizes;

    QMap< tpvRegion, tpvDownloader * > mDownloaders;

    QSemaphore mDownloaderSema;

    QMap< tpvRegion, MrqFsm* > mMrqFsms;

    QMap< tpvRegion, deviceProxyMotor * > mProxyMotors;
};

#define MRQ_PROGRESS( prog, info )      sysProgress( prog, info ); \
                                    sysProgress( true );
#define MRQ_PROGRESS_HIDE()             \
                                    sysProgress( false );

}

#endif
