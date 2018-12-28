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
    //! \note do not know why fail
//    QProcess::execute( mExec, mArg );

    QProcess proc;

    proc.start( mExec, mArg );

    proc.waitForFinished( -1 );

    emit signal_completed( this );
}

void RpcThread::slot_finished()
{
    emit signal_completed( this );
}
