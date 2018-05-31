#include "megatron.h"

////! only for 6 joints
int robotMegatron::downloadTrace( const tpvRegion &region )
{
    int ret;
    if ( mJointsGroup.size() != axes() )
    { return ERR_INVALID_DATA; }

    onLine();

    fsm( region )->setState( MegaDevice::mrq_state_idle );

    //! data order:
    //! fx ly fz bx ry bz t
    //!
    //! joint id: x1,z1,x2,z2,y1,y2
    //!           fx,fz,bx,bz,ly,ry
    //!            0 1  2  3  4  5
    QList<int> jointsTabList;
    jointsTabList<<0<<4<<1<<2<<5<<3;
    ret = download( mJointsGroup, jointsTabList, region );
    if ( ret != 0 )
    { return ret; }
logDbg()<<mJointsGroup.size();

    return 0;
}

