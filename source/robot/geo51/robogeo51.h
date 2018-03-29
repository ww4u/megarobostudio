#ifndef ROBOGEO51_H
#define ROBOGEO51_H

//#include "../../device/vrobot.h"
#include "../../device/mrq/deviceMRQ.h"

class roboGeo51 : public MegaDevice::deviceMRQ
{
public:
    roboGeo51();

protected:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );
};

#endif // ROBOQUEBELEY_H
