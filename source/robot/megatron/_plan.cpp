#include "megatron.h"


int robotMegatron::buildTrace( QList<MegatronKeyPoint> &curve,
                               QList<tpvGroup *> &jointGroup,
                               QList<int> &secList )
{
    megatron_split::endPoint *pPoints = new megatron_split::endPoint[ curve.size() ];
    if ( NULL == pPoints )
    { return -1; }

    //! move data
    for ( int i = 0; i < curve.size(); i++ )
    {
        memcpy( pPoints[i].datas,
                curve.at(i).datas,
                7 * sizeof(float)
                );
    }

    //! split
    QList< tpvList*> splitDataSet;
    int ret;
    ret = megatron_split::megatronSplit( pPoints, curve.size(), splitDataSet );
    delete []pPoints;

    if ( ret != 0 )
    {
        megatron_split::megatronGc( splitDataSet );
        return ret;
    }

    //! to tpv group
    delete_all( jointGroup );
    ret = 0;
    tpvGroup *pGroup;
    for ( int i = 0; i < splitDataSet.size(); i++ )
    {
        pGroup = new tpvGroup();
        if ( NULL == pGroup )
        {
            ret = -1;
            break;
        }

        jointGroup.append( pGroup );

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

            logDbg()<<i<<splitDataSet.at(i)->at(j)->mT<<splitDataSet.at(i)->at(j)->mP;
        }
    }

    //! clean
    megatron_split::megatronGc( splitDataSet );

    if ( ret != 0 )
    {
        delete_all( jointGroup );
        return ret;
    }

    //! add section
    secList<<0<<6;

    return 0;
}

