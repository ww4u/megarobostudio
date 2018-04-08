#include "sinanju.h"
#include "../../com/comassist.h"
int robotSinanju::call( const tpvRegion &region )
{
    onLine();

    lpc()->postMsg( (eRoboMsg)(MegaDevice::mrq_msg_call), region );

    return 0;
}

//! load the data from the file
int robotSinanju::program( const QString &fileName,
                         const tpvRegion &region )
{
    //! 1.load
    int ret;
    ret = loadProgram( fileName );
    if ( ret != 0 )
    { return ret; }

    //! 2.config
    setLoop( 1 );

    //! 3.download
    ret = downloadTrace( region );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int robotSinanju::loadProgram( const QString &fileName )
{
    //!  0   1   2   3   4    5
    //!  J1  J2  J3  J4  End Time
    QList<float> dataset;
    int col = 6;logDbg();
    if ( 0 != comAssist::loadDataset( fileName, col, dataset ) )
    { return ERR_INVALID_INPUT; }logDbg()<<dataset.size();

    //! convert to tpvitem
    if ( dataset.size() < 2 * col )
    { return ERR_INVALID_INPUT; }logDbg();

    //! file split
    delete_all( mJointsGroup );logDbg();

    //! for 5 axis
    for ( int i = 0; i < 5; i++ )
    {
        tpvGroup *pGroup = new tpvGroup();
        if ( NULL == pGroup )
        { return -1; }logDbg();

        //! for each time
        int ret;
        for ( int j = 0; j < dataset.size() / col; j++ )
        {
            ret = pGroup->addItem( dataset.at( j * col + 5),        //! time
                                   dataset.at( j * col + i),        //! p
                                   0
                                   );
            if ( ret != 0 )
            {
                delete pGroup;
                return ERR_FAIL_ADD_TPV;
            }
        }

        mJointsGroup.append( pGroup );
    }

    //! log joint group
    foreach ( tpvGroup *pGp, mJointsGroup )
    {
        logDbg()<<"*******";
        foreach(  tpvItem *pItem, pGp->mItems )
        {
            logDbg()<<pItem->mT<<pItem->mP<<pItem->mV;
        }
    }

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
    float angles[4];
    MegaDevice::deviceMRQ *pDev;
    int ax;
    for ( int i = 0;  i < 4; i++ )
    {
        pDev = jointDevice( i, &ax );
        if ( NULL == pDev )
        { return -1; }

        angles[i] = pDev->getAbsAngle( ax );
        if ( angles[i] < 0 )
        { return -1; }
    }

    //! convert
    return angleToPos( angles, pos );
}

int robotSinanju::nowAngle( QList<double> &angles )
{
    MegaDevice::deviceMRQ *pDev;
    float angle;
    int ax;
    angles.clear();
    for ( int i = 0;  i < 4; i++ )
    {
        pDev = jointDevice( i, &ax );
        if ( NULL == pDev )
        { return -1; }

        angle = pDev->getAbsAngle( ax );
        if ( angle < 0 )
        { return -1; }
    }

    return 0;
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
int robotSinanju::angleToPos( float angles[4],
                               TraceKeyPoint &pos )
{
    //! convert
    jointsAngle rotAngles={ rot_angles(0,1,2,3) };
    jointsAngle archAngles={ mArchAngles.at(0),
                             mArchAngles.at(1),
                             mArchAngles.at(2),
                             mArchAngles.at(3),
                            };
    double armLength[]={ arm_lens(0,1,2,3,4,5) };
    double dAngles[4];
    double xyz[3];
    int ret;

    diffAngle( angles, dAngles );
logDbg()<<dAngles[0]<<dAngles[1]<<dAngles[2]<<dAngles[3];
logDbg()<<angles[0]<<angles[1]<<angles[2]<<angles[3];
logDbg()<<mInitAngles[0]<<mInitAngles[1]<<mInitAngles[2]<<mInitAngles[3];
    ret = ns_kinematic::GetEndPosition( armLength,sizeof_array(armLength),
                                        rotAngles.angles,
                                        archAngles.angles,
                                        dAngles,
                                        4,
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

//! - init angle
void robotSinanju::diffAngle( float angles[4],
                              float anglesOut[4]  )
{
    Q_ASSERT( mAngleDir.size() == 4 );

    for ( int i = 0; i < 4; i++ )
    {
        anglesOut[i] = (angles[i] - mInitAngles.at(i));
    }

    //! direction
    for ( int i = 0; i < 4; i++ )
    {
        if ( mAngleDir.at(i) )
        {}
        else
        { anglesOut[i] = -anglesOut[i]; }

        anglesOut[i] = comAssist::normalizeDegree360( anglesOut[i] );
    }
}
//! - init angle
void robotSinanju::diffAngle( float angles[4],
                              double anglesOut[4]  )
{
    Q_ASSERT( mAngleDir.size() == 4 );

    for ( int i = 0; i < 4; i++ )
    {
        anglesOut[i] = (angles[i] - mInitAngles.at(i));
    }

    //! direction
    for ( int i = 0; i < 4; i++ )
    {
        if ( mAngleDir.at(i) )
        {}
        else
        { anglesOut[i] = -anglesOut[i]; }

        anglesOut[i] = comAssist::normalizeDegree360( anglesOut[i] );
    }
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


