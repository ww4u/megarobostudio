#ifndef TPVDOWNLOADER_H
#define TPVDOWNLOADER_H

#include <QtCore>

#include "../../model/tpvitem.h"
#include "../devicedownloader.h"
//! delcare
namespace MegaDevice {
class deviceMRQ;
}

//! download the tpv to pvts
class tpvDownloader : public DeviceDownloader
{
    Q_OBJECT

private:
    static int _downloaderInterval;


public:
    static void setInterval( int interval );
    static int interval();



public:
    tpvDownloader( QObject *pObj=0 );
    virtual ~tpvDownloader();

Q_SIGNALS:
    void sig_progress( int cur, int from, int to );

protected:
    virtual void run();

protected:
    int downloadProc();

    int batchDownload( QQueue< tpvRow *> &transQueue,
                       int batchSize,
                       int &total,
                       int &now
                       );
    int transmissionProc( QQueue< tpvRow *> &transQueue );

public:
    void attachDevice( MegaDevice::deviceMRQ *pDev,
                       const tpvRegion &region );
    void setRegion( const tpvRegion &region );

    void append( QList<tpvRow*> &rows, int from=0, int len=-1 );

    QString name();
    int     axes();

protected:
    int mTryInterval;   //! us

    MegaDevice::deviceMRQ *m_pMRQ;
    tpvRegion mRegion;

    QQueue< tpvRow *> mTpvs;

//    QQueue< tpvRow *> mTransferTpvs;      //! transfer tpvs

    QMutex mQueueMutex;

};

#endif // TPVDOWNLOADER_H
