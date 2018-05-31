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
    double *m_pPXYZ;
    double *m_pT;

    double *m_pV;

    double *m_pOutput;
    int mSize;
};

class DeltaPhaseCache
{
public:
    DeltaPhaseCache();
    ~DeltaPhaseCache();

public:
    int alloc( int size );
    void clean();

public:
    double *m_pPhase;

    int mSize;
};


}

#endif // DELTADATACACHE_H
