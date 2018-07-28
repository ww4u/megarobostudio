#ifndef INSTCLIENT_H
#define INSTCLIENT_H

#include <QTcpSocket>

namespace MegaDevice
{

class instClient
{
public:
    instClient();
    ~instClient();

public:
    int open( const QString &name,
              const QString &server="127.0.0.1",
              const quint16 port=1234 );
    int close();

    int write( const char *data, int len );
    int size( int tmo = 2000 );
    int read( char *pData, int len );
    QByteArray readAll( int tmo = 20000 );

public:
    QString name();

protected:
    QByteArray mRecvBuf;
    QTcpSocket mSocket;
    QString mName;

};

}

#endif // INSTCLIENT_H
