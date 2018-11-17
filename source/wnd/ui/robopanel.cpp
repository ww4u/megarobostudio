#include "robopanel.h"

RoboPanel::RoboPanel( mcModel *pModel,
                      VRobot *pRobo,
                      const QString &roboName,
                      QWidget *parent ) : RoboView( pRobo, RoboView::view_robo, pModel, parent )
{

    mPanelName = roboName;

    setWindowTitle( roboName );
    setAttribute( Qt::WA_DeleteOnClose );
    setModal( false );
}

void RoboPanel::slot_device_changed()
{ close(); }
