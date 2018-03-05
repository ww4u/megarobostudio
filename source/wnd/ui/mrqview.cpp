#include "mrqview.h"
#include "../../robot/robotfact.h"
#include "../../../sys/sysapi.h"

mrqView::mrqView( QWidget *parent ) : modelView( parent )
{
    mAxesId = 0;
    m_pMrqModel = NULL;
    m_pModelObj = NULL;
}

void mrqView::setAxesId( int id )
{
    mAxesId = id;
}
int mrqView::getAxesId()
{
    return mAxesId;
}

void mrqView::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

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

    //! current device
    MegaDevice::deviceMRQ *pDevice = m_pmcModel->m_pInstMgr->findDevice(
                                                                        m_pmcModel->mConn.mDeviceName
                                                                        );
    if ( NULL == pDevice )
    {
        sysLog( "Invalid Device", m_pmcModel->mConn.mDeviceName );
    }

    return pDevice;
}

void mrqView::modelChanged()
{}
