#include "vrobolist.h"

VRoboList::VRoboList()
{
    m_pBus = NULL;
}

VRoboList::~VRoboList()
{
//    qDeleteAll( *this );
}

void VRoboList::attachBus( MegaDevice::IBus *pBus )
{
    Q_ASSERT( NULL != pBus );

    m_pBus = pBus;
}
MegaDevice::IBus *VRoboList::bus()
{ return m_pBus; }
