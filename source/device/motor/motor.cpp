#include "motor.h"
#include "../mrq/deviceMRQ.h"
namespace MegaDevice
{

deviceProxyMotor::deviceProxyMotor( deviceMRQ *pMrq, const tpvRegion &region )
{
    m_pMRQ = pMrq;
    mRegion = region;
}
deviceProxyMotor::~deviceProxyMotor()
{}
//void deviceMotor::attachCondition( RoboCondition *pCond )
//{
//    Q_ASSERT( NULL != m_pMRQ );

//    m_pMRQ->attachCondition( mRegion.axes(), pCond );
//}
bool deviceProxyMotor::waitCondition(
                                 RoboCondition *pCond,
                                 int tmous )
{
    Q_ASSERT( NULL != m_pMRQ );

    return m_pMRQ->waitCondition( mRegion, pCond, tmous );
}

int deviceProxyMotor::pvtWrite(
              QList<tpvRow *> &list,
              int from,
              int len )
{
    Q_ASSERT( NULL != m_pMRQ );

    return m_pMRQ->pvtWrite( mRegion, list, from, len );
}

int deviceProxyMotor::run()
{
    Q_ASSERT( NULL != m_pMRQ );

    return m_pMRQ->run( mRegion );
}

int deviceProxyMotor::stop()
{
    Q_ASSERT( NULL != m_pMRQ );

    return m_pMRQ->stop( mRegion );
}

int deviceProxyMotor::status()
{
    Q_ASSERT( NULL != m_pMRQ );

    return m_pMRQ->status( mRegion );
}

void deviceProxyMotor::set( deviceMRQ *pMrq,
                       const tpvRegion &region )
{
    Q_ASSERT( NULL != pMrq );

    m_pMRQ = pMrq;

    mRegion = region;
}

deviceMRQ *deviceProxyMotor::Mrq()
{ return m_pMRQ; }
tpvRegion & deviceProxyMotor::region()
{ return mRegion; }

}

