#ifndef DEVICEMRV_H
#define DEVICEMRV_H

#include "../../../include/mcstd.h"
#include "../vdevice.h"
#include "../mrv_board/MRV.h"
#include "../motor/motor.h"

#include "../../com/basetype.h"

#include "mrvaxes.h"

class TpDownloader;

namespace MegaDevice
{

enum mrvMsg
{
    mrv_msg_unk = e_robo_operation,

    mrv_msg_idle,       //! region
    mrv_msg_end,
    mrv_msg_running,

    mrv_msg_run,        //! region
    mrv_msg_stop,
    mrv_msg_reset,
};

class deviceMRV : public MRV
{
public:
    deviceMRV();
    virtual ~deviceMRV();

    virtual void postCtor();
public:
    virtual const void* loadScpiCmd();
    virtual int setDeviceId(DeviceId &id );

    virtual int uploadSetting();
    virtual int applySetting();

protected:
    int _uploadSetting();
    int _applySetting();

public:
    virtual void rst();
    virtual int uploadDesc();
    virtual int uploadBaseInfo();
    virtual int uploadIDs();
    virtual int upload();

    virtual QList<int> deviceIds();
    virtual QString deviceFullDesc();

public:
    virtual void onMsg( int subAxes, RoboMsg &msg );
    virtual void onTimer( void *pContext, int id );

    virtual void setStatus( int stat, const tpvRegion &reg, frameData &data );

public:
    int switchReset( int ax );
    int switchRun( int ax );
    int switchStop( int ax );

    void preSet( int ax = x_channel );
    void postSet( int ax = x_channel );

public:
    //! over write
    int setVALVECTRL_DEVICE( byte val0, MRV_VALVECTRL_DEVICE val1 );

public:
    void acquireDownloader();
    void releaseDownloader();

    bool isDownloading( int ax );
    int tpWrite( QList<TpRow*> &list, int ax );
    int tpWrite( QList<QPointF> &points, int ax );  //! x: t, y : p

    //! send
    int tpBeginSend( int ax );
    int tpSend( TpRow *row, int ax );
    int tpEndSend( int ax, TpRow &preRow );
    quint32 alignP( int ax, tpvType val );

    //! request
    int requestMotionState( int ax, int page );

public:
    //! prop
    QString loadDesc();
    QString loadSN();
    QString loadSwVer();
    QString loadHwVer();
    QString loadBtVer();

protected:
    void loadCycles();
    void loadDeviceMode();
    void loadExecMode();

public:
    MRV_model *getModel();

protected:
    QList<int> mTpIndexes;

    QList<TpDownloader* > mDownloaders;
    QSemaphore mDownloaderSema;

    QList<MrvAxes*> mAxesList;

};

#define MRV_PROGRESS( prog, info )      sysProgress( prog, info ); \
                                    sysProgress( true );


#define MRV_PROGRESS_HIDE()             \
                                    sysProgress( false );

}

#endif // DEVICEMRV_H
