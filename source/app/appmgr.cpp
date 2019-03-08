#include "appmgr.h"
#include "../../include/mcdef.h"
#include "../../include/mydebug.h"

AppMgr::AppMgr( QObject *pParent ) : QObject( pParent )
{

}

AppMgr::~AppMgr()
{
    foreach( AppProcess *pProc, mAppList )
    { pProc->kill(); }

    foreach( AppProcess *pProc, mAppList )
    { pProc->waitForFinished(); }

    delete_all( mAppList );
}

void AppMgr::slot_finished( QProcess *pObj, int exitCode, QProcess::ExitStatus exitStatus )
{
    Q_ASSERT( NULL != pObj );

    delete pObj;
    mAppList.removeAll( (AppProcess*)pObj );

    logDbg()<<exitCode<<(int)exitStatus;
}
void AppMgr::slot_finished( QProcess *pObj, QProcess::ProcessError error )
{
    Q_ASSERT( NULL != pObj );

    delete pObj;
    mAppList.removeAll( (AppProcess*)pObj );

    logDbg()<<(int)error;
}

void AppMgr::slot_readyReadStandardError( QProcess *pObj )
{
    emit signal_output( pObj->readAllStandardError() );
}
void AppMgr::slot_readyReadStandardOutput( QProcess *pObj )
{
    emit signal_output( pObj->readAllStandardOutput() );
}

int AppMgr::startApp( AppItem *pItem )
{
    Q_ASSERT( NULL != pItem );

    //! find
    if ( NULL != findApp(pItem) )
    { return -1; }

    //! create new
    AppProcess *pApp;
    pApp = new AppProcess();
    if ( NULL == pApp )
    { return -2; }

    //! arg
    pApp->setProgram( pItem->program() );
    QStringList args;
    if ( pItem->argument().size() > 0 )
    {
        args<<pItem->argument();
        pApp->setArguments( args );
    }

    //! connect
    connectApp( pApp );

    mAppList.append( pApp );

    pApp->start();

    return 0;
}
int AppMgr::stopApp( AppItem *pItem )
{
    QProcess *pProc = findApp( pItem );
    if ( NULL != pProc )
    {
        logDbg();
        pProc->kill();
        return 0;
    }
    else
    { logDbg(); return -1; }
}

AppProcess* AppMgr::findApp( AppItem *pItem )
{
    foreach( AppProcess *pProc, mAppList )
    {
        if ( str_equ( pProc->program(), pItem->program() ) )
        { }
        else
        { continue; }

        //! for argument
        if ( pItem->argument().size() > 0
             && pProc->arguments().size()==1
             && str_equ( pItem->argument(), pProc->arguments().at(0) ) )
        { return pProc; }
    }

    return NULL;
}

void AppMgr::connectApp( AppProcess *pProc )
{
    Q_ASSERT( NULL != pProc );
    connect( pProc, SIGNAL(signal_finished(QProcess*,int,QProcess::ExitStatus)),
             this, SLOT(slot_finished(QProcess*,int,QProcess::ExitStatus)));
    connect( pProc, SIGNAL(signal_finished(QProcess*,QProcess::ProcessError)),
             this, SLOT(slot_finished(QProcess*,QProcess::ProcessError)));

    connect( pProc, SIGNAL(signal_readyReadStandardError(QProcess*)),
             this, SLOT(slot_readyReadStandardError(QProcess*)));
    connect( pProc, SIGNAL(signal_readyReadStandardOutput(QProcess*)),
             this, SLOT(slot_readyReadStandardOutput(QProcess*)));

}
