#ifndef INSTSERVER_H
#define INSTSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QSignalMapper>
#include <QMutex>

#define LISTEN_PORT     1234

namespace MegaDevice
{

class instServer : public QObject
{
    Q_OBJECT
public:
    instServer( QObject *parent = NULL );
    virtual ~instServer();
public:
    //! to some device
    virtual void dataIn( QTcpSocket *socket,
                           const QString &name,
                           QByteArray &ary );

    virtual void dataOut( void *pPara, const char *data, int len );

    virtual void setObjPara( const QString &name, void *pPara );

protected Q_SLOTS:
//    void slot_newConnection();
    void slot_newConnection( QObject *pObj );
    void slot_readyRead( QObject *pObj );
    void slot_destroyConnection( QObject *pObj );

public:
    bool start( quint32 port = LISTEN_PORT,
                int portCnt = 1 );
    void stop();
    bool isListening();

protected:
    void clear();

protected:
//    QTcpServer mServer;

    QList< QTcpServer *> mServerList;

    QSignalMapper mSignalMap;
    QSignalMapper mServerMap;

    QList<QTcpSocket *> mServiceSockets;
    QMutex mServiceMutex;
};

}

#endif // INSTSERVER_H
