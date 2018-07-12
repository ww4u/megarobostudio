#include "rpcthread.h"

RpcThread::RpcThread(const QString &exec,
                     const QStringList &arg,
                     QObject *parent) : QThread(parent)
{
    mExec = exec;
    mArg = arg;

    connect( this, SIGNAL(finished()),
             this, SLOT(slot_finished()) );
}

void RpcThread::run()
{
    QProcess::execute( mExec, mArg );

    emit signal_completed( this );
}

void RpcThread::slot_finished()
{
    emit signal_completed( this );
}
