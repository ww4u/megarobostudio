#ifndef MRH_T_H
#define MRH_T_H

#include "../phydevice.h"

namespace MegaDevice {

class Mrh_t : public PhyDevice
{
public:
    Mrh_t();
    ~Mrh_t();

public:
    virtual const void* loadScpiCmd();

public:
    //! desc + alias
    virtual QString deviceFullDesc();


public:
    int write( byte *pData,
               int len,
               bool bTrim = false,
               byte v = 0 );

    int read( byte *pData, int len, int tmo, int &retLen );
    byte* recv( int &retLen, int len = 512, int tmo = 5 );

protected:
    byte *m_pBuf;
    int mBufSize;
};

}

#endif // MRH_E_H
