#include "h2z.h"

////! only for 3 joints
int robotH2Z::downloadTrace( const tpvRegion &region,
                            QList< tpvGroup *> &jointsGroup )
{
    int ret;
    if ( jointsGroup.size() != axes() )
    { return ERR_INVALID_DATA; }

    onLine();

    fsm( region )->setState( MegaDevice::mrq_state_program );

    QList<int> jointsTabList;
    jointsTabList<<0<<1<<2;
    ret = RawRobo::download( jointsGroup, jointsTabList, region );
    if ( ret != 0 )
    { return ret; }

    return 0;
}
