#ifndef _SYS_API_H_
#define _SYS_API_H_

#include <QtCore>

#include "../../com/robomsgthread.h"

void attachSysLog( QWidget *pLogWig );

void _sysLog( const QString &preStr = "",

              const QString &str = "",
              const QString &str1="",
              const QString &str2="",
              const QString &str3="",
              const QString &str4=""

              );

void sysLog( const QString &str, 
						 const QString &str1="",
						 const QString &str2="",
						 const QString &str3="",
						 const QString &str4=""
						  );

void sysWarn( const QString &str,
                                                 const QString &str1="",
                                                 const QString &str2="",
                                                 const QString &str3="",
                                                 const QString &str4=""
                                                  );

void sysError( const QString &str,
                                                 const QString &str1="",
                                                 const QString &str2="",
                                                 const QString &str3="",
                                                 const QString &str4=""
                                                  );

void attachSysQueue( RoboMsgThread* pQueue );
RoboMsgThread* sysQueue();

class sysRunTime
{
public:
    QWidget *m_pSysLog;
    RoboMsgThread *m_pSysQueue;
};

#endif
