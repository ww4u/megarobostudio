#include "instserver.h"
#include <QDebug>


namespace MegaDevice
{

instServer::instServer( QObject *parent ) : QObject( parent )
{
//    connect( &mServer, SIGNAL(newConnection()),
//             this, SLOT(slot_newConnection()) );
}
instServer::~instServer()
{
    clear();
}
//! data to device
void instServer::dataIn( QTcpSocket *pSocket,
                         const QString &name,
                         QByteArray &ary )
{
//    qDebug()<<__FUNCTION__<<__LINE__<<ary;
    pSocket->write( ary );
}

void instServer::dataOut( void *pPara, const char *data, int len )
{
    Q_ASSERT( NULL != pPara );

    QTcpSocket *pSocket = (QTcpSocket*)pPara;
    pSocket->write( data, len );
    pSocket->flush();
}

void instServer::setObjPara( const QString &name, void *pPara )
{

}

//void instServer::slot_newConnection()
//{
//    QTcpSocket *pSocket;
//    pSocket = mServer.nextPendingConnection();
//    pSocket->setObjectName("");

////    qDebug()<<__FUNCTION__<<__LINE__<<pSocket->peerAddress()<<pSocket->peerName();

//    //! ready ready
//    connect( pSocket,
//             SIGNAL(readyRead()),
//             &mSignalMap,
//             SLOT(map()) );
//    mSignalMap.setMapping( pSocket, pSocket );

//    connect( &mSignalMap, SIGNAL(mapped(QObject*)),
//             SLOT(slot_readyRead(QObject*)));

//    //! destroy
//    mServiceMutex.lock();
//        mServiceSockets.append( pSocket );
//        connect( pSocket, SIGNAL(destroyed(QObject*)),
//                 this, SLOT(slot_destroyConnection(QObject *)));
//    mServiceMutex.unlock();

//    //! disconnect
//    connect( pSocket, SIGNAL(disconnected()),
//             pSocket, SLOT(deleteLater()));

//}

void instServer::slot_newConnection( QObject *pObj )
{
    //! convert server
    if ( NULL == pObj )
    { return; }
    QTcpServer *pServer = (QTcpServer*)pObj;

    //! socket
    QTcpSocket *pSocket;
    pSocket = pServer->nextPendingConnection();
    if ( pSocket == NULL )
    { return; }

    pSocket->setObjectName("");

//    qDebug()<<__FUNCTION__<<__LINE__<<pSocket->peerAddress()<<pSocket->peerName();

    //! ready ready
    connect( pSocket,
             SIGNAL(readyRead()),
             &mSignalMap,
             SLOT(map()) );
    mSignalMap.setMapping( pSocket, pSocket );

    connect( &mSignalMap, SIGNAL(mapped(QObject*)),
             SLOT(slot_readyRead(QObject*)));

    //! destroy
    mServiceMutex.lock();
        mServiceSockets.append( pSocket );
        connect( pSocket, SIGNAL(destroyed(QObject*)),
                 this, SLOT(slot_destroyConnection(QObject *)));
    mServiceMutex.unlock();

    //! disconnect
    connect( pSocket, SIGNAL(disconnected()),
             pSocket, SLOT(deleteLater()));

    qDebug()<<__FUNCTION__<<__LINE__;
}

void instServer::slot_readyRead( QObject *pObj )
{
    Q_ASSERT( NULL != pObj );
    QTcpSocket *pSocket = (QTcpSocket *)pObj;
    Q_ASSERT( NULL != pSocket );

//qDebug()<<__FUNCTION__<<__LINE__;
    //! set obj name
    //! do not set name
    if ( pSocket->objectName().length() < 1 )
    {
        if ( pSocket->canReadLine() )
        {
            QByteArray ary = pSocket->readLine();
            pSocket->setObjectName( ary.trimmed() );
//            qDebug()<<__FUNCTION__<<__LINE__<<ary;
        }
    }

    //! have set name
    if ( pSocket->objectName().length() > 0 )
    {
        QByteArray ary;
        while( pSocket->canReadLine() )
        {
            ary = pSocket->readLine();
            if ( ary.length() > 0 )
            {
//                qDebug()<<__FUNCTION__<<__LINE__<<ary;
                dataIn( pSocket, pObj->objectName(), ary );
            }
        }
    }
}

void instServer::slot_destroyConnection( QObject *pObj )
{
    Q_ASSERT( NULL != pObj );

    mServiceMutex.lock();
        QTcpSocket *pSocket = (QTcpSocket*)pObj;
        mServiceSockets.removeAll( pSocket );
    mServiceMutex.unlock();
}

bool instServer::start( quint32 port, int portCnt )
{
//    return mServer.listen( QHostAddress::Any, port );

    bool bRet;

    clear();

    //! create listen
    QTcpServer *pServer;
    for ( int i = 0; i < portCnt; i++ )
    {
        pServer = new QTcpServer();
        if ( NULL == pServer )
        { return false; }
//qDebug()<<__FUNCTION__<<__LINE__<<i<<(quint32)pServer;
        mServerList.append( pServer );

        bRet = pServer->listen( QHostAddress::Any, port + i );
        if ( bRet )
        {}
        else
        { return false; }

        connect( pServer,
                 SIGNAL( newConnection()),
                 &mServerMap,
                 SLOT(map()) );
        mServerMap.setMapping( pServer, pServer );

        connect( &mServerMap, SIGNAL(mapped(QObject*)),
                 SLOT(slot_newConnection(QObject*)));

    }

    return true;
}
void instServer::stop()
{
    //! for the sockets
    mServiceMutex.lock();
        foreach( QTcpSocket *pSock, mServiceSockets )
        {
            Q_ASSERT( NULL != pSock );
            pSock->close();
        }
        mServiceSockets.clear();
    mServiceMutex.unlock();

    //! server
    clear();
}
bool instServer::isListening()
{
//    return mServer.isListening();
    return mServerList.size() > 0 ;
}

void instServer::clear()
{
    //! close all
    foreach (QTcpServer *pSocket, mServerList )
    {
        Q_ASSERT( NULL != pSocket );
        pSocket->close();
    }

    //! clear the list
    qDeleteAll( mServerList );
    mServerList.clear();
}

}
