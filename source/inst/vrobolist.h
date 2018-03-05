#ifndef VROBOLIST_H
#define VROBOLIST_H

#include <QtCore>
#include "../../device/vrobot.h"
#include "../../bus/ibus.h"

class VRoboList : public QList<VRobot*>
{
public:
    VRoboList();
    ~VRoboList();

public:
    void attachBus( MegaDevice::IBus *pBus );
    MegaDevice::IBus *bus();

protected:
    MegaDevice::IBus *m_pBus;

};
#endif // VROBOLIST_H
