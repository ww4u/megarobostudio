#include "sinanju.h"

int robotSinanju::call( const tpvRegion &region )
{
    onLine();

    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_call), region );

    return 0;
}

int robotSinanju::program( QList<TraceKeyPoint> &curve,
             const tpvRegion &region )
{
    int ret;

    //! 0. check
    //!

    //! 1.build
    xxxGroup<jointsTrace> jointsPlan;
    ret = buildTrace( curve, jointsPlan );
    if ( ret != 0 )
    { return ret; }

    //! 2.convert
    ret = convertTrace( curve, jointsPlan );
    if ( ret != 0 )
    { return ret; }

    //! 3.config
    setLoop( 1 );

    //! 4.download
    ret = downloadTrace( region );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int robotSinanju::move( QList<TraceKeyPoint> &curve,
                        const tpvRegion &region )
{
    int ret;

    //! request run
    run( region );

    //! program
    ret = program( curve, region );

    return ret;
}

int robotSinanju::moveTest1( const tpvRegion &region )
{
    TraceKeyPoint pt1,pt2;
    pt1.t = 0;
    pt1.x = 280;
    pt1.y = 21.5;
    pt1.z = 452.75;
    pt1.hand = 0;

    pt2.t = 1;
    pt2.x = 250;
    pt2.y = 0;
    pt2.z = 502;
    pt2.hand = 270;

    QList<TraceKeyPoint> curve;
    curve.append( pt1 );
    curve.append( pt2 );
//    return moveTest( pt1, pt2, dt );
// MOVE 280,21.5,452.75,0,250,0,502,90,1
// MOVE 250,0,502,90,280,21.5,452.75,0,1
    return move( curve, region );
}

int robotSinanju::moveTest2( const tpvRegion &region )
{
    TraceKeyPoint pt1,pt2;
    pt1.t = 1;
    pt1.x = 280;
    pt1.y = 21.5;
    pt1.z = 452.75;
    pt1.hand = 0;

    pt2.t = 0;
    pt2.x = 250;
    pt2.y = 0;
    pt2.z = 502;
    pt2.hand = 270;

    //! p2 -> p1
    QList<TraceKeyPoint> curve;
    curve.append( pt2 );
    curve.append( pt1 );

    return move( curve, region );
}


#define ref_angle(id)   mRefAngles.at(id)
#define rot_angle(id)   mRotateAngles.at(id)
#define arm_len(id)     mArmLengths.at(id)

#define ref_angles( id1,id2,id3,id4 )   ref_angle( id1 ),\
                                        ref_angle( id2 ),\
                                        ref_angle( id3 ),\
                                        ref_angle( id4 ),
#define rot_angles( id1,id2,id3,id4 )   rot_angle( id1 ),\
                                        rot_angle( id2 ),\
                                        rot_angle( id3 ),\
                                        rot_angle( id4 ),
#define arm_lens( id1,id2,id3,id4,id5,id6 )     arm_len( id1 ),\
                                                arm_len( id2 ),\
                                                arm_len( id3 ),\
                                                arm_len( id4 ),\
                                                arm_len( id5 ),\
                                                arm_len( id6 ),
int robotSinanju::nowPose( TraceKeyPoint &pos )
{
    //! get
    double angles[4];
    MegaDevice::deviceMRQ *pDev;
    int ax;
    for ( int i = 0;  i < 4; i++ )
    {
        pDev = jointDevice( i, &ax );
        if ( NULL == pDev )
        { return -1; }

        angles[i] = pDev->getAbsAngle( ax );
    }

    //! convert
//    jointsAngle refAngle={ ref_angles(0,1,2,3) };       //! \todo ref angle by now
    jointsAngle convertAngle={ rot_angles(0,1,2,3) };
    double armLength[]={ arm_lens(0,1,2,3,4,5) };

    double xyz[3];
    int ret;
    ret = ns_kinematic::GetEndPosition( armLength,sizeof_array(armLength),
                                  convertAngle.angles,
//                                  refAngle.angles,
                                  angles,
                                  sizeof_array(convertAngle.angles),
                                  xyz
                                  );
    if ( ret != 0 )
    { return -1; }

    //! export
    pos.hand = 0;
    pos.x = xyz[0];
    pos.y = xyz[1];
    pos.z = xyz[2];
    pos.t = 0;

    return 0;
}

int robotSinanju::nowDist( QList<float> &dists )
{
    //! get
    float dist;
    MegaDevice::deviceMRQ *pDev;
    int ax;
    for ( int i = 0;  i < 4; i++ )
    {
        pDev = jointDevice( i, &ax );
        if ( NULL == pDev )
        { return -1; }

        dist = pDev->getDist( ax );
        dists.append( dist );
    }

    return 0;
}
