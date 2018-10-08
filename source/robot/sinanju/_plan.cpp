#include "sinanju.h"
#include "../../com/comassist.h"
int robotSinanju::buildTrace( QList<TraceKeyPoint> &curve,
                              xxxGroup<tracePoint> &tracePlan,
                              xxxGroup<jointsTrace> &jointsPlan )
{
    int ret;

    ret = verifyTrace( curve );
    if ( ret != 0 )
    { return ret; }

    //! coord rotate
    coordRotate( curve );

    //! interp
    interpTune( curve );

logDbg()<<curve.size();
    ret = planTrace( curve, tracePlan );
    if ( ret != 0 )
    {
        sysError( QObject::tr("interp fail") );
        return ret;
    }
logDbg()<<tracePlan.size();
for ( int i = 0; i < tracePlan.size(); i++ )
{
    logDbg()<<tracePlan.data()[i].t
            <<tracePlan.data()[i].x
            <<tracePlan.data()[i].y
            <<tracePlan.data()[i].z;
}
    ret = splitTrace( tracePlan, jointsPlan );
    if ( ret != 0 )
    {
        sysError( QObject::tr("deslove fail") );
        return ret;
    }
logDbg();
    return 0;
}

int robotSinanju::verifyTrace( QList<TraceKeyPoint> &curve )
{
    if ( curve.size() < 2 )
    {
        sysError( QObject::tr("inefficient curve %1").arg( curve.size() ) );
        return -1;
    }

    float tPre;
    tPre = curve.at(0).t;
    if ( tPre < 0 )
    { return -1; }

    for ( int i = 1; i < curve.size(); i++ )
    {
        if ( tPre < curve.at(i).t )
        {}
        else
        {
            sysError( QObject::tr("invalid time at %1 %2").arg(i).arg( curve.at(i).t ) );
            return -1;
        }

        tPre = curve.at(i).t;
    }

    return 0;
}

void robotSinanju::interpTune( QList<TraceKeyPoint> &curve )
{
    float dist;
    for ( int i = 0; i < curve.size() - 1; i++ )
    {
        //! check distance
        dist = comAssist::eulcidenDistance( curve.at(i).x, curve.at(i).y, curve.at(i).z,
                                            curve.at(i+1).x, curve.at(i+1).y, curve.at(i+1).z
                                            );
        //! todo by out
        if ( dist < 5 )
        {
            unset_bit( curve[i].iMask, BIT_INTERP );
        }
    }
}

void robotSinanju::coordRotate( QList<TraceKeyPoint> &curve )
{
    if ( mbTransferable )
    {
        TraceKeyPoint localPoint;
        for ( int i = 0; i < curve.size(); i++ )
        {
            localPoint = curve.at(i);

            coordRotate( localPoint, mTransferR, mTransferS );

            curve[ i ] = localPoint;
        }
    }
}

//! A = rot * pt + shift
void robotSinanju::coordRotate( TraceKeyPoint &pt, double rot[3*3], double shift[3*1] )
{
    TraceKeyPoint localPt;

    localPt = pt;

    //! out
    pt.x = rot[0*3+0] * localPt.x
                    + rot[0*3+1] * localPt.y
                    + rot[0*3+2] * localPt.z
                    + shift[0*1+0];

    pt.y = rot[1*3+0] * localPt.x
                    + rot[1*3+1] * localPt.y
                    + rot[1*3+2] * localPt.z
                    + shift[1*1+0];

    pt.z = rot[2*3+0] * localPt.x
                    + rot[2*3+1] * localPt.y
                    + rot[2*3+2] * localPt.z
                    + shift[2*1+0];

    logDbg()<<localPt.t<<localPt.x<<localPt.y<<localPt.z<<pt.x<<pt.y<<pt.z;
}

void robotSinanju::coordIRotate( TraceKeyPoint &pt )
{
    if ( mbTransferable )
    { coordIRotate( pt, mTransferRInv, mTransferS ); }
    else
    {  }
}

//! pt = rot * (A - shift)
void robotSinanju::coordIRotate( TraceKeyPoint &pt, double rot[3*3], double shift[3*1] )
{
    TraceKeyPoint localPt;

    localPt = pt;

    //! -
    localPt.x = pt.x - shift[0*1+0];
    localPt.y = pt.y - shift[1*1+0];
    localPt.z = pt.z - shift[2*1+0];

    //! out
    pt.x = rot[0*3+0] * localPt.x
                    + rot[0*3+1] * localPt.y
                    + rot[0*3+2] * localPt.z;

    pt.y = rot[1*3+0] * localPt.x
                    + rot[1*3+1] * localPt.y
                    + rot[1*3+2] * localPt.z;

    pt.z = rot[2*3+0] * localPt.x
                    + rot[2*3+1] * localPt.y
                    + rot[2*3+2] * localPt.z;

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
    int ret = ns_pathplan::GetPointLen( &endPoints.data()->datas,
                                      curve.size(),
                                      mPlanAttr.mStep,
                                      mPlanAttr.mMode,
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

    ret = ns_pathplan::GetPointInfo( &tracePlan.data()->datas, xyzResLen );
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
//    ret = ns_kinematic::getArmPosition_Size(

//                    armLength,sizeof_array(armLength),
////                    convertAngle.angles, sizeof_array(convertAngle.angles),
////                    refAngle.angles,

//                    convertAngle.angles,
//                    refAngle.angles,
//                    sizeof_array(convertAngle.angles),

//                    &tracePoints.data()->datas + offsetof_double( tracePoint, x ), sizeof_double(tracePoint),
//                    &tracePoints.data()->datas + offsetof_double( tracePoint, vx ),sizeof_double(tracePoint),
//                    &tracePoints.data()->datas + offsetof_double( tracePoint, t ), sizeof_double(tracePoint),

//                    tracePoints.size(),

//                    &nRes
//                    );

    double archAngles[]= {0,90,180,180};
    double initAngles[]= {0,180,90,90};
    ret = ns_sinanju::GetPvtLen( armLength,
                                   archAngles,
                                   initAngles,
                                   &tracePoints.data()->datas + offsetof_double( tracePoint, x ), sizeof_double(tracePoint),
                                   &tracePoints.data()->datas + offsetof_double( tracePoint, vx ), sizeof_double(tracePoint),
                                   &tracePoints.data()->datas + offsetof_double( tracePoint, t ), sizeof_double(tracePoint),

                                   tracePoints.size(),
                                   &nRes

                                   );
    if ( ret == 0 && nRes > 0 )
    {}
    else
    { logDbg(); return ERR_PLAN_SLOVE_FAIL; }

    //! data
    if ( 0 != traceJoints.alloc( nRes ) )
    { logDbg(); return ERR_ALLOC_FAIL; }

    ret = ns_sinanju::GetPvtInfo( &traceJoints.data()->datas );

    if ( ret != 0 )
    { logDbg(); return ERR_PLAN_SLOVE_FAIL; }

    return 0;
}
