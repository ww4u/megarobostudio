#include "igus_delta.h"
#include "../../com/comassist.h"
#include "../../arith/arith_igus/arith_igus.h"

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

    return 0;
}

int robotIgusDelta::program( QList<TraceKeyPoint> &curve,
             const tpvRegion &region )
{
    int ret;

    //! 0. check
    //!

    //! 1.build
    QList<deltaPoint> jointsPlan;
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

int robotIgusDelta::pose( float xyz[3] )
{
    //! get angle
    float angles[3];

    MegaDevice::deviceMRQ *pMrq;
    int ax,dir;

    dir = mInvert ? 1 : -1;

    for ( int i = 0; i < 3; i++ )
    {
        pMrq = jointDevice( i, &ax );
        if ( NULL == pMrq )
        { return -1; }

        angles[i] = pMrq->getIncAngle( ax );

        //! convert the angles
        angles[ i ] *= ( mAngleToDist * dir );
    }

    //! cfg
    //! config para
    arith_igus::igusConfig cfg;
    for ( int i = 0; i < 4; i++ )
    { cfg.armLength[i] = mArmLengths[i]; }
    for ( int i = 0; i < 2; i++ )
    { cfg.offset[i] = mOffset[i]; }
    for ( int i = 0; i < 3; i++ )
    { cfg.P0[i] = mP0[i]; }
    for ( int i = 0; i < 2; i++ )
    { cfg.posLim[i] = mPosLim[i]; }
    cfg.scal = mScal;
    cfg.vM = mVm;

    //! cw
    int ret;
    ret = arith_igus::cwSlove( cfg, angles, xyz );
    if ( ret != 0 )
    { return ret; }

    return 0;
}
