#ifndef ROBOPANEL_H
#define ROBOPANEL_H

#include "dlgview.h"

class RoboPanel : public DlgView
{
    Q_OBJECT
public:
    RoboPanel( mcModel *pModel,
               const QString &roboName,
               QWidget *parent = 0 );

protected Q_SLOTS:
    void slot_device_changed();

protected:
    QString mPanelName;
};

#endif // ROBOPANEL_H
