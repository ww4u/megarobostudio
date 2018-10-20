#include "rpcmanager.h"
#include "../../include/mcdef.h"
RpcManager::RpcManager(QThread *parent) : QThread(parent)
{
}

RpcManager::~RpcManager()
{
    delete_all( mWorkerList );
}

//! create
int RpcManager::rpc( const QString &script )
{
    RpcWorker *pWorker;

    //! find
    pWorker = findWorker( script );
    if ( NULL == pWorker )
    {
        pWorker = createWorker( script );
        if ( NULL == pWorker )
        {
            return -1;
        }
        //! register the worker
        else
        {
            lockMgr();
                mWorkerList.append( pWorker);
            unlockMgr();
        }
    }
    else
    {
        //! running
        if ( pWorker->isRunning() )
        {
            return -2;
        }
        //! start again
        else
        { pWorker->start(); }
    }

    return 0;
}

//! query
//! status,ret
//! error, -1
//! running, 0
//! finished, ret
QString RpcManager::rpq( const QString &script )
{
    RpcWorker *pWorker = findWorker( script );
    if ( NULL == pWorker )
    { return QString("error|-1"); }

    return QString("%1|%2").arg( pWorker->state() ).arg( pWorker->getRet() );
}

//! kill
int RpcManager::rpk( const QString &script )
{
    RpcWorker *pWorker = findWorker( script );
    if ( NULL == pWorker )
    { return -1; }

    if ( pWorker->isRunning() )
    {}
    else
    { return 0; }

    pWorker->terminate();
    pWorker->wait();

    return 0;
}

//! rpc list
QStringList RpcManager::rpls()
{
    QStringList rpcList, snapshotList;
    foreach (RpcWorker *pWorker, mWorkerList)
    {
        Q_ASSERT( NULL != pWorker );

        snapshotList = pWorker->snapShot();

        rpcList<<snapshotList.join('|');
    }

    return rpcList;
}

void RpcManager::rpgc()
{
    //! collect
    QList<RpcWorker *> gcList;
    foreach (RpcWorker *pWorker, mWorkerList)
    {
        Q_ASSERT( NULL != pWorker );

        if ( pWorker->isRunning() )
        {}
        else
        {
            gcList.append( pWorker );
        }
    }

    //! release
    foreach( RpcWorker *pWorker, gcList )
    {
        mWorkerList.removeAll( pWorker );
    }

    delete_all( gcList );

}

void RpcManager::slot_worker_ret( QObject *pObj, int ret )
{
//    Q_ASSERT( NULL != pObj );

//    delete pObj;
}

RpcWorker* RpcManager::createWorker( const QString &script )
{
    //! check size
    if ( mWorkerList.size() > 64 )
    { rpgc(); }

    RpcWorker *pWorker = new RpcWorker();

    if ( NULL == pWorker )
    { return NULL; }

    pWorker->setScript( script );

    connect( pWorker, SIGNAL(signal_ret(QObject*,int)),
             this, SLOT(slot_worker_ret(QObject*,int)) );

    pWorker->start();

    return pWorker;
}

RpcWorker* RpcManager::findWorker( const QString &script )
{
    foreach ( RpcWorker *pWorker, mWorkerList)
    {
        Q_ASSERT( NULL != pWorker );

        if ( str_equ( script, pWorker->script()) )
        { return pWorker; }
    }

    return NULL;
}

void RpcManager::lockMgr()
{
    mMgrMutex.lock();
}
void RpcManager::unlockMgr()
{
    mMgrMutex.unlock();
}
