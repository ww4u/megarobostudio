#include "sinanju.h"


int robotSinanju::buildTrace( QList<TraceKeyPoint> &curve,
                xxxGroup<jointsTrace> &jointsPlan )
{
    int ret;

    xxxGroup<tracePoint> tracePlan;
    ret = planTrace( curve, tracePlan );
    if ( ret != 0 )
    { return ret; }

    ret = splitTrace( tracePlan, jointsPlan );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int robotSinanju::planTrace( QList<TraceKeyPoint> &curve,
                              xxxGroup<tracePoint> &tracePlan )
{
    xxxGroup<endPoint> endPoints;

    //! alloc
    if ( 0 != endPoints.alloc( curve.size() ) )
    { return ERR_ALLOC_FAIL; }

    //! fill 0
    memset( endPoints.data(), 0, sizeof(endPoints)*curve.size() );

    //! interp fill
    for ( int i = 0; i < curve.size(); i++ )
    {
        //! need interp
        if ( is_bit1( curve.at(i).iMask, BIT_INTERP) )
        { endPoints.data()[i].flagInterp = 1; }
        else
        { endPoints.data()[i].flagInterp = 0; }
    }

    //! xyzt
    for( int i = 0; i < curve.size(); i++ )
    {
        endPoints.data()[i].x = curve.at( i ).x;
        endPoints.data()[i].y = curve.at( i ).y;
        endPoints.data()[i].z = curve.at( i ).z;
        endPoints.data()[i].t = curve.at( i ).t;
    }

    int xyzResLen;
    int ret = ns_pathplan::GetPvtLen( &endPoints.data()->datas,
                                      curve.size(),
                                      mPlanStep,
                                      mPlanMode,
                                      &xyzResLen );
    if ( ret != 0 )
    { return ERR_PLAN_FAIL; }

    int traceSize;
    traceSize = xyzResLen * sizeof(double) / sizeof(tracePoint);
    if ( traceSize > 1 )
    { }
    else
    { return ERR_PLAN_FAIL; }

    tracePlan.clear();
    if ( 0 != tracePlan.alloc( traceSize ) )
    { return ERR_ALLOC_FAIL; }

    ret = ns_pathplan::GetPvtInfo( &tracePlan.data()->datas, xyzResLen );
    if ( ret != 0 )
    { return ERR_PLAN_FAIL; }

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

int robotSinanju::splitTrace( xxxGroup<tracePoint> &tracePoints,
                                xxxGroup<jointsTrace> &traceJoints )
{
    jointsAngle refAngle={ ref_angles(0,1,2,3) };       //! \todo ref angle by now
    jointsAngle convertAngle={ rot_angles(0,1,2,3) };
    double armLength[]={ arm_lens(0,1,2,3,4,5) };

    //! size
    int ret, nRes;
    ret = ns_kinematic::getArmPosition_Size(

                    armLength,sizeof_array(armLength),
//                    convertAngle.angles, sizeof_array(convertAngle.angles),
//                    refAngle.angles,

                    convertAngle.angles,
                    refAngle.angles,
                    sizeof_array(convertAngle.angles),

                    &tracePoints.data()->datas + offsetof_double( tracePoint, x ), sizeof_double(tracePoint),
                    &tracePoints.data()->datas + offsetof_double( tracePoint, vx ),sizeof_double(tracePoint),
                    &tracePoints.data()->datas + offsetof_double( tracePoint, t ), sizeof_double(tracePoint),

                    tracePoints.size(),

                    &nRes
                    );
    if ( ret == 0 && nRes > 0 )
    {}
    else
    { return ERR_PLAN_SLOVE_FAIL; }

    //! data
    if ( 0 != traceJoints.alloc( nRes ) )
    { return ERR_ALLOC_FAIL; }

    ret = ns_kinematic::getArmPosition_Data(

                armLength,sizeof_array(armLength),
//                convertAngle.angles, sizeof_array(convertAngle.angles),
//                refAngle.angles,

                convertAngle.angles,
                refAngle.angles,
                sizeof_array(convertAngle.angles),

                &tracePoints.data()->datas + offsetof_double( tracePoint, x ), sizeof_double(tracePoint),
                &tracePoints.data()->datas + offsetof_double( tracePoint, vx ),sizeof_double(tracePoint),
                &tracePoints.data()->datas + offsetof_double( tracePoint, t ), sizeof_double(tracePoint),

                tracePoints.size(),

                &traceJoints.data()->datas,
                traceJoints.size()
                );
    if ( ret != 0 )
    { return ERR_PLAN_SLOVE_FAIL; }

    return 0;
}
