#ifndef MRQVIEW_H
#define MRQVIEW_H

#include <QWidget>

#include "../../device/mrq/deviceMRQ.h"

#include "modelview.h"

class mrqView : public modelView
{
    Q_OBJECT

public:
    mrqView( QWidget *parent = 0 );
    virtual ~mrqView()
    {}
public:
    virtual void setAxesId( int id );
    int getAxesId();

    virtual void setPage( MRQ_AX_PAGE pg );
    MRQ_AX_PAGE page();

    virtual void setModelObj( mcModelObj *pObj );

protected:
    void setModel( MegaDevice::deviceMRQ *pModel );
    MegaDevice::deviceMRQ *getModel();

    MegaDevice::deviceMRQ *getDevice();

public:


public:
    virtual void modelChanged();

protected:
    byte mAxesId;
    MRQ_AX_PAGE mPage;
    MegaDevice::deviceMRQ *m_pMrqModel;
    MegaDevice::MRQ_model *m_pProxy;

};



#endif // MRQVIEW_H
