
#include "motionedit.h"
#include "ui_motionedit.h"

//#include "../../arith/pathplan/pathplan.h"
//#include "../../arith/kinematic/kinematic.h"

int motionEdit::postDownload( appMsg msg, void *pPara )
{
    //! loop changed
    on_spinLoop_valueChanged( ui->spinLoop->value() );

    int ret;

    //! check groups
    if ( mJointsTpvGroup.size() > 0 )
    {}
    else
    { logDbg();return ERR_NO_TPV_DATA; }

    QList<int> jointTabId;
    for( int i = 0; i < mJointsTpvGroup.size(); i++ )
    { jointTabId<<i; }
    ret = doDownload( mJointsTpvGroup, jointTabId );

    return ret;
}
void motionEdit::beginDownload( void *pPara)
{ logDbg(); }
void motionEdit::endDownload( int ret, void *pPara )
{ logDbg()<<ret; }

int motionEdit::doDownload( QList<tpvGroup *> &groups,
                            QList<int> &jointTabId )
{
    //! check robo
    VRobot *pRobo;
    pRobo = currentRobot();
    if ( NULL == pRobo )
    { return ERR_INVALID_ROBOT_NAME; }

    //! ui info
    setAgent( pRobo->name() );
    setLink( true );

    setChildAgents( pRobo->axesDeviceName() );

    pRobo->onLine();
    pRobo->setLoop( ui->spinLoop->value(),
                    tpvRegion(0,ui->widget->roboPage()) );
    pRobo->download( groups,
                     jointTabId,
                     tpvRegion(0, ui->widget->roboPage() ) );
    return 0;
}


