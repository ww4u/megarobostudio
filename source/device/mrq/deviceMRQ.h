
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

#define pvt_page    int ax, int page
#define pvt_page_p  ax, (MRQ_MOTION_SWITCH_1)page

#define foreach_page( )     for ( byte _i = 0; _i < axes(); _i++ )\
                            { \
                                MRQ_MOTION_SWITCH_1 _axPage=MRQ_MOTION_SWITCH_1_MAIN; \
                                for ( int _j = 0; \
                                      ( _axPage = (MRQ_MOTION_SWITCH_1)_j, _j < pages() ); \
                                      _j++ )


#define end_foreach_page()  }

#define MRQ_AX_PAGE     MRQ_MOTION_SWITCH_1

#define MRQ_MOTION_STATE    MRQ_MOTION_STATE_2
#define MRQ_MOTION_STATE_POWERON MRQ_MOTION_STATE_2_POWERON
#define MRQ_MOTION_STATE_IDLE MRQ_MOTION_STATE_2_IDLE
#define MRQ_MOTION_STATE_CALCING MRQ_MOTION_STATE_2_CALCING
#define MRQ_MOTION_STATE_CALCEND MRQ_MOTION_STATE_2_CALCEND
#define MRQ_MOTION_STATE_STANDBY MRQ_MOTION_STATE_2_STANDBY
#define MRQ_MOTION_STATE_RUNNING MRQ_MOTION_STATE_2_RUNNING
#define MRQ_MOTION_STATE_ERROR MRQ_MOTION_STATE_2_ERROR
namespace MegaDevice
{

class deviceMRQ : public MRQ
{

public:
    deviceMRQ();
    ~deviceMRQ();

public:
    virtual const void* loadScpiCmd();

    virtual int setDeviceId(DeviceId &id, int siblingCnt );

    //! override
public:
    //! overwrite
    void setName( const QString &strName );

public:
    MRQ_model *getModel();

public:
    virtual int uploadSetting();

protected:
    int applyIds( int siblingCnt );
    int verifyIds( int siblingCnt );

public:
    virtual void rst();
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

    QString loadName();

    //! override
public:
    int getREPORT_DATA_( byte val0
    ,MRQ_REPORT_STATE val1, quint8 * val2, bool bQuery=true );

    int getREPORT_DATA_( byte val0
    ,MRQ_REPORT_STATE val1, quint16 * val2, bool bQuery=true );

    int getREPORT_DATA_( byte val0
    ,MRQ_REPORT_STATE val1, quint32 * val2, bool bQuery=true );

    QMetaType::Type getREPORT_TYPE( MRQ_REPORT_STATE stat );

public:
    float getAngle( int ax );
    float getDist( int ax );

    float getSensor( int ax, int dataId );

public:
    //! pvt
    int loadTpvCap();
    int getTpvCap( int ax, int page = 0 );

    int beginTpvDownload( pvt_page );
    int tpvDownload(
                     pvt_page,
                     int index,
                     f32 t,
                     f32 p,
                     f32 v );
    int tpvDownload( pvt_page,
                     QList<tpvRow *> &list,
                     int from,
                     int len );

    int tpvDownload( pvt_page,
                     tpvRow *pItem );

    int endTpvDownload( pvt_page );

    int tpvDownloadMission( pvt_page,
                            QList<tpvRow *> &list,
                            int from,
                            int len );
    int pvtWrite( pvt_page,
                  QList<tpvRow *> &list,
                  int from = 0,
                  int len = -1 );

    int pvtWrite( pvt_page,
                  float t1, float p1,
                  float t2, float p2 );

    int pvtWrite( pvt_page,
                  float dT,
                  float dAngle );

    void setTpvIndex( pvt_page, int index );
    int  getTpvIndex( pvt_page );
    void accTpvIndex( pvt_page );

    void terminate( int axesId );

    int requestMotionState( int axesId );

    void acquireDownloader();
    void releaseDownloader();

public:
    int prepare( int ax, int page = 0 );
    int rotate( int as, float t, float ang );
    int tpvWrite( int ax,
                  int page,
                  tpvRow *pRows,
                  int n
                  );

    int fsmState( int ax );

public:
    virtual int run(int axes);
    virtual int stop(int axes);

    virtual void setStatus( int stat, int ch = 0, int page = 0 );
public:
    virtual void onMsg( int subAxes, RoboMsg &msg );
    virtual void onTimer( void *pContext, int id );

    virtual void attachCondition( int subAxes,
                                  RoboCondition *pCond );
    virtual bool waitCondition( int subAxes,
                                RoboCondition *pCond,
                                int tmoms=-1 );

    RoboFsm *Fsm( int ax );

    deviceMotor *Motor( int ax );

protected:
    int mTpvIndex[4][10];           //! 4 axes
    uint16 mTpvCaps[4][10];


                            //! by ax && by pages
//    QList< QList<int>* > mTpvCaps;

    tpvDownloader mDownloader[4];
    QSemaphore mDownloaderSema;

    MrqFsm mMrqFsms[4];

    deviceMotor mMotors[4];
};

}

#endif
