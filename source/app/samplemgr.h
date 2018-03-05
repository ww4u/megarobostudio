#ifndef SAMPLEMGR_H
#define SAMPLEMGR_H

#include <QtCore>
#include "sampleproxy.h"

#include "../model/dbmeta.h"

#include "../../../inst/instmgr.h"

class sampleThread;

class sampleMgr
{
public:
    sampleMgr();
    ~sampleMgr();

public:
    void attachProxy( sampleProxy *pProxy );
    void attachProxy( sampleProxy &proxy );

    void attachDbMeta( dbMeta *pMeta );
    void attachInstMgr( MegaDevice::InstMgr *pInstMgr );

    int  getCount();

    void clear( sampleThread *pCallThread=NULL );
    void stop();

    void sampleProc( int tickus,
                     sampleThread *pCallThread );
    bool proxySample( sampleProxy *pProxy, int tickus, spyItem *pItem );

protected:
    void commitProc( QList< sampleProxy *> &commitList,
                     QList< spyItem > &valList );

    sampleProxy *find( sampleProxy & proxy );

public:
    QList< sampleProxy *> mSamples;
protected:
    dbMeta *m_pDbMeta;
    MegaDevice::InstMgr *m_pInstMgr;
};

#endif // SAMPLEMGR_H
