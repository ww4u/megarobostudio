#include "mrh_e.h"

namespace MegaDevice {

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

Mrh_e::Mrh_e()
{
    setType( mcModelObj::model_hub );
    m_pBuf = NULL;
    mBufSize = 0;

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
}

Mrh_e::~Mrh_e()
{
    if ( NULL != m_pBuf )
    {
        delete []m_pBuf;
        m_pBuf = NULL;
    }
}

QString Mrh_e::deviceFullDesc( )
{
     return QString( "MRH-E(%1)").arg( getName() ) ;
}

int Mrh_e::write( byte *pData, int len )
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

    //! copy the data to add \n
    memcpy( m_pBuf, pData, len );
    m_pBuf[len] = 0x0a;

    int ret = m_pBus->write( m_pBuf, len+1 );

    return ret;
}
int Mrh_e::read( byte *pData, int len, int tmo, int &retLen )
{
    Q_ASSERT( NULL != pData );
    Q_ASSERT( NULL != m_pBus );

    int ret = m_pBus->read( pData, len, tmo, retLen );

    return ret;
}

byte* Mrh_e::recv( int &retLen, int len, int tmo )
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

    return m_pBuf;
}

}
