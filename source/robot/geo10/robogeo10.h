#ifndef ROBOGEO10_H
#define ROBOGEO10_H

//#include "../../device/vrobot.h"
#include "../../device/mrq/deviceMRQ.h"

class roboGeo10 : public MegaDevice::deviceMRQ
{
public:
    roboGeo10();

public:
    virtual QString trigSrcAlias( int ax, int iTrig );

protected:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );
};

#endif // ROBOQUEBELEY_H
