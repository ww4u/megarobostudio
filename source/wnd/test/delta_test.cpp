
#include <QApplication>
#include <QDebug>

#include "../../include/mcstd.h"

#include "../../arith/delta/arith_delta.h"
#include "../../com/comassist.h"
void sloveTest()
{
    QList<D4Point> points;
    D4Point pt;

//    pt.t = 0;
//    pt.x = 0;
//    pt.y = 0;
//    pt.z = 0;
//    points.append( pt );

//    pt.t = 2;
//    pt.x = 0;
//    pt.y = 0;
//    pt.z = -20;
//    points.append( pt );

    pt.t = 0;
    pt.x = 0;
    pt.y = 0;
    pt.z = 0;
    points.append( pt );

    pt.t = 2;
    pt.x = 20;
    pt.y = 0;
    pt.z = 10;
    points.append( pt );

    pt.t = 4;
    pt.x = -60;
    pt.y = 0;
    pt.z = 20;
    points.append( pt );

    int ret;
    QList<arith_delta::deltaPoint> deltaPoints;
    ret = arith_delta::ccwSlove( points, deltaPoints );
    logDbg()<<ret;

    if ( ret != 0 )
    { return; }

    for ( int i = 0; i < deltaPoints.size(); i++ )
    {
        logDbg()<<points.at(i).t<<points.at(i).x<<points.at(i).y<<points.at(i).z
                <<"----"
                <<deltaPoints.at(i).t
                <<deltaPoints.at(i).p[0]
                <<deltaPoints.at(i).p[1]
//                <<deltaPoints.at(i).p[2]
                <<deltaPoints.at(i).v[0]
                <<deltaPoints.at(i).v[1];
    }

    //! cw
    deltaPoints.clear();

    arith_delta::deltaPoint phasePoint;

    phasePoint.pl = 270;
    phasePoint.py = 0;
    phasePoint.pr = 90;
    phasePoint.tt = 0;

    deltaPoints.append( phasePoint );

    phasePoint.pl = 268;
    phasePoint.py = 0;
    phasePoint.pr = 79;
    phasePoint.tt = 2;

    deltaPoints.append( phasePoint );

    phasePoint.pl = 293;
    phasePoint.py = 0;
    phasePoint.pr = 103;
    phasePoint.tt = 4;
    deltaPoints.append( phasePoint );

    points.clear();
    ret = arith_delta::cwSlove( deltaPoints, points );
    logDbg()<<ret;
    for ( int i = 0; i < points.size(); i++ )
    {
        logDbg()<<points.at(i).t
                <<points.at(i).x
                <<points.at(i).y
                <<points.at(i).z
                <<points.at(i).v;
    }
}

#include "../../robot/delta/delta.h"
void deltaRoboTest()
{
    robotDelta delta;

    QList<TraceKeyPoint> points;
    TraceKeyPoint pt;

    pt.iMask = 0;

//    pt.t = 0;
//    pt.x = 0;
//    pt.y = -230.8074;
//    pt.z = 0;
//    pt.hand = 10;
//    points.append( pt );

//    pt.t = 2;
//    pt.x = 113.7092;
//    pt.y = -187.3488;
//    pt.z = 10;
//    pt.hand = 20;
//    points.append( pt );

//    pt.t = 4;
//    pt.x = -60;
//    pt.y = -150;
//    pt.z = 20;
//    pt.hand = 30;
//    points.append( pt );

    //! load from csv
//    QString strFile( "G:/work/mc/develope/installer/package/dataset/Delta_04-04-21-26-08-Pre.dsp" );
    QString strFile( "G:/work/mc/develope/installer/package/dataset/Delta-04-04-21-26-08-back.dsp" );
//    QString strFile( "G:/work/mc/develope/installer/package/dataset/delta.csv" );
    QList<float> dataSet;
    comAssist::loadDataset( strFile,
                            6,
                            dataSet );

    for ( int i = 0; i < dataSet.size()/6; i++ )
    {
        pt.x = dataSet.at(i*6+0);
        pt.z = dataSet.at(i*6+1) - 218.818;
        pt.y = dataSet.at(i*6+2) ;
        pt.hand = dataSet.at(i*6+3);
        pt.t = dataSet.at(i*6+4);
        pt.iMask = dataSet.at(i*6+5);
        points.append( pt );
    }

    int ret;
    QList<arith_delta::deltaPoint> deltaPoints;
    ret = delta.buildTrace( points, deltaPoints );
    logDbg()<<ret;
    if ( ret != 0 )
    { return; }

    for ( int i = 0; i <deltaPoints.size(); i++ )
    {
        logDbg()<<i
                <<deltaPoints.at(i).t
                <<deltaPoints.at(i).p[0]<<deltaPoints.at(i).p[1]
                <<deltaPoints.at(i).v[0]<<deltaPoints.at(i).v[1];
//                <<deltaPoints.at(i).a[0]<<deltaPoints.at(i).a[1]<<deltaPoints.at(i).a[2];
    }
}

int main(int argc, char *argv[] )
{
    QApplication app( argc, argv );


//    delta_test();
//    return 0;

    sloveTest();

//    deltaRoboTest();

    return 0;
}
