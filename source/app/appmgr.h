#ifndef APPMGR_H
#define APPMGR_H

#include "../model/appmodel.h"
#include "appprocess.h"

class AppMgr : public QObject
{
    Q_OBJECT
public:
    AppMgr( QObject *pParent = Q_NULLPTR );
    ~AppMgr();

Q_SIGNALS:
    void signal_output( QByteArray ary );

protected slots:
    void slot_finished( QProcess *pObj, int exitCode, QProcess::ExitStatus exitStatus );
    void slot_finished( QProcess *pObj, QProcess::ProcessError error );

    void slot_readyReadStandardError( QProcess *pObj );
    void slot_readyReadStandardOutput( QProcess *pObj );

public:
    int startApp( AppItem *pItem );
    int stopApp( AppItem *pItem );

    void startup();

protected:
    AppProcess* findApp( AppItem *pItem );
    void connectApp( AppProcess *pProc );

protected:
    QList<AppProcess *> mAppList;
};

#endif // APPMGR_H
