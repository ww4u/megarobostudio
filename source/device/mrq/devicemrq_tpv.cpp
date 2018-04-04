#include "deviceMRQ.h"

#ifdef DEVICE_EMIT_SIGNAL
#include "../../app/dpcobj.h"
#endif

namespace MegaDevice
{

//! load only one time
int deviceMRQ::loadTpvCap( )
{
    int ret;
    tpvRegion region;

    //! read again
    uint16 cap;
    foreach_page()
    {
        ret = getMOTIONPLAN_REMAINPOINT( _i, _axPage, &cap );
        if ( ret != 0 )
        { return ret; }

        region.setRegion( _i, _j );
        Q_ASSERT( mTpvCaps.contains(region) );

        mTpvCaps[ tpvRegion(_i,_j) ] = cap;
    }
    end_foreach_page()

//    foreach_page()
//    {
//        logDbg()<<mTpvCaps[ tpvRegion(_i,_j) ];
//    }
//    end_foreach_page()

    return 0;
}

int deviceMRQ::getTpvCap( const tpvRegion &region )
{
    Q_ASSERT( mTpvCaps.contains(region) );
    return mTpvCaps[ region ];
}

int deviceMRQ::beginTpvDownload( const tpvRegion &region )
{
    int ret;

    DELOAD_REGION();

    //! \errant exec mode to cycle
    checked_call( setMOTIONPLAN_EXECUTEMODE( pvt_page_p,
                                             MRQ_MOTIONPLAN_EXECUTEMODE_1_CYCLE) );

    checked_call( setMOTIONPLAN_PVTCONFIG( pvt_page_p, MRQ_MOTIONPLAN_PVTCONFIG_1_CLEAR ) );
    checked_call( getMOTIONPLAN_REMAINPOINT( pvt_page_p, mMOTIONPLAN_REMAINPOINT[ax]+page ) );

    checked_call( setMOTION_STATEREPORT( ax, MRQ_MOTION_STATEREPORT_QUERY ) );

    setTpvIndex( pvt_region_p, 0 );

    return ret;
}
int deviceMRQ::tpvDownload(
                 const tpvRegion &region,
                 int index,
                 f32 t,
                 f32 p,
                 f32 v )
{
    int ret;

    DELOAD_REGION();

    checked_call( setPOSITION( pvt_page_p, index, p * _mPBase ) );
    checked_call( setVELOCITY( pvt_page_p, index, v * _mVBase ) );
    checked_call( setTIME( pvt_page_p, index, t * _mTBase ) );

//logWarning()<<axesId<<index<<p<<v<<t;
    return ret;
}
int deviceMRQ::tpvDownload( const tpvRegion &region,
                 QList<tpvRow *> &list,
                 int from,
                 int len )
{
    int ret = 0;

    DELOAD_REGION();

    int id = 0;
    for( int i = 0; i < mMOTIONPLAN_REMAINPOINT[ax][page] && i < len && id < list.size(); i++ )
    {
        id = i + from;
        checked_call( tpvDownload( region,
                                   i,
                                   list[id]->mT,
                                   list[id]->mP,
                                   list[id]->mV
                                   )
                      );

        #ifdef DEVICE_EMIT_SIGNAL
        dpcObj::instance()->tlsProgress( i, 0, len );
        logDbg()<<i;
        #endif
    }

    return ret;
}

int deviceMRQ::tpvDownload( pvt_region,
                            tpvRow *pItem )
{
    Q_ASSERT( NULL != pItem );
    int ret;

    checked_call( tpvDownload( pvt_region_p,
                               getTpvIndex( pvt_region_p ),
                               pItem->mT,
                               pItem->mP,
                               pItem->mV
                               )
                  );

    accTpvIndex( pvt_region_p );

    return ret;
}

int deviceMRQ::endTpvDownload( pvt_region )
{
    int ret;

    DELOAD_REGION();

    checked_call( setMOTIONPLAN_PVTCONFIG( pvt_page_p, MRQ_MOTIONPLAN_PVTCONFIG_1_END ) );

    return ret;
}

//! \todo not send end
int deviceMRQ::tpvDownloadMission( pvt_region,
                                   QList<tpvRow *> &list,
                                   int from,
                                   int len )
{
    int ret;

    checked_call( beginTpvDownload( pvt_region_p ) );

    ret = tpvDownload( pvt_region_p, list, from, len );

    checked_call( endTpvDownload( pvt_region_p ) );

    return ret;
}

int deviceMRQ::pvtWrite( pvt_region,
              QList<tpvRow *> &list,
              int from,
              int len )
{logDbg()<<region.axes()<<region.page()<<list.size()<<name();

    tpvDownloader *pLoader = downloader( region );
    Q_ASSERT( NULL != pLoader );

    if ( pLoader->isRunning() )
    {
        sysError( QObject::tr("Busy now, can not downloading") );
        sysError( QString::number(region.axes()), QString::number(region.page()) );
        return ERR_CAN_NOT_RUN;
    }
    else
    {
        pLoader->append( list, from, len );
        pLoader->setRegion( region );
        pLoader->start();
    }

    return 0;
}

int deviceMRQ::pvtWrite( pvt_region,
              float t1, float p1,
              float t2, float p2,
              float endV
              )
{
    DELOAD_REGION();

    logDbg()<<t1<<p1<<t2<<p2;
    //! point 1
    tpvRow *pRow1 = new tpvRow();
    if ( NULL == pRow1 )
    { return ERR_ALLOC_FAIL; }

    pRow1->setGc( true );
    pRow1->mT = t1;
    pRow1->mP = p1;
    pRow1->mV = 0;

    //! point 2
    tpvRow *pRow2 = new tpvRow();
    if ( NULL == pRow2 )
    {
        delete pRow1;
        return ERR_ALLOC_FAIL;
    }

    pRow2->setGc( true );
    pRow2->mT = t2;
    pRow2->mP = p2;
    pRow2->mV = endV;

    QList< tpvRow *> rotList;
    rotList.append( pRow1 );
    rotList.append( pRow2 );

    return pvtWrite( pvt_region_p, rotList );
}

int deviceMRQ::pvtWrite( pvt_region,
                         float dT,
                         float dAngle,
                         float endV )
{
    return pvtWrite( pvt_region_p, 0, 0, dT, dAngle, endV );
}

int deviceMRQ::pvtWrite( pvt_region,
              tpvRow *pRows,
              int n
              )
{
    Q_ASSERT( NULL != pRows );

    QList< tpvRow *> transRows;
    tpvRow *pRow;
    for ( int i = 0; i < n; i++ )
    {
        pRow = new tpvRow();
        if ( NULL == pRow )      //! new fail
        {
            delete_all(transRows);
            return -1;
        }

        //! copy
        *pRow = pRows[i];
        pRow->setGc( true );

        transRows.append( pRow );
    }

    return pvtWrite( pvt_region_p, transRows );
}


void deviceMRQ::setTpvIndex( pvt_region, int index )
{
    DELOAD_REGION();

    Q_ASSERT( mTpvIndexes.contains(region) );

    mTpvIndexes[ region ] = index;
}
int  deviceMRQ::getTpvIndex( pvt_region )
{
    DELOAD_REGION();

    Q_ASSERT( mTpvIndexes.contains(region) );

    return mTpvIndexes[ region ];
}
void deviceMRQ::accTpvIndex( pvt_region )
{
    DELOAD_REGION();

    Q_ASSERT( mTpvIndexes.contains(region) );

    //! turn around
    mTpvIndexes[ region ] = mTpvIndexes[ region ] + 1;
    if ( mTpvIndexes[ region ] >= mTpvCaps[region] )
    {
        mTpvIndexes[ region ] = 0;
    }
    else
    {}
}



}
