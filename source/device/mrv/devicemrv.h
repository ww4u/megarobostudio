#ifndef DEVICEMRV_H
#define DEVICEMRV_H

#include "../../../include/mcstd.h"
#include "../vdevice.h"
#include "../mrv_board/MRV.h"
#include "../motor/motor.h"

#include "../../com/basetype.h"

namespace MegaDevice
{
class deviceMRV : public MRV
{
public:
    deviceMRV();

public:
    virtual void rst();
    virtual int uploadDesc();
    virtual int uploadBaseInfo();
    virtual int uploadIDs();
    virtual int upload();

    virtual QList<int> deviceIds();
    virtual QString deviceFullDesc();
public:
    //! prop
    QString loadDesc();
    QString loadSN();
    QString loadSwVer();
    QString loadHwVer();
    QString loadBtVer();

public:
    MRV_model *getModel();
};

}

#endif // DEVICEMRV_H
