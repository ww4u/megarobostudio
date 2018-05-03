
#include <QApplication>
#include <QDebug>

#include "../../include/mcstd.h"
#include "../../com/basetype.h"
//#include "../../../arith/kinematic/kinematic.h"
#include "../../../arith/pathplan/pathplan.h"

//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);


//    endPoint endPoints[] =
//    {
//        //! x, y, z, end, t, res
//        { 280,21.5,452.75, 0, 0, 1, },
//        { 250,0,502, 0, 1, 1 },
//    };

//    //! plan
//    //! trace plan
//    int xyzResLen;
//    int ret = ns_pathplan::GetPvtLen( &endPoints[0].datas,
//                                      2,
//                                      0.005,
//                                      0,
//                                      &xyzResLen );


//    double angles[]={ 0,90,-90,0 };



////    ret = ns_kinematic::GetArmPosition( (double*)(&angle),

////                    &m_pTracePoint->datas + offsetof_double( tracePoint, x ), sizeof_double(tracePoint),
////                    &m_pTracePoint->datas + offsetof_double( tracePoint, vx ),sizeof_double(tracePoint),
////                    &m_pTracePoint->datas + offsetof_double( tracePoint, t ), sizeof_double(tracePoint),

////                    mTracePointSize,
////                    &m_pJointsTrace->datas
////                    );




//    QFile file("E:/trash/a.csv");
//    file.open( QIODevice::WriteOnly );
//    QTextStream textStream( &file );

////    for ( int i = 0; i < tps.size(); i++ )
////    {
////        textStream<<tps[i].x()<<","<<tps[i].y()<<","<<tvs[i].y()<<"\n";
////    }

//    file.close();

//    return 0;
//}

#include "../../../arith/pathplan/pathplan.h"
#include "../../../arith/sinanju_split/sinanju_split.h"

void kinematicFullTest()
{
    //! test plan
    endPoint points[]=
    {
        //! x, y, z, end, t, interp
//        { 280,21.5,452.75,  0,0,1,},
//        { 250,0,502,        0,1,1,},
////        { 280,21.5,452.75,  0,2,1,},
//        { 250,0,512,  0,0,1,},
//        { 280,21,512,  0,1,1,},

//        { 250,0,512,  0,0,1,},
//        { 19.4,1.4,704, 0, 5, 1 },
//        { 250,0,512,  0,10,1,},

//        { 118.77,12.44,494,  0,0,1,},
//        { 250,0,512,  0,10,1,},

        { 250,	0,	512,0,0,1},
        { 204.4,	-51,	257,0,1,1,},
        { 203.5,	-51,	257,0,2,1,},


//        { 250,0,512,  0,0,1,},
//        { 240,0,512,  0,5,1,},

//        { 250,0,512,  0,10,1,},
    };

    int resLen;
    int ret = ns_pathplan::GetPointLen( (double*)points, sizeof_array(points), 2, 0, &resLen );

    logDbg()<<resLen<<ret;
    xxxGroup<tracePoint> tracePoints;
    int traceCount = resLen * sizeof(double)/ sizeof(tracePoint);

    tracePoints.alloc( traceCount );

    ns_pathplan::GetPointInfo( &tracePoints.data()->datas, resLen );

    tracePoint *pTrace = tracePoints.data();

    QFile file("E:/trash/a.csv");
    file.open( QIODevice::WriteOnly );
    QTextStream textStream( &file );
//    textStream.setPadChar(' ');
//    textStream.setFieldWidth( 10 );
//    textStream.setFieldAlignment( QTextStream::AlignLeft );
    for( int i = 0; i < tracePoints.size(); i++ )
    {
//        logDbg()<<i<<pTrace[i].t<<pTrace[i].x<<pTrace[i].y<<pTrace[i].z<<pTrace[i].vx<<pTrace[i].vy<<pTrace[i].vz;
        textStream<<pTrace[i].t<<","
                  <<pTrace[i].x<<","
                  <<pTrace[i].y<<","
                  <<pTrace[i].z<<","
                  <<pTrace[i].vx<<","
                  <<pTrace[i].vy<<","
                  <<pTrace[i].vz<<"\n";
    }
    file.close();
//    return ;

//    //! arm
////    jointsAngle refAngle={ 0, 90, -90, -90 };
////    jointsAngle convertAngle={ 0, 90, 180, 180 };

////    jointsAngle refAngle={ 0, 90, -90, -90 };
////    jointsAngle refAngle={ 0, 180, -90, -90 };    //! 250 0 502
////    jointsAngle refAngle={ 0, 180, 0, -90 };        //! 0,0,752
////    jointsAngle refAngle={ 0, 180, 90, -90 };        //! -250,0,502
////    jointsAngle refAngle={ 0, 180, 90, 90 };        //! -250,0,502
////    jointsAngle refAngle={ 0, -90, 0, 0 };        //! -505,0,247
////    jointsAngle refAngle={ 0, 90, 0, 0 };        //! 505,0,247
//    jointsAngle refAngle={ 0, 135, -90, 0 };        //! 505,0,247
////    jointsAngle convertAngle={ 0, 90, -90, -90 };

//    jointsAngle convertAngle={ 0, 90, 0, -90 };
//    jointsAngle archAngle={ 0, 180, -90, -90 };
////    jointsAngle shiftAngle={ 0, 0, 0, 0 };
//    jointsAngle shiftAngle={ 0, 90, 0, 0 };

    double armLength[]={ 257, 255, 250, 0, 0, 0 };

//    double xyz[3];
//    ns_sinanju::GetEndPosition( armLength,sizeof_array(armLength),
//                                  convertAngle.angles,
//                                  archAngle.angles,
//                                  shiftAngle.angles,
//                                  4,
//                                  xyz );
//    logDbg()<<xyz[0]<<xyz[1]<<xyz[2];

    int outSize;
    double archAngles[]= {0,90,180,180};
    double initAngles[]= {0,180,90,90};
    ret = ns_sinanju::GetPvtLen(

                armLength,
                                                   archAngles,
                                                   initAngles,

                    &pTrace->datas + offsetof( tracePoint, x )/sizeof(double), sizeof(tracePoint)/sizeof(double),
                    &pTrace->datas + offsetof( tracePoint, vx )/sizeof(double),sizeof(tracePoint)/sizeof(double),
                    &pTrace->datas + offsetof( tracePoint, t )/sizeof(double),sizeof(tracePoint)/sizeof(double),

                    traceCount,
                    &outSize

                    );
    logDbg()<<ret<<outSize<<traceCount;
    if ( ret == 0 && outSize > 0 )
    {}
    else
    { return; }

    xxxGroup<jointsTrace> jointTraces;
    if ( 0 != jointTraces.alloc( outSize ) )
    { return; }

    ret = ns_sinanju::GetPvtInfo(
                    &jointTraces.data()->datas
                     );
    logDbg()<<ret;
    jointsTrace *pJointsTrace = jointTraces.data();
    if ( ret == 0 )
    {
        QFile fileb("e:/trash/k.csv");
        if ( fileb.open( QIODevice::WriteOnly ) )
        {}
        else
        { return; }

        logDbg()<<jointTraces.size();
        QTextStream textStream( &fileb );

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

        fileb.close();
    }
}

//void kinematic_CrossTest()
//{
//    ns_kinematic::zeroCrossTest();
//}

int main(int argc, char *argv[] )
{
    QApplication app( argc, argv );

//    kinematic_CrossTest();

    kinematicFullTest();

    return 0;
}
