#ifndef _SYS_API_H_
#define _SYS_API_H_

#include <QtCore>

#include "../com/robomsgthread.h"
#include "../../app/systimer.h"

//! sys log
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

//! sys progress
void sysProgress( int n,
                  const QString &info="",
                  int ma = 100, int mi = 0
                  );
void sysProgress( bool b );

//! sys status
void sysStatus( const QString &str );

void sysPrompt( const QString &str );

void sysRpc( const RpcRequest &rpc );

//! sys queue
void attachSysQueue( RoboMsgThread* pQueue );
RoboMsgThread* sysQueue();

quint64 sysTimeStamp();

void sysPauseTimer();
void sysRestartTimer();

class sysRunTime
{
public:
    RoboMsgThread *m_pSysQueue;
};

#endif
