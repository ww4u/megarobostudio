
#include <QApplication>
#include <QDebug>

#include "../../include/mcstd.h"

//#include "../../../arith/kinematic/kinematic.h"
//#include "../../../arith/pathplan/pathplan.h"

//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);

//    int ret;

//    endPoint endPoints[] =
//    {
//        //! x, y, z, end, t, res
//        { 280,21.5,452.75, 0, 0, 1, },
//        { 250,0,502, 0, 1, 1 },
//    };

//    //! plan
//    //! trace plan
//    int xyzResLen;
//    int ret = ns_pathplan::GetPvtLen( &pEndPoints->datas,
//                                      count,
//                                      ui->spinStep->value(),
//                                      ui->cmbInterp->currentIndex(),
//                                      &xyzResLen );


//    double angles[]={ 0,90,-90,0 };



//    ret = ns_kinematic::GetArmPosition( (double*)(&angle),

//                    &m_pTracePoint->datas + offsetof_double( tracePoint, x ), sizeof_double(tracePoint),
//                    &m_pTracePoint->datas + offsetof_double( tracePoint, vx ),sizeof_double(tracePoint),
//                    &m_pTracePoint->datas + offsetof_double( tracePoint, t ), sizeof_double(tracePoint),

//                    mTracePointSize,
//                    &m_pJointsTrace->datas
//                    );




//    QFile file("E:/trash/a.csv");
//    file.open( QIODevice::WriteOnly );
//    QTextStream textStream( &file );

//    for ( int i = 0; i < tps.size(); i++ )
//    {
//        textStream<<tps[i].x()<<","<<tps[i].y()<<","<<tvs[i].y()<<"\n";
//    }

//    file.close();

//    return 0;
//}

#include "../../../arith/pathplan/pathplan.h"
#include "../../../arith/kinematic/kinematic.h"

void kinematicFullTest()
{
    //! test plan
    endPoint points[]=
    {
        //! x, y, z, end, t, interp
        { 280,21.5,452.75,  0,0,1,},
        { 250,0,502,        0,1,1,},
//        { 280,21.5,452.75,  0,2,1,},
    };

    int resLen;
    int ret = ns_pathplan::GetPvtLen( (double*)points, sizeof_array(points), 5, 0, &resLen );

    logDbg()<<resLen<<ret;
    xxxGroup<tracePoint> tracePoints;
    int traceCount = resLen * sizeof(double)/ sizeof(tracePoint);

    tracePoints.alloc( traceCount );

    ns_pathplan::GetPvtInfo( &tracePoints.data()->datas, resLen );

    tracePoint *pTrace = tracePoints.data();

    for( int i = 0; i < tracePoints.size(); i++ )
    {
        logDbg()<<i<<pTrace[i].t<<pTrace[i].x<<pTrace[i].y<<pTrace[i].z<<pTrace[i].vx<<pTrace[i].vy<<pTrace[i].vz;
    }
//    return ;

    //! arm
    jointsAngle refAngle={ 0, 90, -90, -90 };
    jointsAngle convertAngle={ 0, 90, 180, 180 };
    double armLength[]={ 247.75, 255, 250, 0, 0, 0 };

    double xyz[3];
    ns_kinematic::GetEndPosition( armLength,sizeof_array(armLength),
                                  convertAngle.angles,
                                  refAngle.angles,
                                  4,
                                  xyz );
    logDbg()<<xyz[0]<<xyz[1]<<xyz[2];

    int outSize;
    ret = ns_kinematic::getArmPosition_Size(

                    armLength,sizeof_array(armLength),
                    convertAngle.angles, 4,
                    refAngle.angles,

                    &pTrace->datas + offsetof( tracePoint, x )/sizeof(double), sizeof(tracePoint)/sizeof(double),
                    &pTrace->datas + offsetof( tracePoint, vx )/sizeof(double),sizeof(tracePoint)/sizeof(double),
                    &pTrace->datas + offsetof( tracePoint, t )/sizeof(double),sizeof(tracePoint)/sizeof(double),

                    traceCount,
                    &outSize

                    );
    if ( ret == 0 && outSize > 0 )
    {}
    else
    { return; }

    xxxGroup<jointsTrace> jointTraces;
    if ( 0 != jointTraces.alloc( outSize ) )
    { return; }

    ret = ns_kinematic::getArmPosition_Data(

                    armLength,sizeof_array(armLength),
                    convertAngle.angles, 4,
                    refAngle.angles,

                    &pTrace->datas + offsetof( tracePoint, x )/sizeof(double), sizeof(tracePoint)/sizeof(double),
                    &pTrace->datas + offsetof( tracePoint, vx )/sizeof(double),sizeof(tracePoint)/sizeof(double),
                    &pTrace->datas + offsetof( tracePoint, t )/sizeof(double),sizeof(tracePoint)/sizeof(double),

                    traceCount,
                    &jointTraces.data()->datas,
                    outSize );
    logDbg()<<ret;
    jointsTrace *pJointsTrace = jointTraces.data();
    if ( ret == 0 )
    {
        QFile file("e:/trash/k.csv");
        if ( file.open( QIODevice::WriteOnly ) )
        {}
        else
        { return; }

        logDbg()<<jointTraces.size();
        QTextStream textStream( &file );

        textStream<<"t"<<","
                <<"p1"<<","
                <<"v1"<<","
                <<"p2"<<","
                <<"v2"<<","
                <<"p3"<<","
                <<"v3"<<","
                <<"p4"<<","
                <<"v4"<<"\n";
        for ( int i = 0; i < jointTraces.size(); i++ )
        {
            textStream<<pJointsTrace[i].t<<","
                    <<pJointsTrace[i].p[0]<<","
                    <<pJointsTrace[i].v[0]<<","
                    <<pJointsTrace[i].p[1]<<","
                    <<pJointsTrace[i].v[1]<<","
                    <<pJointsTrace[i].p[2]<<","
                    <<pJointsTrace[i].v[2]<<","
                    <<pJointsTrace[i].p[3]<<","
                    <<pJointsTrace[i].v[3]<<"\n";
        }

        file.close();
    }
}

void kinematic_CrossTest()
{
    ns_kinematic::zeroCrossTest();
}

int main(int argc, char *argv[] )
{
    QApplication app( argc, argv );

//    kinematic_CrossTest();

    kinematicFullTest();

    return 0;
}
