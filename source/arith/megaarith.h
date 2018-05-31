#ifndef MEGAARITH_H
#define MEGAARITH_H

#define local_ABS( a )  qAbs( a )

#define MEGA_EXPORT
#define MEGA_EXTERN

#include <QtCore>

template<typename T>
class smartBuffer{
public:
    T *m_pBuffer;
    int mSize;

public:
    smartBuffer( int size )
    {
        m_pBuffer = NULL;
        mSize = 0;

        if ( size > 0 )
        {
            allocate( size );
        }
    }

    ~smartBuffer()
    {
        if ( mSize != 0 )
        {
            delete []m_pBuffer;
            m_pBuffer = NULL;
            mSize = 0;
        }
    }

    void allocate( int n )
    {
        m_pBuffer = new T[ n ];
        mSize = n;
        Q_ASSERT( NULL != m_pBuffer );

        memset( m_pBuffer, 0, sizeof(T)*mSize );
    }

    T& operator[]( int i )
    {
        Q_ASSERT( i >=0 && i < mSize );
        return m_pBuffer[i];
    }
};

#endif // MEGAARITH_H
