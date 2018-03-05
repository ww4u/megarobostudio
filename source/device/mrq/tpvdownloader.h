#ifndef TPVDOWNLOADER_H
#define TPVDOWNLOADER_H

#include <QtCore>



#include "../../model/tpvitem.h"

//! delcare
namespace MegaDevice {
class deviceMRQ;
}

//! download the tpv to pvts
class tpvDownloader : public QThread
{
    Q_OBJECT

public:
    tpvDownloader( QObject *pObj=0 );

Q_SIGNALS:
    void sig_progress( int cur, int from, int to );

protected:
    virtual void run();

protected:
    void downloadProc();

    int batchDownload( int batchSize,
                       int &total,
                       int &now
                       );
    int transmissionProc();

public:
    void attachDevice( MegaDevice::deviceMRQ *pDev,
                       int axesId );

    void append( QList<tpvRow*> &rows, int from=0, int len=-1 );

    QString name();
    int     axes();

protected:
    int mTryInterval;   //! us

    MegaDevice::deviceMRQ *m_pMRQ;
    int mAxesId;

    QQueue< tpvRow *> mTpvs;
    QMutex mQueueMutex;

};

#endif // TPVDOWNLOADER_H
