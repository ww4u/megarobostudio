#include "mrvview.h"

mrvView::mrvView( QWidget *parent ) : mrxView( parent )
{
    m_pMRV = NULL;
    m_pProxy = NULL;
}

void mrvView::setModelObj( mcModelObj *pObj )
{
    mrxView::setModelObj( pObj );

    //! get mrq from device
    setModel( (MegaDevice::deviceMRV*)( pObj ) );

    //! model changed
    modelChanged();
}

void mrvView::setModel( MegaDevice::deviceMRV *pModel )
{
    Q_ASSERT( NULL != pModel );

    m_pMRV = pModel;
    m_pProxy = m_pMRV->getModel();

    Q_ASSERT( NULL != m_pMRV );
    Q_ASSERT( NULL != m_pProxy );
}

MegaDevice::deviceMRV *mrvView::getModel()
{
    return m_pMRV;
}
