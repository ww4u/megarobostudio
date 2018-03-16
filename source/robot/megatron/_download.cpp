#include "megatron.h"

int robotMegatron::convertTrace(   WorldPoint &pt1,
                    WorldPoint &pt2,
                    float dt,
                    xxxGroup<jointsTrace> &jointsPlan )
{
    //! 0.check
    if( jointsPlan.size() > 0 )
    {}
    else
    { return ERR_NO_TPV_DATA; }

    int ret;
    delete_all( mJointsGroup );

    //! 0~3
    ret = buildTpvGroup( jointsPlan, mJointsGroup );
    if ( ret != 0 )
    { return ret; }

    //! tail
    //! for hand
    tpvGroup *pGroup = new tpvGroup();
    if ( NULL == pGroup )
    { return ERR_ALLOC_FAIL; }

    ret = pGroup->addItem( 0, pt1.hand, 0 );
    if ( ret != 0 )
    { return ERR_FAIL_ADD_TPV; }
    ret = pGroup->addItem( dt, pt2.hand, 0 );
    if ( ret != 0 )
    { return ERR_FAIL_ADD_TPV; }

    mJointsGroup.append( pGroup );
logDbg()<<dt<<pt1.hand<<pt2.hand;
// MOVE 280,21.5,452.75,0,250,0,502,90,0.5
    return 0;
}

//! only for 4 joints
int robotMegatron::downloadTrace( )
{
    int ret;
    if ( mJointsGroup.size() != axes() )
    { return ERR_INVALID_DATA; }

    onLine();

    QList<int> jointsTabList;
    jointsTabList<<0<<1<<2<<3<<4;
    ret = download( mJointsGroup, jointsTabList );
    if ( ret != 0 )
    { return ret; }
logDbg()<<mJointsGroup.size();

    return 0;
}

int robotMegatron::buildTpvGroup( xxxGroup<jointsTrace> &jointsPlan,
                                  QList< tpvGroup *> &jointsGroup )
{
    //! create each joints group
    delete_all( jointsGroup );

    int ret;
    //! for each axis
    for ( int i = 0; i < 4; i++ )
    {
        tpvGroup *pGroup = new tpvGroup();
        Q_ASSERT( NULL != pGroup );

        //! for each data
        for ( int j = 0; j < jointsPlan.size(); j++ )
        {
            ret = pGroup->addItem(
                             jointsPlan.data()[j].t,
                             jointsPlan.data()[j].p[i],
                             jointsPlan.data()[j].v[i]
                             );
            if ( ret != 0 )
            {
                delete_all( jointsGroup );
                return ret;
            }
        }

        jointsGroup.append( pGroup );
    }

    return 0;
}
