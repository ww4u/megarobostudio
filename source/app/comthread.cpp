#include "comthread.h"

#include "../../include/mcstd.h"
#include "../../sys/sysapi.h"
ComThread::ComThread(QObject *parent) : QThread(parent)
{

}

void ComThread::run()
{
    //! port info
    mPort.setPortName( mPortName );
    mPort.setBaudRate( QSerialPort::Baud115200 );
    mPort.setDataBits( QSerialPort::Data8 );
    mPort.setStopBits( QSerialPort::OneStop );
    mPort.setParity( QSerialPort::NoParity );
    mPort.setFlowControl( QSerialPort::NoFlowControl );

    //! open success
    if ( mPort.open( QIODevice::ReadWrite ) )
    { sysLog( mPortName, tr("Open Success") ); }
    else
    {
        sysError( mPortName, tr("Open fail") );
        return;
    }

    QString strRecv;
    QByteArray bAry;
    forever
    {
        if ( isInterruptionRequested() )
        { break; }

        //! have data
        if ( mPort.waitForReadyRead(1000) || mPort.bytesAvailable() > 0 )
        {
            bAry.append( mPort.readAll() ); logDbg()<<bAry;
        }
        //! timeout
        else
        {
            //! has data
            if ( bAry.size() > 0 )
            {
                strRecv.clear();
                strRecv.append( bAry );
                emit signal_receive( strRecv );

                bAry.clear();
            }
            else
            { }
        }

    }

    mPort.close();
}

void ComThread::setPort( const QString &str )
{ mPortName = str; }
QString ComThread::port()
{ return mPortName; }

void ComThread::slot_transmit( const QByteArray &ary )
{
    mPort.write( ary );
}

