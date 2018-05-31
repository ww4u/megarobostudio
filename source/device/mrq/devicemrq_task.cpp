#include "deviceMRQ.h"

namespace MegaDevice
{

ArgLightCoupZero::ArgLightCoupZero()
{
    mAx = 0;
    mPage = 0;

    mT = 0;
    mAngle = 0;

    mInvT = 0;
    mInvAngle = 0;
}

MrqTaskThread::MrqTaskThread( QObject *parent ) : RoboTask( parent )
{

}

void MrqTaskThread::run()
{
    checkRequest( m_pReq );

    sysLog( __FUNCTION__, QString::number(__LINE__), "Enter" );

    deviceMRQ *pMrq;
    pMrq = (deviceMRQ *)m_pReq->m_pRobo;

//    Q_ASSERT( NULL != m_pReq->m_pApi );

//    deviceMRQ::apiMrqRequest pApi;
//    pApi = (deviceMRQ::apiMrqRequest)m_pReq->m_pApi;

//    ArgLightCoupZero *pArg = (ArgLightCoupZero*)(m_pReq->m_pArg);

    int ret = ( pMrq->*(m_pReq->m_pApi))( m_pReq->m_pArg );

    gc();

    sysLog( __FUNCTION__, QString::number(__LINE__), QString::number( ret ) );
}

}
