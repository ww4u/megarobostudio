#ifndef ROBOPANEL_H
#define ROBOPANEL_H

#include "roboview.h"

class RoboPanel : public RoboView
{
    Q_OBJECT
public:
    RoboPanel( mcModel *pModel,
               VRobot *pRobo,
               const QString &roboName,
               QWidget *parent = 0 );

protected Q_SLOTS:
    void slot_device_changed();

protected:
    QString mPanelName;

};

#endif // ROBOPANEL_H
