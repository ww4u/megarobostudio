#ifndef ROBOQUEBELEY_H
#define ROBOQUEBELEY_H

//#include "../../device/vrobot.h"
#include "../../device/mrq/deviceMRQ.h"

class roboQuebeley : public MegaDevice::deviceMRQ
{
public:
    roboQuebeley( robotEnum id );

protected:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );
};

#endif // ROBOQUEBELEY_H
