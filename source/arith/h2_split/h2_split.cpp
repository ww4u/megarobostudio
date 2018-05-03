#include "h2_split.h"


namespace h2_split {

#include "./arith/functions.cpp"

//! no memory check
int _h2Split(
                    QList< endPoint *> & points,
                    QList< tpvList*> &splitDataset )
{
    if ( points.size() < 2 )
    { return -1; }

//    int ret = 0;

    //! create 3 list
    tpvList *pList;
    for ( int i = 0; i < 3; i++ )
    {
        pList = new tpvList();
        if ( NULL == pList )
        { return -1; }

        splitDataset.append( pList );
    }

    tpvRow *pRow;
    foreach( endPoint *pPoint, points )
    {
        Q_ASSERT( NULL != pPoint );

        //! foreach row
        for ( int i = 1; i <= 3; i++ )
        {
            pRow = new tpvRow( );
            if ( NULL == pRow )
            { return -1; }

            pRow->mT = pPoint->datas[0];    //! time
            pRow->mP = pPoint->datas[i];    //! phase

            //! append data
            splitDataset.at(i-1)->append( pRow );
        }
    }

    return 0;
}

//! split into 3 axes
int h2Split(
                    QList< endPoint *> &points,
                    QList< tpvList*> &splitDataset )
{
    if ( points.size() < 2 )
    { return -1; }

    //! split
    int ret = _h2Split( points, splitDataset );
    //! fail to gc
    if ( ret != 0 )
    {
        h2Gc( splitDataset );
        return ret;
    }

    return 0;
}

int h2Split(
                    endPoint * pPoints,
                    int n,
                    QList< tpvList*> &splitDataset )
{
    QList< endPoint *> points;

    for ( int i = 0; i < n; i++ )
    {
        points.append( pPoints + i );
    }

    return h2Split( points, splitDataset );
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

}
