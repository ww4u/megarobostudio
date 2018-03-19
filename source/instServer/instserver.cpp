#include "instserver.h"
#include <QDebug>


namespace MegaDevice
{

instServer::instServer( QObject *parent ) : QObject( parent )
{
    connect( &mServer, SIGNAL(newConnection()),
             this, SLOT(slot_newConnection()) );
}

//! data to device
void instServer::dataIn( QTcpSocket *pSocket,
                           const QString &name,
                           QByteArray &ary )
{
    qDebug()<<__FUNCTION__<<__LINE__<<ary;
    pSocket->write( ary );
}

void instServer::dataOut( void *pPara, const char *data, int len )
{
    Q_ASSERT( NULL != pPara );

    QTcpSocket *pSocket = (QTcpSocket*)pPara;
    pSocket->write( data, len );
}

void instServer::setObjPara( const QString &name, void *pPara )
{

}

void instServer::slot_newConnection()
{
    QTcpSocket *pSocket;
    pSocket = mServer.nextPendingConnection();
    pSocket->setObjectName("");

    qDebug()<<__FUNCTION__<<__LINE__<<pSocket->peerAddress()<<pSocket->peerName();

    //! ready ready
    connect( pSocket,
             SIGNAL(readyRead()),
             &mSignalMap,
             SLOT(map()) );
    mSignalMap.setMapping( pSocket, pSocket );

    connect( &mSignalMap, SIGNAL(mapped(QObject*)),
             SLOT(slot_readyRead(QObject*)));

    //! disconnect
    connect( pSocket, SIGNAL(disconnected()),
             pSocket, SLOT(deleteLater()));

}

void instServer::slot_readyRead( QObject *pObj )
{
    Q_ASSERT( NULL != pObj );

    QTcpSocket *pSocket = (QTcpSocket *)pObj;
qDebug()<<__FUNCTION__<<__LINE__;
    //! set obj name
    //! do not set name
    if ( pSocket->objectName().length() < 1 )
    {
        if ( pSocket->canReadLine() )
        {
            QByteArray ary = pSocket->readLine();
            pSocket->setObjectName( ary.trimmed() );
            qDebug()<<__FUNCTION__<<__LINE__<<ary;
        }
    }

    //! have set name
    if ( pSocket->objectName().length() > 0 )
    {
        //! according the name
        QByteArray ary = pSocket->readAll();
        dataIn( pSocket, pObj->objectName(), ary );
        qDebug()<<__FUNCTION__<<__LINE__<<ary;
    }
}

void instServer::start( quint32 port )
{
    if ( mServer.listen( QHostAddress::Any, port ) )
    {
        qDebug()<<__FUNCTION__<<__LINE__<<mServer.serverAddress()<<mServer.serverPort();
        qDebug()<<mServer.errorString();
    }
    else
    { qDebug()<<mServer.errorString(); }
}
void instServer::stop()
{
    mServer.close();
}
bool instServer::isListening()
{
    return mServer.isListening();
}

}
