#include "motionedit.h"

//! start
int motionEdit::postStart( appMsg msg, void *pPara )
{
//    QString str;
//    int axesId;
//    str = m_pmcModel->getConnection().getDeviceName();
//    axesId = m_pmcModel->getConnection().getDeviceCH();

//    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( str,
//                                                                      axesId );
//    Q_ASSERT( NULL != pMrq );

////    MRQ_MOTION_STATE_2 stat;

////    int ret = pMrq->getMOTION_STATE( axesId, MRQ_MOTION_SWITCH_1_MAIN, &stat );
////    if ( ret != 0 )
////    { return ret; }
////    if ( stat != MRQ_MOTION_STATE_2_CALCEND )
////    { return ERR_CAN_NOT_RUN; }
//    int ret;
//    ret = pMrq->run( tpvRegion(axesId,0) );

//    return ret;
    return 0;
}
void motionEdit::beginStart( void *pPara )
{}
void motionEdit::endStart( int ret, void *pPara )
{
    if ( ret != 0 )
    { sysError( tr("start fail") ); }
    else
    { }
}
