
#include "motionedit.h"
#include "ui_motionedit.h"

//#include "../../arith/pathplan/pathplan.h"
//#include "../../arith/kinematic/kinematic.h"

int motionEdit::postDownload( appMsg msg, void *pPara )
{
//    testDownload();
//    return 0;
logDbg();
    on_spinLoop_valueChanged( ui->spinLoop->value() );

    int ret;
//logDbg();
//    ret = buildTpvGroups();
//    if ( ret != 0 )
//    { logDbg();return ret; }
logDbg();
    //! check groups
    if ( mJointsTpvGroup.size() > 0 )
    {}
    else
    { logDbg();return ERR_NO_TPV_DATA; }
logDbg();
    QList<int> jointTabId;
    jointTabId<<0<<1<<2<<3<<4;
    ret = doDownload( mJointsTpvGroup, jointTabId );
logDbg()<<ret;

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
    pRobo->download( groups,
                     jointTabId,
                     tpvRegion(0,0) );  //! \todo page

    return 0;
}


