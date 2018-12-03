#include "h2z.h"
#include "../../model/h2zmotiongroup.h"

int robotH2Z::build( MegaTableModel *pModel,
                         xxxGroup<tracePoint> &tracePlan,
                         xxxGroup<jointsTrace> &jointsPlan,
                         QList< tpvGroup *> &tpvGroups,
                         QList< int > &sectionList
                         )
{
    Q_ASSERT( NULL != pModel );

    H2ZMotionGroup *pGroup;
    pGroup = (H2ZMotionGroup*)pModel;

    //! build curve
    QList<H2ZKeyPoint> curve;
    H2ZKeyPoint keyP;
    for (int i = 0; i < pGroup->mItems.size(); i++ )
    {
        if ( pGroup->mItems.at(i)->enable() )
        {
            keyP.t = pGroup->mItems.at(i)->T();
            keyP.x = pGroup->mItems.at(i)->X();
            keyP.y = pGroup->mItems.at(i)->Y();
            keyP.z = pGroup->mItems.at(i)->Z();

            curve.append( keyP );
        }
        else
        {}
    }

    int ret;

    ret = verifyTrace( curve );
    if ( ret != 0 )
    { return ret; }

    ret = buildTrace( curve, tpvGroups, sectionList );
    if ( ret != 0 )
    { return ret; }

    return 0;
}
