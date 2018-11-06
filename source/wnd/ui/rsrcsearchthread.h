#ifndef RSRCSEARCHTHREAD_H
#define RSRCSEARCHTHREAD_H

#include <QThread>

class RsrcSearchThread : public QThread
{
public:
    RsrcSearchThread( QObject *parent = Q_NULLPTR );

protected:
    virtual void run();

public:
    void setOption( int type, const QString & patt );
    QStringList searchList();

protected:
    void search();

protected:
    int mPortType;
    QString mRsrcPattern;
    QStringList mSearchList;

};

#endif // RSRCSEARCHTHREAD_H
