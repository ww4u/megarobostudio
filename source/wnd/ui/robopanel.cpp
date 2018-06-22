#include "robopanel.h"

RoboPanel::RoboPanel( mcModel *pModel,
                      const QString &roboName,
                      QWidget *parent ) : DlgView( pModel, parent )
{
    mPanelName = roboName;

    setWindowTitle( roboName );
    setAttribute( Qt::WA_DeleteOnClose );
    setModal( false );
}

void RoboPanel::slot_device_changed()
{ close(); }
