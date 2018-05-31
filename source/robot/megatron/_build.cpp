#include "megatron.h"
#include "../../model/megatronmotiongroup.h"

int robotMegatron::build( MegaTableModel *pModel,
                         xxxGroup<tracePoint> &tracePlan,
                         xxxGroup<jointsTrace> &jointsPlan,
                         QList< tpvGroup *> &tpvGroups,
                         QList< int > &sectionList
                         )
{
    Q_ASSERT( NULL != pModel );

    MegatronMotionGroup *pGroup;
    pGroup = (MegatronMotionGroup*)pModel;

    //! build curve
    QList<MegatronKeyPoint> curve;
    MegatronKeyPoint keyP;
    for (int i = 0; i < pGroup->mItems.size(); i++ )
    {
        if ( pGroup->mItems.at(i)->enable() )
        {
            keyP.t = pGroup->mItems.at(i)->T();
            keyP.fx = pGroup->mItems.at(i)->Fx();
            keyP.ly = pGroup->mItems.at(i)->Ly();
            keyP.fz = pGroup->mItems.at(i)->Fz();

            keyP.bx = pGroup->mItems.at(i)->Bx();
            keyP.ry = pGroup->mItems.at(i)->Ry();
            keyP.bz = pGroup->mItems.at(i)->Bz();

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
