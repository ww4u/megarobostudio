#include "injectpump.h"
#include "../../model/ipmotiongroup.h"

int robotInject::build( MegaTableModel *pModel,
                         xxxGroup<tracePoint> &tracePlan,
                         xxxGroup<jointsTrace> &jointsPlan,
                         QList< tpvGroup *> &tpvGroups,
                         QList< int > &sectionList
                         )
{
    Q_ASSERT( NULL != pModel );

    IPMotionGroup *pGroup;
    pGroup = (IPMotionGroup*)pModel;

    //! build curve
    IPKeyPointList curve;
    IPKeyPoint keyP;
    for (int i = 0; i < pGroup->mItems.size(); i++ )
    {
        if ( pGroup->mItems.at(i)->enable() )
        {
//            keyP.t = pGroup->mItems.at(i)->T();
            keyP.t = pGroup->getAbsT( i );
            keyP.x = pGroup->mItems.at(i)->X();
            keyP.y = pGroup->mItems.at(i)->Y();

            curve.append( keyP );
        }
        else
        {}
    }

    int ret;
    ret = buildTrace( curve, tpvGroups, sectionList );
    if ( ret != 0 )
    { return ret; }

    return 0;
}
