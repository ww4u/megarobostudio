#include "rs232bus.h"
#include "receivecache.h"
#include "../device/board/_mrq_enum.h"
namespace MegaDevice {

//Rs232Bus::Rs232Bus() : IBus()
Rs232Bus::Rs232Bus() : CANBus()
{
    mViDef = 0;
    mViDevice = 0;

    mName = "RS232-BUS";
    mBusType = e_bus_232;

    mLinkType = MRQ_LINK_INTFC_RS232;
}

Rs232Bus::~Rs232Bus()
{
    close();
}

int Rs232Bus::open(
                  const modelSysPref &pref,
                  int devType,
                  int devId, int seqId, int canId,
                  const QString &desc )
{
    //! open
    int ret = open_( pref, desc );
    if ( ret != 0 )
    { return ret; }

    //! attach
    Q_ASSERT( NULL == m_pRecvCache );
    m_pRecvCache = new receiveCache();
    if ( NULL == m_pRecvCache )
    {
        close();
        return -3;
    }
    m_pRecvCache->attachBus( this );
    m_pRecvCache->start();

    return 0;
}

int Rs232Bus::open_( const modelSysPref &pref, QString str )
{
    ViStatus viStat = viOpenDefaultRM( &mViDef );
    if ( viStat != VI_SUCCESS )
    { return -1; }

    viStat = viOpen( mViDef, str.toLatin1().data(), 0, 2000, &mViDevice );
    if ( viStat != VI_SUCCESS )
    { return -1; }

    QList<int> baudList;
    baudList<<4800<<7200<<9600<<14400
            <<19200<<38400<<57600<<115200
            <<128000;
    Q_ASSERT( pref.mBaudIndex >=0 && pref.mBaudIndex < baudList.size() );
    viStat = viSetAttribute( mViDevice, VI_ATTR_ASRL_BAUD, baudList.at( pref.mBaudIndex) );
    if ( viStat != VI_SUCCESS )
    { return -1; }

    QList<int> widList;
    widList<<8<<9;
    Q_ASSERT( pref.mDataWidthIndex >=0 && pref.mDataWidthIndex < widList.size() );
    viStat = viSetAttribute( mViDevice, VI_ATTR_ASRL_DATA_BITS, widList.at(pref.mDataWidthIndex) );
    if ( viStat != VI_SUCCESS )
    { return -1; }

    QList<int> parityList;
    parityList<<VI_ASRL_PAR_NONE<< VI_ASRL_PAR_EVEN<<VI_ASRL_PAR_ODD;
    Q_ASSERT( pref.mParityIndex >=0 && pref.mParityIndex < parityList.size() );
    viStat = viSetAttribute( mViDevice, VI_ATTR_ASRL_PARITY, parityList.at(pref.mParityIndex) );
    if ( viStat != VI_SUCCESS )
    { return -1; }

    viStat = viSetAttribute( mViDevice, VI_ATTR_ASRL_FLOW_CNTRL, VI_ASRL_FLOW_NONE );
    if ( viStat != VI_SUCCESS )
    { return -1; }

    viStat = viSetAttribute( mViDevice, VI_ATTR_ASRL_END_IN, VI_ASRL_END_NONE );
    if ( viStat != VI_SUCCESS )
    { return -1; }

    //! timeout = 1ms
    viStat = viSetAttribute( mViDevice, VI_ATTR_TMO_VALUE, mRecvTmo );
    if ( viStat != VI_SUCCESS )
    { return -1; }

    return 0;
}

void Rs232Bus::close()
{
    //! clean cache
    if ( NULL != m_pRecvCache )
    {
        m_pRecvCache->requestInterruption();
        m_pRecvCache->wait();
        delete m_pRecvCache;
        m_pRecvCache = NULL;

        //! bypass off
        receiveCache::setByPass( mDevId, 0 );
    }

    if ( mViDevice != 0 )
    {
        viClose( mViDevice);
        mViDevice = 0;
    }

    if ( mViDef != 0 )
    {
        viClose( mViDef );
        mViDef = 0;
    }
}

//! no para
int Rs232Bus::write( quint8 m, quint8 c )
{
    U8List varList;

    varList<<m<<c;

    return write( varList );
}

//! 1 para
int Rs232Bus::write( quint8 m, quint8 c, quint8 v0 )
{
    U8List varList;

    varList<<m<<c<<v0;

    return write( varList );
}

#define var_b( v, id)   ((quint8*)&v)[ id ]
int Rs232Bus::write( quint8 m, quint8 c, quint16 v0 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)<<var_b(v0,1);

    return write( varList );
}
int Rs232Bus::write( quint8 m, quint8 c, quint32 v0 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)<<var_b(v0,1)<<var_b(v0,2)<<var_b(v0,3);

    return write( varList );
}

int Rs232Bus::write( quint8 m, quint8 c, qint8 v0 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0);

    return write( varList );
}
int Rs232Bus::write( quint8 m, quint8 c, qint16 v0 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)<<var_b(v0,1);

    return write( varList );
}
int Rs232Bus::write( quint8 m, quint8 c, qint32 v0 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)<<var_b(v0,1)<<var_b(v0,2)<<var_b(v0,3);

    return write( varList );
}

//! 2 para
int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, quint8 v1 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)<<var_b(v1,0);

    return write( varList );
}
int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, quint16 v1 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)<<var_b(v1,0)<<var_b(v1,1);

    return write( varList );
}
int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, quint32 v1 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)<<var_b(v1,0)<<var_b(v1,1)<<var_b(v1,2)<<var_b(v1,3);

    return write( varList );
}

int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, qint8 v1 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)<<var_b(v1,0);

    return write( varList );
}
int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, qint16 v1 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)<<var_b(v1,0)<<var_b(v1,1);

    return write( varList );
}
int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, qint32 v1 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)<<var_b(v1,0)<<var_b(v1,1)<<var_b(v1,2)<<var_b(v1,3);

    return write( varList );
}

int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, quint8 v1, quint8 v2 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)
                 <<var_b(v1,0)
                 <<var_b(v2,0);

    return write( varList );
}

int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, quint8 v1, quint32 v2 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)
                 <<var_b(v1,0)
                 <<var_b(v2,0)<<var_b(v2,1)<<var_b(v2,2)<<var_b(v2,3);

    return write( varList );
}

int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, quint8 v1, float v2 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)
                 <<var_b(v1,0)
                 <<var_b(v2,0)<<var_b(v2,1)<<var_b(v2,2)<<var_b(v2,3);

    return write( varList );
}

int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, quint16 v1, quint16 v2 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)
                 <<var_b(v1,0)<<var_b(v1,1)
                 <<var_b(v2,0)<<var_b(v2,1);

    return write( varList );
}
int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, quint32 v1, quint32 v2 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)
                 <<var_b(v1,0)<<var_b(v1,1)<<var_b(v1,2)<<var_b(v1,3)
                 <<var_b(v2,0)<<var_b(v2,1)<<var_b(v2,2)<<var_b(v2,3);

    return write( varList );
}

int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, qint8 v1, qint8 v2 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)
                 <<var_b(v1,0)
                 <<var_b(v2,0);

    return write( varList );
}
int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, qint16 v1, qint16 v2 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)
                 <<var_b(v1,0)<<var_b(v1,1)
                 <<var_b(v2,0)<<var_b(v2,1);

    return write( varList );
}
int Rs232Bus::write( quint8 m, quint8 c, quint8 v0, qint32 v1, qint32 v2 )
{
    U8List varList;

    varList<<m<<c<<var_b(v0,0)
                 <<var_b(v1,0)<<var_b(v1,1)<<var_b(v1,2)<<var_b(v1,3)
                 <<var_b(v2,0)<<var_b(v2,1)<<var_b(v2,2)<<var_b(v2,3);

    return write( varList );
}

int Rs232Bus::write( U8List &rawList )
{
    //! 1. encode
    U8List outList;
    encode( rawList, outList );

    //! 2. send
    int ret;
    ret = send( outList );
    if ( ret != 0 )
    { return ret; }

    return ret;
}

int Rs232Bus::write( QByteArray &rawList )
{
    U8List datList;

    for( int i = 0; i <rawList.size(); i++ )
    {
        datList.append( rawList.at(i) );
    }

    return write( datList );
}

int Rs232Bus::read( U8List &rawList )
{
    //! 1. read header
    int ret;

    U8List headList;
    ret = recv( headList, 2 );
    if ( ret != 0 )
    { return ret; }

    //! for the data
    if ( headList[0] != 0xfe )
    { return -1; }

    //! check size
    quint8 packLen = headList[1];
    if ( packLen < 1 || packLen > 0xf9 )
    { return -2; }

    //! read payload
    U8List packList;
    ret = recv( packList, packLen - 2 );
    if ( ret != 0 )
    { return -2; }

    U8List padList;
    decode( packList, padList );
    rawList = padList;
    rawList.removeFirst();

    //! check the xor
    quint8 x;
    x = xored( rawList );
    x = x ^ headList[0];
    quint8 dLen;
    dLen = 2 + 1 + rawList.size();
    x = x ^ dLen;

    if ( x != padList[0] )
    { return -3; }

    return 0;
}

void Rs232Bus::encode( U8List &rawData,
                     U8List &outAry )
{
    //! frame header
    //! len
    //! xor

    quint8 header, packLen, packXor;

    header = 0xfe;

    //! xor
    quint8 x;

    packXor = xored( rawData );
    packLen = 2 + 1 + rawData.length();

    x = header ^ packLen;
    x = x ^ packXor;

    //! payload
    U8List dataSets;
    dataSets = rawData;
    dataSets.prepend( x );

    //! now for the data
    U8List payload;
    for ( int i = 0; i < dataSets.size(); i++ )
    {
        if ( dataSets[i] == 0xfe )
        {
            payload.append( 0xfd );
            payload.append( 0x5e );
        }
        else if ( dataSets[i] == 0xfd )
        {
            payload.append( 0xfd );
            payload.append( 0x5d );
        }
        else
        { payload.append( dataSets[i]); }
    }

    //! build the output
    outAry.append( header );
    packLen = 2 + payload.length();
    outAry.append( packLen );

    outAry.append( payload );
}

void Rs232Bus::decode( U8List &payload, U8List &rawData )
{
    //! for decode
    for ( int i = 0; i < payload.size(); i++ )
    {
        if ( payload[i] == 0xfd )
        {
            Q_ASSERT( ( i + 1 ) < payload.size() );

            if ( payload[i+1] == 0x5d )
            { i++; rawData.append( 0xfd); }
            else if ( payload[i+1] == 0x5e )
            { i++; rawData.append( 0xfe); }
            else
            { //! error
                Q_ASSERT( false );
            }
        }
        else
        { rawData.append( payload[i]); }
    }
}

int Rs232Bus::send( U8List &sendList )
{
    quint8 *pBuf = new quint8[ sendList.size() ];
    Q_ASSERT( NULL != pBuf );

    for ( int i = 0; i < sendList.size(); i++ )
    {
        pBuf[i] = sendList.at(i);
    }

    ViUInt32 retCount;

    IBus::lock();

    //! flush
    viFlush( mViDevice, VI_WRITE_BUF  );

    //! write again
    ViStatus viStat = viWrite( mViDevice, pBuf, (quint32)sendList.size(), &retCount );
    delete []pBuf;

    IBus::unlock();

    if ( viStat != VI_SUCCESS )
    { return -1; }
    if ( (int)retCount != sendList.size() )
    { return -1; }

    return 0;
}

int Rs232Bus::recv( U8List &readList, int cnt )
{
    quint8 *pBuf = new quint8[ cnt ];
    Q_ASSERT( NULL != pBuf );

    ViUInt32 retCount;
    IBus::lock();
    ViStatus viStat;
    viSetAttribute( mViDevice, VI_ATTR_TMO_VALUE, mRecvTmo * (cnt) );
    viStat = viRead( mViDevice, pBuf, cnt, &retCount );

    //! try again
    if ( viStat == VI_ERROR_TMO )
    {
        if ( retCount > 0 ) //! read success
        {
            Q_ASSERT( retCount < cnt );
            viStat = viRead( mViDevice, pBuf + retCount, cnt - retCount, &retCount );
        }
    }
    else
    {}

    IBus::unlock();
    if ( viStat != VI_SUCCESS_MAX_CNT )
    {
        delete []pBuf;
        return -1;
    }

    //! attach data
    for( int i = 0; i < (int)cnt; i++ )
    {
        readList.append( pBuf[i] );
    }

    delete []pBuf;

    return 0;

}

quint8 Rs232Bus::xored( U8List &data )
{
    quint8 x;

    x = data[0];
    for ( int i = 1; i < data.size(); i++ )
    {
        x = x ^ data[i];
    }

    return x;
}

}
