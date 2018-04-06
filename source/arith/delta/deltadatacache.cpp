#include <QtCore>
#include "deltadatacache.h"
#include "../../../include/mcdef.h"
namespace arith_delta {

DeltaDataCache::DeltaDataCache()
{
    m_pPX = NULL;
    m_pPY = NULL;
    m_pPZ = NULL;

    m_pVX = NULL;
    m_pVY = NULL;
    m_pVZ = NULL;

    m_pAX = NULL;
    m_pAY = NULL;
    m_pAZ = NULL;

    mSize = 0;
}

DeltaDataCache::~DeltaDataCache()
{
    clean();
}

#define new_array( buf )      buf = new double[size]; \
                                    if ( NULL == buf ) \
                                    { return -1; } \
                                    memset( buf, 0, size * sizeof(double) );
#define new_array2( buf1, buf2 )    new_array( buf1 );\
                                    new_array( buf2 );
#define new_array3( buf1, buf2, buf3 )    new_array( buf1 );\
                                          new_array( buf2 ); \
                                          new_array( buf3 );
int DeltaDataCache::alloc( int size )
{
    Q_ASSERT( size > 0 );

    clean();

    //! again
    new_array3( m_pPX, m_pPY, m_pPZ );
    new_array3( m_pVX, m_pVY, m_pVZ );
    new_array3( m_pAX, m_pAY, m_pAZ );

    mSize = size;

    return 0;
}

void DeltaDataCache::clean()
{
    mSize = 0;

    gc_array3( m_pPX, m_pPY, m_pPZ );
    gc_array3( m_pVX, m_pVY, m_pVZ );
    gc_array3( m_pAX, m_pAY, m_pAZ );
}


}
