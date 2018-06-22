#ifndef MRVVIEW_H
#define MRVVIEW_H

#include "mrxview.h"

#include "../../device/mrv/devicemrv.h"

class mrvView : public mrxView
{
    Q_OBJECT

public:
    mrvView( QWidget *parent = 0 );

public:
    virtual void setModelObj( mcModelObj *pObj );

protected:
    void setModel( MegaDevice::deviceMRV *pMrv );
    MegaDevice::deviceMRV *getModel();


protected:
    MegaDevice::deviceMRV *m_pMRV;
    MegaDevice::MRV_model *m_pProxy;

};

#endif // MRVVIEW_H
