#ifndef RPCWORKER_H
#define RPCWORKER_H

#include <QThread>

class RpcWorker : public QThread
{
    Q_OBJECT
public:
    explicit RpcWorker(QThread *parent = nullptr);

public:
    void setScript( const QString &str );
    QString script();

    int getRet();

    QString state();
    QStringList snapShot();

signals:
    void signal_ret( QObject *pObj, int e );

protected slots:
    void slot_finished();

protected:
    virtual void run();

protected:
    QString mScript;
    int mRet;

};

#endif // RPCWORKER_H
