#include "mcmodel.h"

mcModel::mcModel()
{
    init();
}

mcModel::~mcModel()
{
    deinit();

}

//! load data from file
void mcModel::preload()
{
    mDeviceDbs.load( DEVICE_TALBE_NAME );
}

mcConnection& mcModel::getConnection()
{
    return mConn;
}

void mcModel::init()
{
    m_pInstMgr = MegaDevice::InstMgr::proxy();

    Q_ASSERT( NULL != m_pInstMgr );

    m_pInstMgr->start();        //! start the server
}
void mcModel::deinit()
{
    MegaDevice::InstMgr::free();
}
