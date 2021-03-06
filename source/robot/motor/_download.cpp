#include "motor.h"

////! only for 1 joints
int robotMotor::downloadTrace( const tpvRegion &region )
{
    int ret;
    if ( mJointsGroup.size() != axes() )
    { return ERR_INVALID_DATA; }

    onLine();

    QList<int> jointsTabList;
    jointsTabList<<0;
    ret = RawRobo::download( mJointsGroup, jointsTabList, region );
    if ( ret != 0 )
    { return ret; }

    return 0;
}
