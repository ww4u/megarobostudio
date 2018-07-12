#ifndef RPCTHREAD_H
#define RPCTHREAD_H

#include <QtCore>

class RpcThread : public QThread
{
    Q_OBJECT
public:
    explicit RpcThread(
                        const QString &exec,
                        const QStringList &arg,
                        QObject *parent = nullptr );

protected:
    virtual void run();

Q_SIGNALS:
    void signal_completed( RpcThread *pThread );

protected Q_SLOTS:
    void slot_finished();

protected:
    QString mExec;
    QStringList mArg;
};

#endif // RPCTHREAD_H
