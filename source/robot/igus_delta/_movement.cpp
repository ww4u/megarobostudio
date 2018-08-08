#include "igus_delta.h"
#include "../../com/comassist.h"
//! load the data from the file
int robotIgusDelta::program( const QString &fileName,
                         const tpvRegion &region )
{
    //! 1.load
    int ret;
    ret = loadProgram( fileName );
    if ( ret != 0 )
    { return ret; }

    //! 2.config
    setLoop( 1 );

    //! 3.download
    ret = downloadTrace( region );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

//! \todo for each axis
int robotIgusDelta::loadProgram( const QString &fileName )
{
    //!  0  1  2   3  4  5  6   7
    //! LX LXV RX RXV Y YV END TIME
    QList<float> dataset;
    int col = 8;
    QList<int> dataCols;
    dataCols<<0<<1<<2<<3<<4<<5<<6<<7;
    if ( 0 != comAssist::loadDataset( fileName, col, dataCols, dataset ) )
    { return ERR_INVALID_INPUT; }logDbg()<<dataset.size();

    //! convert to tpvitem
    if ( dataset.size() < 2 * col )
    { return ERR_INVALID_INPUT; }logDbg();

    //! file split
    delete_all( mJointsGroup );logDbg();

    //! for 3 axis
    for ( int i = 0; i < 3; i++ )
    {
        tpvGroup *pGroup = new tpvGroup();
        if ( NULL == pGroup )
        { return -1; }logDbg();

        //! for each time
        int ret;
        for ( int j = 0; j < dataset.size() / col; j++ )
        {
            ret = pGroup->addItem( dataset.at( j * col + 7),            //! time
                                   dataset.at( j * col + i * 2),        //! p
                                   dataset.at( j * col + i * 2 + 1)     //! v
                                   );
            if ( ret != 0 )
            {
                delete pGroup;
                return ERR_FAIL_ADD_TPV;
            }
        }

        mJointsGroup.append( pGroup );
    }

    //! end
    {
        tpvGroup *pGroup = new tpvGroup();
        if ( NULL == pGroup )
        { return -1; }

        //! for each time
        int ret;
        for ( int j = 0; j < dataset.size() / col; j++ )
        {
            ret = pGroup->addItem( dataset.at( j * col + 7),        //! time
                                   dataset.at( j * col + 6),        //! p
                                   0
                                   );
            if ( ret != 0 )
            {
                delete pGroup;
                return ERR_FAIL_ADD_TPV;
            }
        }

        mJointsGroup.append( pGroup );
    }

        //! log joint group
        foreach ( tpvGroup *pGp, mJointsGroup )
        {
            logDbg()<<"*******";
            foreach(  tpvItem *pItem, pGp->mItems )
            {
                logDbg()<<pItem->mT<<pItem->mP<<pItem->mV;
            }
        }

    return 0;
}

int robotIgusDelta::program( QList<TraceKeyPoint> &curve,
             const tpvRegion &region )
{
    int ret;

    //! 0. check
    //!

    //! 1.build
    QList<arith_delta::deltaPoint> jointsPlan;
    ret = buildTrace( curve, jointsPlan );
    if ( ret != 0 )
    { return ret; }

    //! 2.convert
    QList<int> secList;
    ret = convertTrace( curve, jointsPlan, mJointsGroup, secList );
    if ( ret != 0 )
    { return ret; }

    //! 3.config
    setLoop( 1 );

    //! 4.download
    ret = downloadTrace( region );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int robotIgusDelta::move( QList<TraceKeyPoint> &curve,
                        const tpvRegion &region )
{
    int ret;

    //! request run
    run( region );

    ret = preMove( curve, region );

    return ret;
}

int robotIgusDelta::preMove( QList<TraceKeyPoint> &curve,
                        const tpvRegion &region )
{
    int ret;

    setLoop( 1, region );

    //! program
    ret = program( curve, region );

    return ret;
}

int robotIgusDelta::moveTest1( const tpvRegion &region )
{
    TraceKeyPoint pt1,pt2;
    pt1.t = 0;
    pt1.x = 0;
    pt1.y = -230.8074;
    pt1.z = 0;
    pt1.hand = 10;

    pt2.t = 2;
    pt2.x = 113.7092;
    pt2.y = -187.3488;
    pt2.z = 10;
    pt2.hand = 20;

    QList<TraceKeyPoint> curve;
    curve.append( pt1 );
    curve.append( pt2 );
//    return moveTest( pt1, pt2, dt );
// MOVE 280,21.5,452.75,0,250,0,502,90,1
// MOVE 250,0,502,90,280,21.5,452.75,0,1
    return move( curve, region );
}

int robotIgusDelta::moveTest2( const tpvRegion &region )
{
    TraceKeyPoint pt1,pt2;
    pt1.t = 2;
    pt1.x = 0;
    pt1.y = -230.8074;
    pt1.z = 0;
    pt1.hand = 10;

    pt2.t = 0;
    pt2.x = 113.7092;
    pt2.y = -187.3488;
    pt2.z = 10;
    pt2.hand = 20;

    //! p2 -> p1
    QList<TraceKeyPoint> curve;
    curve.append( pt2 );
    curve.append( pt1 );

    return move( curve, region );
}
