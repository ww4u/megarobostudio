#ifndef ROBOGEOGOOG_H
#define ROBOGEOGOOG_H


//#include "../../device/vrobot.h"
#include "../../device/mrq/deviceMRQ.h"

class roboGeogoog : public MegaDevice::deviceMRQ
{
public:
    roboGeogoog();

public:
    virtual QString trigSrcAlias( int ax, int iTrig );

protected:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );
};

#endif // ROBOGEOGOOG_H
