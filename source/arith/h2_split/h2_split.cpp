#include "h2_split.h"


namespace h2_split {

#include "./arith/functions.cpp"

//! no memory check
int _h2Split( const QList<double> &armLengthes,
              const QList<double> &zeroXy,
              QList< endPoint *> & points,
              QList< tpvList*> &splitDataset )
{
    if ( points.size() < 2 )
    { return -1; }

    //! cache the data
    xxxGroup< double > ts, xys, vs;

    if ( 0 != xys.alloc( points.size() * 2 ) )
    { return -1; }
    if ( 0 != vs.alloc( points.size() * 2 ) )
    { return -1; }
    if ( 0 != ts.alloc( points.size() ) )
    { return -1; }

    //! export data
    for ( int i = 0; i < points.size(); i++ )
    {
        ts.data()[i] = points.at(i)->t;
        xys.data()[i*2] = points.at(i)->x;
        xys.data()[i*2+1] = points.at(i)->y;

        vs.data()[i*2] = points.at(i)->vx;       //! \note for xv,yv
        vs.data()[i*2+1] = points.at(i)->vy;
    }

    Q_ASSERT( armLengthes.size() == 6 );
    double armLength[]={ armLengthes.at(0),
                         armLengthes.at(1),
                         armLengthes.at(2),
                         armLengthes.at(3),
                         armLengthes.at(4),
                         armLengthes.at(5),
                       };
    double armLim[]={ -2 * PI, 2 * PI, -2 * PI, 2 * PI };
    Q_ASSERT( zeroXy.size() == 2 );
    double P0[] = { zeroXy.at(0), zeroXy.at(1) };

    //! alloc for slove
    xxxGroup< SlovePoint > slovePoints;
    if ( 0 != slovePoints.alloc( points.size()) )
    { return -1; }

    int ret;
    ret = SolveStructureKinematics( armLength, armLim, P0,
                                    xys.data(), vs.data(), ts.data(), points.size(),
                                    1,
                                    slovePoints.data()->datas
                                      );
    if ( ret != 0 )
    { return -2; }

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
            const QList<double> &zeroXy,
                    QList< endPoint *> &points,
                    QList< tpvList*> &splitDataset )
{
    if ( points.size() < 2 )
    { return -1; }

    //! split
    int ret = _h2Split( armLengthes, zeroXy, points, splitDataset );
    //! fail to gc
    if ( ret != 0 )
    {
        h2Gc( splitDataset );
        return ret;
    }

    return 0;
}

int h2Split(const QList<double> &armLengthes,
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

    return h2Split( armLengthes, zeroXy, points, splitDataset );
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
            const QList<double> &zeroXy,
            float angleL, float angleR,
            float &x, float &y )
{
    //! paras
    Q_ASSERT( armLengthes.size() == 6 );
    double armLength[]={ armLengthes.at(0),
                         armLengthes.at(1),
                         armLengthes.at(2),
                         armLengthes.at(3),
                         armLengthes.at(4),
                         armLengthes.at(5),
                       };
    double armLim[]={ -2 * PI, 2 * PI, -2 * PI, 2 * PI };
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

    ret = SolveStructureKinematics( armLength, armLim, P0,
                                    pIn, vIn, tIn,
                                    1,
                                    0,                  //! angle->x,y
                                    xys
                                      );

    x = xys[0];
    y = xys[1];

    return ret;
}

}
