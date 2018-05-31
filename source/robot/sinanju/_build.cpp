#include "sinanju.h"
#include "../../model/sinanjumotiongroup.h"

int robotSinanju::build( MegaTableModel *pModel,
                         xxxGroup<tracePoint> &tracePlan,
                         xxxGroup<jointsTrace> &jointsPlan,
                         QList< tpvGroup *> &tpvGroups,
                         QList< int > &sectionList
                         )
{
    Q_ASSERT( NULL != pModel );

    //! model
    SinanjuMotionGroup *pGroup;
    pGroup = (SinanjuMotionGroup *)pModel;
    Q_ASSERT( NULL != pModel );

    //! move curve
    QList<TraceKeyPoint> curve;
    TraceKeyPoint keyP;
    for ( int i = 0; i < pGroup->mItems.size(); i++ )
    {
        //! check enable
        if ( pGroup->mItems.at(i)->enable() )
        {
            keyP.t = pGroup->mItems.at(i)->T();
            keyP.x = pGroup->mItems.at(i)->X();
            keyP.y = pGroup->mItems.at(i)->Y();
            keyP.z = pGroup->mItems.at(i)->Z();
            keyP.hand = pGroup->mItems.at(i)->H();

            keyP.iMask = pGroup->mItems.at(i)->IAttr();

            curve.append( keyP );
        }
    }

    //! build
    int ret;
    ret = buildTrace( curve, tracePlan, jointsPlan );
    if ( ret != 0 )
    { return ret; }
logDbg();
    //! convert
    ret = convertTrace( curve, jointsPlan, tpvGroups, sectionList );
    if ( ret != 0 )
    { return ret; }
logDbg();

//    //! export
//    int lastPt = curve.size() - 1;
//    QString fileName = QString( "%1_%2_%3-%4_%5_%6.csv" ).arg( curve.at(0).x ).arg( curve.at(0).y ).arg( curve.at(0).z )
//                                                     .arg( curve.at(lastPt).x ).arg( curve.at(lastPt).y ).arg( curve.at(lastPt).z );
//    exportPlan( QCoreApplication::applicationDirPath() + "/dump/" + "plan_" + fileName, tracePlan );
//    exportJoints( QCoreApplication::applicationDirPath() + "/dump/" + "joints_" + fileName, jointsPlan );

    return 0;
}

void robotSinanju::exportPlan( const QString &fileName,
                               xxxGroup<tracePoint> &tracePlan )
{
    QFile file(fileName);

    if ( file.open(QIODevice::WriteOnly) )
    {}
    else
    { return; }

    QTextStream stream( &file );
//    stream.setRealNumberNotation( QTextStream::SmartNotation );
    stream.setRealNumberPrecision( 8 );
    stream<<"t"<<COL_SEP
          <<"x"<<COL_SEP
          <<"y"<<COL_SEP
          <<"z"<<COL_SEP
          <<"vx"<<COL_SEP
          <<"vy"<<COL_SEP
          <<"vz"<<ROW_SEP;
    for( int i = 0; i < tracePlan.size(); i++ )
    {
        stream<<tracePlan.data()[i].t<<COL_SEP
              <<tracePlan.data()[i].x<<COL_SEP
              <<tracePlan.data()[i].y<<COL_SEP
              <<tracePlan.data()[i].z<<COL_SEP
              <<tracePlan.data()[i].vx<<COL_SEP
              <<tracePlan.data()[i].vy<<COL_SEP
              <<tracePlan.data()[i].vz<<ROW_SEP;
    }

    file.close();
}
void robotSinanju::exportJoints( const QString &fileName,
                                  xxxGroup<jointsTrace> &jointsPlan )
{
    QFile file(fileName);

    if ( file.open(QIODevice::WriteOnly) )
    {}
    else
    { return; }

    QTextStream stream( &file );
    stream.setRealNumberNotation( QTextStream::SmartNotation );
    stream.setRealNumberPrecision( 8 );

    stream<<"t"<<COL_SEP
          <<"p0"<<COL_SEP
          <<"p1"<<COL_SEP
          <<"p2"<<COL_SEP
          <<"p3"<<COL_SEP
          <<"v0"<<COL_SEP
          <<"v1"<<COL_SEP
          <<"V2"<<COL_SEP
          <<"v3"<<ROW_SEP;
    for( int i = 0; i < jointsPlan.size(); i++ )
    {
        stream<<jointsPlan.data()[i].t<<COL_SEP
              <<jointsPlan.data()[i].p[0]<<COL_SEP
              <<jointsPlan.data()[i].p[1]<<COL_SEP
              <<jointsPlan.data()[i].p[2]<<COL_SEP
              <<jointsPlan.data()[i].p[3]<<COL_SEP
              <<jointsPlan.data()[i].v[0]<<COL_SEP
              <<jointsPlan.data()[i].v[1]<<COL_SEP
              <<jointsPlan.data()[i].v[2]<<COL_SEP
              <<jointsPlan.data()[i].v[3]<<ROW_SEP;
    }

    file.close();
}

