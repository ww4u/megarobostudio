#ifndef SAMPLETHREAD_H
#define SAMPLETHREAD_H

#include <QThread>
#include "samplemgr.h"

class sampleThread : public QThread
{
    Q_OBJECT
public:
    sampleThread( QObject *pObj = 0 );
    ~sampleThread();

Q_SIGNALS:
    void sigSpyUpdated( const QString &name,
                        int item,
                        int size );

public:
    void setSampleInterval( int interval );

    void attachDbMeta( dbMeta *pMeta );
    void attachInstMgr( MegaDevice::InstMgr *pInstMgr );

    void attachSampleProxy( MRQ_REPORT_STATE stat,
                            const QString &chName,
                            bool bRun = true,
                            int cacheSize = 1024,
                            int intervalus = 1000000
                            );

    void attachSampleProxy( sampleProxy & proxy );

    sampleProxy *getSampleProxy( int stat,
                                 const QString &chName );

    void clear();
    void stop();

protected:
    virtual void run();

protected:
    QMutex mThreadMutex;

    sampleMgr mSampleMgr;
    int mSampleInterval;    //! us

    MegaDevice::InstMgr *m_pInstMgr;
};

#endif // SAMPLETHREAD_H
