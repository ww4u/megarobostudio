#include "h2.h"
#include "../../model/h2motiongroup.h"

int robotH2::build( MegaTableModel *pModel,
                         xxxGroup<tracePoint> &tracePlan,
                         xxxGroup<jointsTrace> &jointsPlan,
                         QList< tpvGroup *> &tpvGroups,
                         QList< int > &sectionList
                         )
{
    Q_ASSERT( NULL != pModel );

    H2MotionGroup *pGroup;
    pGroup = (H2MotionGroup*)pModel;

    //! build curve
    QList<H2KeyPoint> curve;
    H2KeyPoint keyP;
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

    //! verify
    ret = verifyTrace( curve );
    if ( ret != 0 )
    { return ret; }

    //! build
    ret = buildTrace( curve, tpvGroups, sectionList );
    if ( ret != 0 )
    { return ret; }

    return 0;
}
