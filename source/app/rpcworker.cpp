#include "rpcworker.h"
#include <QProcess>
#include "../../include/mydebug.h"

RpcWorker::RpcWorker(QThread *parent) : QThread(parent)
{
    connect( this, SIGNAL(finished()),
             this, SLOT(slot_finished()) );

    mRet = 0;
}

void RpcWorker::setScript( const QString &str )
{ mScript = str; }
QString RpcWorker::script()
{ return mScript; }

int RpcWorker::getRet()
{ return mRet; }

QString RpcWorker::state()
{
    if ( isRunning() )
    { return "running"; }
    else
    { return "finished"; }
}

QStringList RpcWorker::snapShot()
{
    QStringList tList;

    tList<<mScript<<state()<<QString::number(mRet);

    return tList;
}

void RpcWorker::slot_finished()
{ emit signal_ret( this, mRet); }

void RpcWorker::run()
{
    logDbg()<<mScript;
    mRet = QProcess::execute( mScript );
    logDbg()<<mRet;
}

