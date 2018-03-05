
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
    int setMOTIONPLAN_POSITION( uint16 val0
    ,f32 val1 );

    int setMOTIONPLAN_VELOCITY( uint16 val0
    ,f32 val1 );

    int setMOTIONPLAN_TIME( uint16 val0
    ,f32 val1 );

    int setMOTIONPLAN_POSITION( int axesid, uint16 val0
    ,f32 val1 );

    int setMOTIONPLAN_VELOCITY( int axesid, uint16 val0
    ,f32 val1 );

    int setMOTIONPLAN_TIME( int axesid, uint16 val0
    ,f32 val1 );

    //! overwrite
    void setName( const QString &strName );

public:
    MRQ_model *getModel();

    int loadOn();

protected:
    int applyIds( int siblingCnt );
    int verifyIds( int siblingCnt );

public:
    virtual void rst();
    virtual int upload();

public:
    int testAdd( int a, int b );

public:
    QString loadDesc();
    QString loadSN();
    QString loadSwVer();
    QString loadHwVer();
    QString loadFwVer();
    QString loadBtVer();

    QString loadName();

    int     loadTpvCap();

    int goInit(  int axesId,
                 MRQ_MOTION_INITPOSITIONUNIT unit,
                 MRQ_MOTION_INITIOSIGNAL iosig,
                 float pos
                 );

    int beginTpvDownload( int axesId );
    int tpvDownload(
                     int axesId,
                     int index,
                     f32 t,
                     f32 p,
                     f32 v );
    int tpvDownload( int axesId,
                     QList<tpvRow *> &list,
                     int from,
                     int len );

    int tpvDownload( int axesId,
                     tpvRow *pItem );

    int endTpvDownload( int axesId );

    int tpvDownloadMission( int axesId,
                            QList<tpvRow *> &list,
                            int from,
                            int len );
    int pvtWrite( int axesId,
                  QList<tpvRow *> &list,
                  int from = 0,
                  int len = -1 );

    int pvtWrite( int axesId,
                  float t1, float p1,
                  float t2, float p2 );

    int pvtWrite( int axesId,
                  float dT,
                  float dAngle );

    void setTpvIndex( int axesId, int index );
    int  getTpvIndex( int axesId );
    void accTpvIndex( int axesId );

    int  getTpvCap();

    void terminate( int axesId );

    int requestMotionState( int axesId );

    void acquireDownloader();
    void releaseDownloader();

public:
    int rotate( int as, float t, float ang );
    int fsmState( int ax );
public:
    virtual int run(int axes);
    virtual int stop(int axes);

    virtual void setStatus( int stat, int ch = 0 );
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
    int mTpvIndex[4];   //! 4 axes
    int mTpvCap;

    tpvDownloader mDownloader[4];
    QSemaphore mDownloaderSema;

    MrqFsm mMrqFsms[4];

    deviceMotor mMotors[4];
};

}

#endif
