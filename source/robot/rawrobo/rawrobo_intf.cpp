#include "rawrobo.h"

int RawRobo::call( const tpvRegion &region )
{
    onLine();

    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_call), region );

    return 0;
}


int RawRobo::download( QList<tpvGroup*> &groups,
                      QList<int> &joints,       //! joint tab id
                      const tpvRegion &region )
{logDbg();
    Q_ASSERT( groups.size() == joints.size() );

    MegaDevice::deviceMRQ *pMrq;
    int axes;
logDbg();
    int ret;
    tpvRegion mrqRegion;
    for ( int i = 0; i < groups.size(); i++ )
    {
        pMrq = jointDevice( joints[i], &axes );

        if ( NULL == pMrq )
        { logDbg()<<mAxesConnectionName[i];return ERR_INVALID_DEVICE_NAME; }

        QList<tpvRow*> rows;
        groups[i]->getRows( rows );
logDbg();
        //! \note change to mrq ax
        mrqRegion = region;
        mrqRegion.setAx( axes );

        //! send
        ret = pMrq->pvtWrite( mrqRegion, rows );
        if ( ret != 0 )
        { return ret; }
    }
logDbg();
    return 0;
}

