#include "instclient.h"
#include <QDebug>

namespace MegaDevice
{

instClient::instClient(  )
{
}

instClient::~instClient(  )
{
    close();
}

int instClient::open( const QString &name,
                      const QString &server,
                      const quint16 port)
{
    if ( mSocket.isOpen() )
    { mSocket.close();}

    mSocket.connectToHost( server, port );

    if ( mSocket.waitForConnected() )
    {
        QString latName = name + "\r\n";
        mSocket.write( latName.toLatin1() );
        mSocket.flush();

        mName = name;

        return 0;
    }
    else
    {
        qDebug()<<__FUNCTION__<<__LINE__;
        return -1;
    }
}
int instClient::close()
{
    if ( mSocket.isOpen() )
    {
        mSocket.close();
        mName.clear();
    }
    return 0;
}

int instClient::write( const char *data, int len )
{
    if ( mSocket.isValid() )
    {}
    else
    { return 0; }

    qint64 ret;
    ret = mSocket.write( data, len );

    mSocket.flush();
    return ret;
}
int instClient::size( int tmo )
{
    if ( mSocket.waitForReadyRead(tmo) )
    {}

    return mSocket.size();
}
int instClient::read( char *pData, int len )
{
    mRecvBuf = mSocket.readAll();

    if ( len > mRecvBuf.size() )
    { len = mRecvBuf.size(); }

    if ( len > 0 )
    {
        memcpy( pData, mRecvBuf.data(), len );
    }

    return len;
}

QByteArray instClient::readAll( int tmo )
{
    if ( mSocket.waitForReadyRead(tmo) )
    {}

    return mSocket.readAll();
}

QString instClient::name()
{ return mName; }

}
