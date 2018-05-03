#include <QtCore>
#include "deltadatacache.h"
#include "../../../include/mcdef.h"
namespace arith_delta {

DeltaDataCache::DeltaDataCache()
{
//    m_pPX = NULL;
//    m_pPY = NULL;
//    m_pPZ = NULL;

//    m_pVX = NULL;
//    m_pVY = NULL;
//    m_pVZ = NULL;

//    m_pAX = NULL;
//    m_pAY = NULL;
//    m_pAZ = NULL;

    m_pPXYZ = NULL;
    m_pT = NULL;
    m_pV = NULL;

    m_pOutput = NULL;

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
    m_pPXYZ = new double[ 3 * size ];
    if ( NULL == m_pPXYZ )
    { return -1; }

    new_array2( m_pT, m_pV );

    //! output
    m_pOutput = new double[ 5 * size ];
    if ( NULL == m_pOutput )
    { return -1; }

    mSize = size;

    return 0;
}

void DeltaDataCache::clean()
{
    mSize = 0;

    gc_array3( m_pPXYZ, m_pT, m_pV );
    gc_array( m_pOutput );
}


}
