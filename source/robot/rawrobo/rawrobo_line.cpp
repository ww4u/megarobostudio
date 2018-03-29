#include "rawrobo.h"

void RawRobo::onLine()
{
    MegaDevice::deviceMRQ *pMrq;
    int ax;

    //! each ax
    for ( int i = 0; i < axes(); i++ )
    {
        pMrq = jointDevice( i, &ax );

        Q_ASSERT( NULL != pMrq );

        Q_ASSERT( regions() == pMrq->regions() );

        //! each region
        for ( int j = 0; j < regions(); j++ )
        {
            Q_ASSERT( pMrq->Fsm( tpvRegion(ax,j) ) != NULL );
            Q_ASSERT( NULL != mFsms[ tpvRegion(0,j) ] );
            pMrq->Fsm( tpvRegion(ax,j) )->setLeader( mFsms[ tpvRegion(0,j) ],
                                                    (void*)i );
        }

        //! use phy bus
        attachBus( pMrq->Bus() );
    }
}
void RawRobo::offLine()
{
    MegaDevice::deviceMRQ *pMrq;
    int ax;

    //! each ax
    for ( int i = 0; i < axes(); i++ )
    {
        pMrq = jointDevice( i, &ax );

        //! each region
        for ( int j = 0; j < regions(); j++ )
        {
            Q_ASSERT( pMrq->Fsm( tpvRegion(ax,j) ) != NULL );
            pMrq->Fsm( tpvRegion(ax,j) )->setLeader( NULL, NULL );
        }
    }

    //! \todo
//    detachBus();
}
