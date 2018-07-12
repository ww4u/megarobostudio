#ifndef TPDOWNLOADER_H
#define TPDOWNLOADER_H

#include <QThread>

#include "../model/tpitem.h"
#include "devicemrv.h"

#include "../devicedownloader.h"

class TpDownloader : public DeviceDownloader
{
    Q_OBJECT
public:
    explicit TpDownloader(QObject *parent = nullptr);

signals:

public slots:

protected:
    virtual void run();

public:
    void attachDevice( MegaDevice::deviceMRV *pMrv, int ax );
    void append( QList<TpRow*> &rows );

protected:
    int downloadProc();
    int batchDownload( QList<TpRow*> &rows, TpRow &refItem );
    int diffT( QList<TpRow*> &rows );

protected:
    MegaDevice::deviceMRV *m_pMRV;
    int mAx;

    QList<TpRow*> mTpList;

    QMutex mTpMutex;

};

#endif // TPDOWNLOADER_H
