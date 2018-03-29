#ifndef ROBOGEO8_H
#define ROBOGEO8_H

//#include "../../device/vrobot.h"
#include "../../device/mrq/deviceMRQ.h"

class roboGeo8 : public MegaDevice::deviceMRQ
{
public:
    roboGeo8();

protected:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );
};

#endif // ROBOQUEBELEY_H
