
#include "../../sys/sysapi.h"
#include "mainwindow.h"

static sysRunTime _sysRunTime;

void _sysLog( const QString &preStr,
              const QString &str,
              const QString &str1,
              const QString &str2,
              const QString &str3,
              const QString &str4
              )
{
    //! pre
    QString catedString = preStr;

    if ( str.length() > 0 )
    { catedString += " " + str; }
    if ( str1.length() > 0 )
    { catedString += " " + str1; }
    if ( str2.length() > 0 )
    { catedString += " " + str2; }
    if ( str3.length() > 0 )
    { catedString += " " + str3; }
    if ( str4.length() > 0 )
    { catedString += " " + str4; }

    if ( catedString.length() > 0 && sysQueue()!= NULL )
    {
        sysQueue()->postMsg( e_logout, catedString );
    }
}

void sysLog( const QString &str,
                         const QString &str1,
                         const QString &str2,
                         const QString &str3,
                         const QString &str4
                          )
{
    _sysLog( "", str, str1, str2, str3, str4 );
}

void sysWarn( const QString &str,
                         const QString &str1,
                         const QString &str2,
                         const QString &str3,
                         const QString &str4
                          )
{
    _sysLog( "!!!Warnning", str, str1, str2, str3, str4 );
}

void sysError( const QString &str,
                         const QString &str1,
                         const QString &str2,
                         const QString &str3,
                         const QString &str4
                          )
{
    _sysLog( "!!!Error", str, str1, str2, str3, str4 );
}

void sysProgress( int n,
                  const QString &info,
                  int ma, int mi
                   )
{
    sysQueue()->postMsg( e_progress_para, mi, ma, n, info );

}
void sysProgress( bool b )
{
    sysQueue()->postMsg( e_progress_visible, b);
}

void sysStatus( const QString &str )
{
    sysQueue()->postMsg( e_status, str );
}

void attachSysQueue( RoboMsgThread* pQueue )
{
    Q_ASSERT( NULL != pQueue );
    _sysRunTime.m_pSysQueue = pQueue;
}

RoboMsgThread* sysQueue()
{ return _sysRunTime.m_pSysQueue; }
