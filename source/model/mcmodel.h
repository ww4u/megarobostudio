#ifndef MCMODEL_H
#define MCMODEL_H

#include "../inst/instmgr.h"
#include "../com/drivercontroller.h"
#include "mcconnection.h"
#include "modelsyspref.h"


class mcModel
{
public:
    mcModel();
    ~mcModel();

    void preload();
    void postload();

    mcConnection& getConnection();

protected:
    void init();
    void deinit();
public:
    void startCommunicate();
    void stopCommunicate();
    void resetCommunicate();

public:
    MegaDevice::InstMgr *m_pInstMgr;
    deviceDb mDeviceDbs;

    mcConnection mConn;

    modelSysPref mSysPref;
};

#endif // MCMODEL_H

