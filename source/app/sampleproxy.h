#ifndef SAMPLEPROXY_H
#define SAMPLEPROXY_H

#include "../../device/mrq/deviceMRQ.h"
#include "../model/spyitemmodel.h"

class sampleTimer
{
public:
    sampleTimer( int interval = time_s(10) );

    sampleTimer &operator=( const sampleTimer &samp );
public:
    void start();
    void stop();

    void setInterval( int interval );
    int getInterval();

    bool tick( int time );

public:
    int mPeriod;
    int mNow;
    bool mbRun;
};

class sampleProxy
{
public:
    sampleProxy( );
    sampleProxy( const sampleProxy &proxy );
    sampleProxy( sampleProxy *proxy );

    ~sampleProxy( );

    sampleProxy &operator=( const sampleProxy &proxy );
    bool isLike( const sampleProxy &proxy );

    void attach( MRQ_REPORT_STATE stat,
                 const QString &chName,
                 bool bRun = true,
                 int cacheSize = 1024,
                 int interval = 1000000 );

    void start();
    void stop();

    void clear();

    void setChName( const QString &chName );
    QString chName();

    //! ticks -- elapsed
    bool sample( int tickus,
                 spyItem *pItem,

                 MegaDevice::deviceMRQ *pMrq,
                 int ax );

    int logInCache( spyItem *pItem );
    int cacheLen();

public:
    QString mChName;
    int mCacheSize;

    MRQ_REPORT_STATE mItem;
    sampleTimer mTimer;

    quint32 mSignature;
    qint32 mAx;
                                //! items
    QQueue<spyItem *> mSpyCache;

};

#endif // SAMPLEPROXY_H
