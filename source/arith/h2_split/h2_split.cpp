#include "h2_split.h"
#include "../../../include/mcdef.h"
#include <float.h>
#include "../../../include/mydebug.h"
namespace h2_split {

#include "./arith/functions.cpp"

int _h2Split( const QList<double> &armLengthes,
              const QList<int> &gantry,
              const QList<double> &zeroXy,
              QList< endPoint *> & points,
              QList< tpvList*> &splitDataset )
{
    if ( points.size() < 2 )
    { return -1; }

    //! cache the data
    xxxGroup< double > ts, xys, vs;

    //! \note for z reserved
    if ( 0 != xys.alloc( points.size() * 3 ) )
    { return -1; }
    if ( 0 != vs.alloc( points.size() * 3 ) )
    { return -1; }
    if ( 0 != ts.alloc( points.size() ) )
    { return -1; }

    //! export data
    for ( int i = 0; i < points.size(); i++ )
    {
        ts.data()[i] = points.at(i)->t;
        xys.data()[i*3+0] = points.at(i)->x;
        xys.data()[i*3+1] = points.at(i)->y;
        xys.data()[i*3+2] = 0;                      //! \note z

        vs.data()[i*3+0] = points.at(i)->vx;        //! \note for xv,yv
        vs.data()[i*3+1] = points.at(i)->vy;
        vs.data()[i*3+2] = 0;                       //! \note z
    }

    Q_ASSERT( armLengthes.size() == 4 );
    double armLength[]={ armLengthes.at(0),
                         armLengthes.at(1),
                         armLengthes.at(2),
                         armLengthes.at(3),
                       };
logDbg()<<armLengthes;
logDbg()<<gantry;
    Q_ASSERT( gantry.size() == 4 );
    int inputParam[]=
    {
        gantry.at(0),
        gantry.at(1),
        gantry.at(2),
        1,
        gantry.at(3),
    };

    Q_ASSERT( zeroXy.size() == 2 );
    double P0[] = { zeroXy.at(0), zeroXy.at(1) };

    //! alloc for slove
    xxxGroup< SlovePoint > slovePoints;
    if ( 0 != slovePoints.alloc( points.size()) )
    { return -1; }

    int ret;
    ret = SolveStructureKinematics( armLength, inputParam, P0,
                                    xys.data(), vs.data(), ts.data(), points.size(),
                                    slovePoints.data()->datas
                                      );
    if ( ret != 0 )
    { logDbg()<<ret;return -2; }

    //! split
    tpvRow *pRow;
    tpvList *pColumn;

    //! for each column
    for ( int i = 0; i < 2; i++ )
    {
        pColumn = new tpvList();
        if ( NULL == pColumn )
        { return -3; }

        //! for each data
        for ( int j = 0; j < points.size(); j++ )
        {
            pRow = new tpvRow( );
            if ( NULL == pRow )
            { return -1; }

            pRow->mT = slovePoints.data()[j].t;
            pRow->mP = slovePoints.data()[j].Pxy[i];
            pRow->mV = slovePoints.data()[j].Vxy[i];

            pColumn->append( pRow );
        }

        splitDataset.append( pColumn );
    }

    return 0;
}

//! split into 2 axes
int h2Split(const QList<double> &armLengthes,
            const QList<int> &gantry,
            const QList<double> &zeroXy,
                    QList< endPoint *> &points,
                    QList< tpvList*> &splitDataset )
{
    if ( points.size() < 2 )
    { return -1; }

    //! split
    int ret = _h2Split( armLengthes, gantry, zeroXy, points, splitDataset );
    //! fail to gc
    if ( ret != 0 )
    {
        h2Gc( splitDataset );
        return ret;
    }

    return 0;
}

int h2Split(const QList<double> &armLengthes,
            const QList<int> &gantry,
            const QList<double> &zeroXy,
            endPoint * pPoints,
            int n,
            QList< tpvList*> &splitDataset )
{
    QList< endPoint *> points;

    for ( int i = 0; i < n; i++ )
    {
        points.append( pPoints + i );
    }

    return h2Split( armLengthes, gantry, zeroXy, points, splitDataset );
}

int h2Gc( QList< tpvList*> &splitDataset )
{
    //! delete the list
    foreach( tpvList *pList, splitDataset )
    {
        Q_ASSERT( NULL != pList );
        qDeleteAll( *pList );
    }

    qDeleteAll( splitDataset );
    return 0;
}

//! to pose by angle
int h2Pose( const QList<double> &armLengthes,
            const QList<int> &gantry,
            const QList<double> &zeroXy,
            float angleL, float angleR,
            float &x, float &y )
{
    //! paras
    Q_ASSERT( armLengthes.size() == 4 );
    double armLength[]={ armLengthes.at(0),
                         armLengthes.at(1),
                         armLengthes.at(2),
                         armLengthes.at(3),

                       };
    Q_ASSERT( gantry.size() == 4 );
    int inputParam[]=
    {
        gantry.at(0),
        gantry.at(1),
        gantry.at(2),
        0,
        gantry.at(3),
    };

    Q_ASSERT( zeroXy.size() == 2 );
    double P0[] = { zeroXy.at(0), zeroXy.at(1) };

    //! input
    int ret;
    double xys[3];      //! x, y, t

    double pIn[2];
    double vIn[2] = {0.0};
    double tIn[1] = {0.0};

    pIn[0] = angleL;
    pIn[1] = angleR;

    ret = SolveStructureKinematics( armLength, inputParam, P0,
                                    pIn, vIn, tIn,
                                    0,                  //! angle->x,y
                                    xys
                                      );

    x = double_to_float( xys[0] );
    y = double_to_float( xys[1] );

    return ret;
}

}
