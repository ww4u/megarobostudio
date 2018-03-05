#ifndef _MY_DEBUG_H_
#define _MY_DEBUG_H_

#include <QDebug>

#define logDbg()	qDebug()<<__FUNCTION__<<__FILE__<<__LINE__
#define logWarning()    qDebug()<<"!!!Warning:"<<__FUNCTION__<<__FILE__<<__LINE__
#define logError()      qDebug()<<"!!!Error:"<<__FUNCTION__<<__FILE__<<__LINE__

#define begin_time()    { QTime time; time.start();
#define end_time()        logDbg()<<"Time Elapsed:"<<time.elapsed(); }
#endif
