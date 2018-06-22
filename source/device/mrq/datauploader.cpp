
#include "datauploader.h"

#include "deviceMRQ.h"
#include "../bus/receivecache.h"

DataUploader::DataUploader( QObject *pObj ) : QThread( pObj )
{
    m_pCache = NULL;
    m_pDev = NULL;
    mChId = 0;

    mTickms = time_ms(800);
    mTmoms = time_s( (10*60) );

    mState = 0;
    mCallRet = -1;          //! unk
}

DataUploader::~DataUploader( )
{}

void DataUploader::kickoff( Priority pri )
{
    mState = -1;
    QThread::start( pri );
}

int DataUploader::state()
{ return mState; }

int DataUploader::callRet()
{ return mCallRet; }

void DataUploader::run()
{
    //! not idle
    mState = 1;

    sysProgress( 0, "loading" );
    sysProgress( true );
    mCallRet = uploadProc();
    sysProgress( false );

    //! idle
    mState = 0;

    sysLog( __FUNCTION__, QString::number(__LINE__), QString::number(mCallRet) );
}

void DataUploader::requestLoad(
                  receiveCache *pReceive,
                  MegaDevice::deviceMRQ *pDev,
                  int chId,
                  const QString &fileName )
{
    Q_ASSERT( NULL != pReceive );
    Q_ASSERT( NULL != pDev );

    m_pCache = pReceive;
    m_pDev = pDev;
    mChId = chId;
    mFileOutName = fileName;
}

int DataUploader::uploadProc()
{
    Q_ASSERT( NULL != m_pDev && NULL != m_pCache );

    int ret;

    //! 1. get len
    quint16 stepCount;
    ret = m_pDev->getPDM_MICSTEPCOUNT( mChId, &stepCount );
    if ( ret != 0 )
    { return -1; }

    if ( stepCount > 0 )
    {}
    else
    { return -1; }

    //! 2. request the data
    ret = m_pDev->requestPDM_MICSTEPDATA( mChId, 0, stepCount );
    if ( ret != 0 )
    { return -2; }

    int tmo = mTmoms;
    int bytes = 0;
    int totalSize = stepCount * 2;
    while( tmo > 0 && bytes < totalSize )
    {
        QThread::usleep( mTickms );

        //! get frame len
        bytes = m_pCache->frameBytes();
        sysLog( __FUNCTION__, QString::number(__LINE__), QString::number(bytes) );
//        emit sig_progress( bytes, 0, totalSize );
        sysProgress( bytes, "loading",  totalSize, 0 );

        tmo -= mTickms;
    }

    //! check tmo
    if ( tmo > 0 )
    { }
    else
    { return -3; }

    //! 3. save file
    QFile file( mFileOutName );
    if ( file.open( QIODevice::WriteOnly) )
    {}
    else
    { return -4;}

    frameHouse bufHouse;
    m_pCache->readFrame( m_pDev->mDeviceId, bufHouse );

    int payloadLen;
    for ( int i = 0; i < bufHouse.size(); i++ )
    {
        payloadLen = bufHouse.at(i).length();
        if ( payloadLen != file.write( bufHouse.at(i).data(), payloadLen ) )
        {
            file.close();
            return -5;
        }
    }

    file.close();

    return 0;
}




