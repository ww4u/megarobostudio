#include "delta.h"

#include "../../model/deltamotiongroup.h"

int robotDelta::build(   MegaTableModel *pModel,
                         xxxGroup<tracePoint> &tracePlan,
                         xxxGroup<jointsTrace> &jointsPlan,
                         QList< tpvGroup *> &tpvGroups,
                         QList< int > &sectionList
                         )
{
    Q_ASSERT( NULL != pModel );

    //! model
    DeltaMotionGroup *pGroup;
    pGroup = (DeltaMotionGroup *)pModel;
    Q_ASSERT( NULL != pModel );

    //! move curve
    QList<TraceKeyPoint> curve;
    TraceKeyPoint keyP;
    for ( int i = 0; i < pGroup->mItems.size(); i++ )
    {
        //! check enable
        if ( pGroup->mItems.at(i)->enable() )
        {
//            keyP.t = pGroup->mItems.at(i)->T();
            keyP.t = pGroup->getAbsT( i );
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
    QList<deltaPoint> jointTrace;
    ret = buildTrace( curve, jointTrace );
    if ( ret != 0 )
    {
        sysPrompt( QObject::tr( "Solve fail" ) );
        return ret;
    }

    //! convert
    ret = convertTrace( curve, jointTrace, tpvGroups, sectionList );
    if ( ret != 0 )
    { return ret; }

//    //! export
//    int lastPt = curve.size() - 1;
//    QString fileName = QString( "%1_%2_%3-%4_%5_%6.csv" ).arg( curve.at(0).x ).arg( curve.at(0).y ).arg( curve.at(0).z )
//                                                     .arg( curve.at(lastPt).x ).arg( curve.at(lastPt).y ).arg( curve.at(lastPt).z );
//    exportPlan( QCoreApplication::applicationDirPath() + "/dump/" + "plan_" + fileName, tracePlan );
//    exportJoints( QCoreApplication::applicationDirPath() + "/dump/" + "joints_" + fileName, jointsPlan );

    return 0;
}
