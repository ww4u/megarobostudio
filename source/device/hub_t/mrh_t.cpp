#include "mrh_t.h"

namespace MegaDevice {

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

Mrh_t::Mrh_t()
{
    setType( mcModelObj::model_hub );
    m_pBuf = NULL;
    mBufSize = 0;

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

Mrh_t::~Mrh_t()
{
    if ( NULL != m_pBuf )
    {
        delete []m_pBuf;
        m_pBuf = NULL;
    }
}

QString Mrh_t::deviceFullDesc( )
{
     return QString( "MRH-T(%1)").arg( getName() ) ;
}

int Mrh_t::write( byte *pData,
                  int len,
                  bool bTrim,
                  byte v )
{
    Q_ASSERT( NULL != pData );
    Q_ASSERT( NULL != m_pBus );

    if ( (len+1) > mBufSize )
    {
        if ( m_pBuf == NULL )
        {}
        else
        { delete []m_pBuf; }

        m_pBuf = new byte[ len+1 ];
        if ( NULL == m_pBuf )
        { return -1; }

        mBufSize = len + 1;
    }

    //! copy
    if ( bTrim )
    {
        int trimLen = 0;
        for ( int i = 0; i < len; i++ )
        {
            if ( v != pData[i] )
            { m_pBuf[ trimLen++] = pData[i]; }
        }
        len = trimLen;
    }
    else
    {
        //! copy the data to add \n
        memcpy( m_pBuf, pData, len );
    }

    //! trim the end
    {
        int pos = len - 1;
        do
        {
            if ( m_pBuf[pos] == '\n'  )
            {}
            else if ( m_pBuf[pos] == '\r' )
            {}
            else
            { break; }
            pos--;

        }while( pos >= 0 );

        Q_ASSERT( pos > 0 );
        m_pBuf[ pos + 1 ] = '\n';
        len = pos + 2;
    }

    int ret = m_pBus->write( m_pBuf, len );

    return ret;
}
int Mrh_t::read( byte *pData, int len, int tmo, int &retLen )
{
    Q_ASSERT( NULL != pData );
    Q_ASSERT( NULL != m_pBus );

    int ret = m_pBus->read( pData, len, tmo, retLen );

    return ret;
}

byte* Mrh_t::recv( int &retLen, int len, int tmo )
{
    if ( (len+1) > mBufSize )
    {
        if ( m_pBuf == NULL )
        {}
        else
        { delete []m_pBuf; }

        m_pBuf = new byte[ len+1 ];
        if ( NULL == m_pBuf )
        {
            retLen = 0;
            return NULL;
        }

        mBufSize = len + 1;
    }

    read( m_pBuf, len, tmo, retLen );
//    if ( retLen <= 0 )
//    {}
//    else
//    {
//        m_pBuf[ retLen ] = 0;
//    }

    return m_pBuf;
}

}
