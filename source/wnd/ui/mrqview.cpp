#include "mrqview.h"
#include "../../robot/robotfact.h"
#include "../../../sys/sysapi.h"

mrqView::mrqView( QWidget *parent ) : mrxView( parent )
{
    mPage = MRQ_MOTION_SWITCH_1_MAIN;
    m_pMrqModel = NULL;
    m_pModelObj = NULL;
}

void mrqView::setPage( MRQ_AX_PAGE pg )
{ mPage  = pg; }
MRQ_AX_PAGE mrqView::page()
{ return mPage; }

void mrqView::setModelObj( mcModelObj *pObj )
{
    mrxView::setModelObj( pObj );

    //! get mrq from device
    setModel( (MegaDevice::deviceMRQ*)( pObj ) );

    //! model changed
    modelChanged();
}

void mrqView::setModel( MegaDevice::deviceMRQ *pModel )
{
    Q_ASSERT( NULL != pModel );

    m_pMrqModel = pModel;
    m_pProxy = m_pMrqModel->getModel();

    Q_ASSERT( NULL != m_pMrqModel );
    Q_ASSERT( NULL != m_pProxy );

}
MegaDevice::deviceMRQ *mrqView::getModel()
{
    return m_pMrqModel;
}

MegaDevice::deviceMRQ *mrqView::getDevice()
{
    Q_ASSERT( NULL != m_pmcModel );

    return m_pMrqModel;
//    //! current device
//    MegaDevice::deviceMRQ *pDevice = m_pmcModel->m_pInstMgr->findDevice(
//                                                                        m_pmcModel->mConn.mDeviceName
//                                                                        );
//    if ( NULL == pDevice )
//    {
//        sysLog( tr("Invalid Device"), m_pmcModel->mConn.mDeviceName );
//        logDbg()<<m_pmcModel->m_pInstMgr->roboResources();
//        logDbg()<<m_pmcModel->m_pInstMgr->getResources();
//    }

//    return pDevice;
}


