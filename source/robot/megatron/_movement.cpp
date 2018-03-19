#include "megatron.h"


int robotMegatron::move( WorldPoint &pt1,
                         WorldPoint &pt2,
                         float dt )
{
    int ret;

    //! 0. check
    //!

    //! 1.build
    xxxGroup<jointsTrace> jointsPlan;
    ret = buildTrace( pt1, pt2, dt, jointsPlan );
    if ( ret != 0 )
    { return ret; }

    //! 2.convert
    ret = convertTrace( pt1, pt2, dt, jointsPlan );
    if ( ret != 0 )
    { return ret; }

    //! 3.config
    setLoop( 1 );
    run();              //! request run

    //! 4.download
    ret = downloadTrace( );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int robotMegatron::moveTest1()
{
    WorldPoint pt1,pt2;
    pt1.x = 280;
    pt1.y = 21.5;
    pt1.z = 452.75;
    pt1.hand = 0;

    pt2.x = 250;
    pt2.y = 0;
    pt2.z = 502;
    pt2.hand = 90;

    float dt = 1.0;

//    return moveTest( pt1, pt2, dt );
// MOVE 280,21.5,452.75,0,250,0,502,90,1
    return move( pt1, pt2, dt );
}

int robotMegatron::moveTest2()
{
    WorldPoint pt1,pt2;
    pt1.x = 280;
    pt1.y = 21.5;
    pt1.z = 452.75;
    pt1.hand = 0;

    pt2.x = 250;
    pt2.y = 0;
    pt2.z = 502;
    pt2.hand = 90;

    float dt = 1.0;

//    return moveTest( pt2, pt1, dt );
    return move( pt2, pt1, dt );
}

int robotMegatron::moveTest( WorldPoint &pt1,
                             WorldPoint &pt2,
                             float dt )
{
    //! build
    int ret;
    xxxGroup<jointsTrace> jointsPlan;
    ret = buildTrace( pt1, pt2, dt, jointsPlan );
    if ( ret != 0 )
    { return ret; }

    for( int i = 0; i < jointsPlan.size(); i++ )
    {
        logDbg()<<jointsPlan.data()[i].t
                <<jointsPlan.data()[i].p[0]<<jointsPlan.data()[i].v[0]
                <<jointsPlan.data()[i].p[1]<<jointsPlan.data()[i].v[1]
                <<jointsPlan.data()[i].p[2]<<jointsPlan.data()[i].v[2]
                <<jointsPlan.data()[i].p[3]<<jointsPlan.data()[i].v[3];
    }

    return 0;
}

int robotMegatron::nowPose( WorldPoint &pos )
{
    //! \todo get x,y,z by angles

    pos.hand = 0;
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;

    return 0;
}
