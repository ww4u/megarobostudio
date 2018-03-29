
#include <QApplication>
#include <QDebug>

#include "../../include/mcstd.h"

//#include "../../robot/megatron/megatron.h"
#include "../../arith/megatron_split/megatron_split.h"

int main(int argc, char *argv[] )
{
    QApplication app( argc, argv );


    megatron_split::endPoint points[]=
    {
        { 0,1,2,3,4,5,0},
        { 2,3,4,5,6,7,1},
        { 3,4,5,6,7,8,2},
    };

    QList< tpvList*> splitDataset;
    int ret;
    ret = megatron_split::megatronSplit( points, 3, splitDataset );
    if ( ret == 0 )
    {
        for ( int i = 0; i < splitDataset.size(); i++ )
        {
            logDbg()<<"********";
            foreach (tpvRow *pRow, *splitDataset.at(i))
            {
                Q_ASSERT( NULL != pRow );
                logDbg()<<pRow->mT<<pRow->mP<<pRow->mV;
            }
        }
    }
    megatron_split::megatronGc( splitDataset );

    return 0;
}
