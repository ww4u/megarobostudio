#ifndef RPCMANAGER_H
#define RPCMANAGER_H

#include <QThread>
#include <QSignalMapper>
#include <QMutex>

#include "rpcworker.h"

class RpcManager : public QThread
{
    Q_OBJECT
public:
    explicit RpcManager(QThread *parent = nullptr);
    ~RpcManager();
signals:

public:
    int rpc( const QString &script );
    QString rpq( const QString &script );
    int rpk( const QString &script );
    QStringList rpls();
    void rpgc();

protected slots:
    void slot_worker_ret( QObject *pObj, int ret );

protected:
    RpcWorker* createWorker( const QString &script );
    RpcWorker* findWorker( const QString &script );

    void lockMgr();
    void unlockMgr();

protected:
    QList<RpcWorker *> mWorkerList;
    QMutex mMgrMutex;

};

#endif // RPCMANAGER_H
