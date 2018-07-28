#ifndef MRH_E_H
#define MRH_E_H

#include "../phydevice.h"

namespace MegaDevice {

class Mrh_e : public PhyDevice
{
public:
    Mrh_e();
    ~Mrh_e();

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
