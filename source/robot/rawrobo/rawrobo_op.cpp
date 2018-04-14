#include "rawrobo.h"

#include "../../device/board/_MRQ_enum.h"

int RawRobo::run( const tpvRegion &region  )
{
    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_run), region );
    return 0;
}
int RawRobo::stop( const tpvRegion &region  )
{
    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_stop), region );
    return 0;
}

int RawRobo::setLoop( int n, const tpvRegion &region )
{
    MegaDevice::deviceMRQ *pMrq;
    int ax;
    for ( int i = 0; i < axes(); i++ )
    {
        pMrq = jointDevice( i, &ax );

        Q_ASSERT( NULL != pMrq );

        pMrq->setMOTIONPLAN_CYCLENUM( ax,
                                      (MRQ_MOTION_SWITCH_1)region.page(),
                                      n );
    }

    return 0;
}

void RawRobo::switchReset( const tpvRegion &region )
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH,
                   x_channel,
                   (byte)MRQ_MOTION_SWITCH_RESET,
                   (byte)region.page() );
}
void RawRobo::switchStop( const tpvRegion &region )
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH,
                   x_channel,
                   (byte)MRQ_MOTION_SWITCH_STOP,
                   (byte)region.page() );
}
void RawRobo::switchRun( const tpvRegion &region )
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH,
                   x_channel,
                   (byte)MRQ_MOTION_SWITCH_RUN,
                   (byte)region.page() );
    logDbg();
}
void RawRobo::switchPrepare( const tpvRegion &region )
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH,
                   x_channel,
                  (byte)MRQ_MOTION_SWITCH_PREPARE,
                  (byte)region.page() );

    //! for each call
    MegaDevice::deviceMRQ *pMrq;
    int ax;
    tpvRegion mrqRegion;
    for ( int i = 0; i < axes(); i++ )
    {
        pMrq = jointDevice( i, &ax );
        Q_ASSERT( NULL != pMrq );

//        pMrq->setMOTION_SWITCH(
//                                ax,
//                                MRQ_MOTION_SWITCH_PREPARE,
//                                (MRQ_MOTION_SWITCH_1)region.page()
//                            );

        //! mrq
        mrqRegion.setAx( ax );
        mrqRegion.setPage( region.page() );
        pMrq->lpc( ax )->postMsg((eRoboMsg)(MegaDevice::mrq_msg_prepare),
                                  mrqRegion );

    }
}
void RawRobo::switchEmergStop( const tpvRegion &region )
{
    Q_ASSERT( NULL != m_pBus );
    MegaDevice::DeviceId id( mCanGroupId );
    m_pBus->write( id, mc_MOTION, sc_MOTION_SWITCH,
                   x_channel,
                   (byte)MRQ_MOTION_SWITCH_EMERGSTOP,
                   (byte)region.page() );
}


