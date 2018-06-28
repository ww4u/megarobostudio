#ifndef DEVICEMRV_H
#define DEVICEMRV_H

#include "../../../include/mcstd.h"
#include "../vdevice.h"
#include "../mrv_board/MRV.h"
#include "../motor/motor.h"

#include "../../com/basetype.h"

//#include "tpdownloader.h"
class TpDownloader;

namespace MegaDevice
{
class deviceMRV : public MRV
{
public:
    deviceMRV();

    virtual void postCtor();
public:
    virtual const void* loadScpiCmd();

    virtual int uploadSetting();
    virtual int applySetting();

public:
    virtual void rst();
    virtual int uploadDesc();
    virtual int uploadBaseInfo();
    virtual int uploadIDs();
    virtual int upload();

    virtual QList<int> deviceIds();
    virtual QString deviceFullDesc();

public:
    void acquireDownloader();
    void releaseDownloader();

    int tpWrite( QList<TpRow*> &list, int ax );
    int tpWrite( QList<QPointF> &points, int ax );  //! x: t, y : p

    //! send
    int tpBeginSend( int ax );
    int tpSend( TpRow *row, int ax );
    int tpEndSend( int ax );

public:
    //! prop
    QString loadDesc();
    QString loadSN();
    QString loadSwVer();
    QString loadHwVer();
    QString loadBtVer();

public:
    MRV_model *getModel();

protected:
    QList<int> mTpIndexes;

    QList<TpDownloader* > mDownloaders;
    QSemaphore mDownloaderSema;
};

}

#endif // DEVICEMRV_H
