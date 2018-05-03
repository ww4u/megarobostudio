#ifndef DELTADATACACHE_H
#define DELTADATACACHE_H

namespace arith_delta {

class DeltaDataCache
{
public:
    DeltaDataCache();
    ~DeltaDataCache();

public:
    int alloc( int size );
    void clean();

public:
//    double *m_pPX, *m_pPY, *m_pPZ;
//    double *m_pVX, *m_pVY, *m_pVZ;
//    double *m_pAX, *m_pAY, *m_pAZ;

    double *m_pPXYZ;
    double *m_pT;

    double *m_pV;

    double *m_pOutput;
    int mSize;
};

}

#endif // DELTADATACACHE_H
