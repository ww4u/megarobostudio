#include "injectpump.h"


int robotInject::downloadTrace( const tpvRegion &region,
                            QList< tpvGroup *> &jointsGroup )
{
    int ret;
    if ( jointsGroup.size() != axes() )
    { return ERR_INVALID_DATA; }

    onLine();

    fsm( region )->setState( MegaDevice::mrq_state_idle );

    QList<int> jointsTabList;
    jointsTabList<<0<<1;
    ret = RawRobo::download( jointsGroup, jointsTabList, region );
    if ( ret != 0 )
    { return ret; }
//logDbg()<<jointsGroup.size();

    return 0;
}

////! \note only for internal task
//int robotInject::waitFsm( pvt_region,
//             int dstState,
//             int tmous,
//             int tick
//             )
//{
//    do
//    {
//        Q_ASSERT( tick > 0 );
//        QThread::usleep( tick );

//        if ( state( region, 1 ) == dstState )
//        { return 0; }

//        tmous -= tick;
//    }while( tmous > 0 );

//    return -1;
//}
