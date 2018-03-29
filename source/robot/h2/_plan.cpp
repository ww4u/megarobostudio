#include "h2.h"
#include "../../arith/h2_split/h2_split.h"
int robotH2::buildTrace( QList<H2KeyPoint> &curve
                )
{
    h2_split::endPoint *pPoints = new h2_split::endPoint[ curve.size() ];
    if ( NULL == pPoints )
    { logDbg()<<curve.size(); return -1; }

    //! move data
    for ( int i = 0; i < curve.size(); i++ )
    {
        memcpy( pPoints[i].datas,
                curve.at(i).datas,
                4 * sizeof(float)
                );
    }

    //! split
    QList< tpvList*> splitDataSet;
    int ret;
    ret = h2_split::h2Split( pPoints, curve.size(), splitDataSet );
    delete []pPoints;

    if ( ret != 0 )
    {
        h2_split::h2Gc( splitDataSet );
        return ret;
    }

    //! to tpv group
    delete_all( mJointsGroup );
    ret = 0;
    tpvGroup *pGroup;
    for ( int i = 0; i < splitDataSet.size(); i++ )
    {
        pGroup = new tpvGroup();
        if ( NULL == pGroup )
        {logDbg();
            ret = -1;
            break;
        }

        mJointsGroup.append( pGroup );

        //! for each item
        for ( int j = 0; j < splitDataSet.at(i)->size(); j++ )
        {
            ret = pGroup->addItem(
                                splitDataSet.at(i)->at(j)->mT,
                                splitDataSet.at(i)->at(j)->mP,
                                splitDataSet.at(i)->at(j)->mV
                        );
            if ( ret != 0 )
            { break; }
        }
    }

    //! clean
    h2_split::h2Gc( splitDataSet );

    if ( ret != 0 )
    {
        delete_all( mJointsGroup );
        return ret;
    }

    return 0;
}
