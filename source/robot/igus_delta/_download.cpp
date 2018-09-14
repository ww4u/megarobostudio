#include "igus_delta.h"

////! only for 4 joints
int robotIgusDelta::downloadTrace( const tpvRegion &region )
{
    int ret;
    if ( mJointsGroup.size() != axes() )
    { return ERR_INVALID_DATA; }

    onLine();

    fsm( region )->setState( MegaDevice::mrq_state_program );

    QList<int> jointsTabList;
    //! left/right/plate/end
    jointsTabList<<0<<1<<2<<3;
    ret = download( mJointsGroup, jointsTabList, region );
    if ( ret != 0 )
    { return ret; }
logDbg()<<mJointsGroup.size();

    return 0;
}
