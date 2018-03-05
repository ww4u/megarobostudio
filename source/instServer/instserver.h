#ifndef INSTSERVER_H
#define INSTSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QSignalMapper>

namespace MegaDevice
{

class instServer : public QObject
{
    Q_OBJECT
public:
    instServer( QObject *parent = NULL );

public:
    //! to some device
    virtual void dataIn( QTcpSocket *socket,
                           const QString &name,
                           QByteArray &ary );

    virtual void dataOut( void *pPara, const char *data, int len );

    virtual void setObjPara( const QString &name, void *pPara );

protected Q_SLOTS:
    void slot_newConnection();
    void slot_readyRead( QObject *pObj );

public:
    void start();
    void stop();

protected:
    QTcpServer mServer;
    QSignalMapper mSignalMap;
};

}

#endif // INSTSERVER_H
