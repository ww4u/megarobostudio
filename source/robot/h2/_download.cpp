#include "h2.h"

////! only for 3 joints
int robotH2::downloadTrace( const tpvRegion &region )
{
    int ret;
    if ( mJointsGroup.size() != axes() )
    { return ERR_INVALID_DATA; }

    onLine();

    QList<int> jointsTabList;
    jointsTabList<<0<<1<<2;
    ret = RawRobo::download( mJointsGroup, jointsTabList, region );
    if ( ret != 0 )
    { return ret; }
logDbg()<<mJointsGroup.size();

    return 0;
}
