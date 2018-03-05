#include "motor.h"
#include "../mrq/deviceMRQ.h"
namespace MegaDevice
{

deviceMotor::deviceMotor( deviceMRQ *pMrq, int ax )
{
    m_pMRQ = pMrq;
    mAxes = ax;
}

void deviceMotor::attachCondition( RoboCondition *pCond )
{
    Q_ASSERT( NULL != m_pMRQ );

    m_pMRQ->attachCondition( mAxes, pCond );
}
bool deviceMotor::waitCondition( RoboCondition *pCond,
                            int tmous )
{
    Q_ASSERT( NULL != m_pMRQ );

    return m_pMRQ->waitCondition( mAxes, pCond, tmous );
}

int deviceMotor::deviceMotor::pvtWrite(
              QList<tpvRow *> &list,
              int from,
              int len )
{
    Q_ASSERT( NULL != m_pMRQ );

    return m_pMRQ->pvtWrite( mAxes, list, from, len );
}

int deviceMotor::run()
{
    Q_ASSERT( NULL != m_pMRQ );

    return m_pMRQ->run( mAxes );
}

int deviceMotor::stop()
{
    Q_ASSERT( NULL != m_pMRQ );

    return m_pMRQ->stop( mAxes );
}

int deviceMotor::status()
{
    Q_ASSERT( NULL != m_pMRQ );

    return m_pMRQ->status( mAxes );
}

void deviceMotor::set( deviceMRQ *pMrq, int ax )
{
    Q_ASSERT( NULL != pMrq );

    m_pMRQ = pMrq;
    mAxes = ax;
}

deviceMRQ *deviceMotor::Mrq()
{
    return m_pMRQ;
}
int deviceMotor::Axes()
{ return mAxes; }

}

