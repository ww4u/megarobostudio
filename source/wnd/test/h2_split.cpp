
#include <QApplication>
#include <QDebug>

#include "../../include/mcstd.h"

//#include "../../robot/megatron/megatron.h"
#include "../../arith/h2_split/h2_split.h"

int main(int argc, char *argv[] )
{
    QApplication app( argc, argv );

    QList<double> armLengthes;
    armLengthes<<13.4<<13.4<<802<<494<<52<<38;
    QList<double> xyZeros;
    xyZeros<<32.7<<0;

    h2_split::endPoint points[]=
    {
        { 0,247,-380,0, },
        { 1,267,-380,0 },

//        { 0,0,0,0, },
//        { 1,20,20,0 },
//        { 0,42,0,0 },
//        { 1,0,0,0 },
//        { 2,42,0,0 },
//        { 3,20,0,0 },
//        { 4,20,20,0 },

//        { 2,0+32.7,0,0 },
//        { 3,0+32.7,50,0 },
//        { 4,0+60,50,0 },
    };

    int ret;
    QList< tpvList*> splitDataset;
    ret = h2_split::h2Split( armLengthes,
                xyZeros,
                points,
                sizeof_array( points ),
                splitDataset );
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
    h2_split::h2Gc( splitDataset );

    float x, y;
    ret = h2_split::h2Pose( armLengthes, xyZeros, 0,0, x,y );
    logDbg ()<<ret;
    if ( ret == 0 )
    {
        logDbg()<<x<<y;
    }

    return 0;
}
