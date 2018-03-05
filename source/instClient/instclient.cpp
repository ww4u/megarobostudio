#include "instclient.h"
#include <QDebug>

namespace MegaDevice
{

instClient::instClient(  )
{
//    connect( &mSocket, SIGNAL(readyRead()),
//             this, SLOT(slot_readyRead()) );

//    connect( &mSocket, SIGNAL(connected()),
//             this, SLOT(slot_connected()));
}

//void instClient::slot_readyRead()
//{
//    mRecvBuf = mSocket.readAll();
//    qDebug()<<__FUNCTION__<<__LINE__<<mRecvBuf;
//}

//void instClient::slot_connected()
//{
////    mSocket.write( mName.toLatin1() );
//    qDebug()<<__FUNCTION__<<__LINE__;
//}

int instClient::open( const QString &name,
                      const QString &server,
                      const quint16 port)
{
    if ( mSocket.isOpen() )
    { mSocket.close();}

    mSocket.connectToHost( server, port );

    if ( mSocket.waitForConnected() )
    {
        QString latName = name + "\n";
        mSocket.write( latName.toLatin1() );
        qDebug()<<latName;
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
    mSocket.close();
    return 0;
}

int instClient::write( const char *data, int len )
{
    if ( mSocket.isValid() )
    {}
    else
    { return 0; }

    return mSocket.write( data, len );
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

}
