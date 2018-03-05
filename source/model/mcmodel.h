#ifndef MCMODEL_H
#define MCMODEL_H

#include "../inst/instmgr.h"
#include "../com/drivercontroller.h"
#include "mcconnection.h"
#include "modelsyspref.h"

#include "eventactionmodel.h"

class mcModel
{
public:
    mcModel();
    ~mcModel();

    void preload();

    mcConnection& getConnection();

protected:
    void init();
    void deinit();

public:
    MegaDevice::InstMgr *m_pInstMgr;
    deviceDb mDeviceDbs;

    mcConnection mConn;

    modelSysPref mSysPref;

    EventActionModel mEventActionModel;
};

#endif // MCMODEL_H
