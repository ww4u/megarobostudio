#ifndef ROBOGOUF_H
#define ROBOGOUF_H

//#include "../../device/vrobot.h"
#include "../../device/mrv/devicemrv.h"

//class MegaDevice::deviceMRV;

class roboGouf : public MegaDevice::deviceMRV
{
public:
    roboGouf();
    ~roboGouf();
public:
//    virtual QString trigSrcAlias( int ax, int iTrig );

protected:
//    virtual int serialIn( QXmlStreamReader &reader );
//    virtual int serialOut( QXmlStreamWriter &writer );
};

#endif // ROBOQUEBELEY_H
