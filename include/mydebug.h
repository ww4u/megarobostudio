#ifndef _MY_DEBUG_H_
#define _MY_DEBUG_H_


#include <QDebug>

#define logDbg()	qDebug()<<__FUNCTION__<<__FILE__<<__LINE__
#define logWarning()    qDebug()<<"!!!Warning:"<<__FUNCTION__<<__FILE__<<__LINE__
#define logError()      qDebug()<<"!!!Error:"<<__FUNCTION__<<__FILE__<<__LINE__

//#define logDbg()	QT_NO_QDEBUG_MACRO()<<__FUNCTION__<<__FILE__<<__LINE__
//#define logWarning()    QT_NO_QDEBUG_MACRO()<<"!!!Warning:"<<__FUNCTION__<<__FILE__<<__LINE__
//#define logError()      QT_NO_QDEBUG_MACRO()<<"!!!Error:"<<__FUNCTION__<<__FILE__<<__LINE__

#define begin_time()    { QTime time; time.start();
#define end_time()        logDbg()<<"Time Elapsed:"<<time.elapsed(); }

#define SYS_LOG()       sysLog( __FILE__, __FUNCTION__, QString::number(__LINE__) );
#define SYS_LOG_V( v )       sysLog( __FILE__, __FUNCTION__, QString::number(__LINE__),\
                                                                QString::number(v) );
#define SYS_LOG_V2( v1,v2 )       sysLog( __FILE__, __FUNCTION__, QString::number(__LINE__),\
                                                                QString::number(v1),\
                                                                QString::number(v2));
#endif
