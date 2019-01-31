#ifndef ROBOGEO2_H
#define ROBOGEO2_H


#include "../../device/mrq/deviceMRQ.h"

class roboGeo2 : public MegaDevice::deviceMRQ
{
public:
    roboGeo2();

public:
    virtual QString trigSrcAlias( int ax, int iTrig );

protected:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );
};

#endif // ROBOQUEBELEY_H
