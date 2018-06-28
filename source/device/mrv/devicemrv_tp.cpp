#include "devicemrv.h"
#include "tpdownloader.h"

namespace MegaDevice
{

void deviceMRV::acquireDownloader()
{ mDownloaderSema.acquire(); }
void deviceMRV::releaseDownloader()
{ mDownloaderSema.release(); }

int deviceMRV::tpWrite( QList<TpRow*> &list, int ax )
{
    mDownloaders.at( ax )->append( list );

    if ( mDownloaders.at(ax)->isRunning() )
    {}
    else
    { mDownloaders.at(ax)->start(); }

    return 0;
}
int deviceMRV::tpWrite( QList<QPointF> &points, int ax )  //! x: t, y : p
{
    QList<TpRow*> rows;
    TpRow *pRow;
    foreach( QPointF pt, points )
    {
        pRow = new TpRow();
        Q_ASSERT( NULL != pRow );

        pRow->setGc( true );
        pRow->mT = (tpvType)pt.x();
        pRow->mP = (tpvType)pt.y();

        rows.append( pRow );
    }

    tpWrite( rows, ax );

    return 0;
}

//! send
int deviceMRV::tpBeginSend( int ax )
{
    Q_ASSERT( ax >= 0 && ax < axes() );

    mTpIndexes[ ax ] = 0;

    int ret;
    checked_call( setPVT_START( ax ) );

    return 0;
}
int deviceMRV::tpSend( TpRow *row, int ax )
{
    Q_ASSERT( NULL != row );
    Q_ASSERT( ax >= 0 && ax < axes() );

    int ret;
    //! \todo t,p format
    checked_call( setPVT_PDATA( mTpIndexes[ax], row->mP ) );

    checked_call( setPVT_TDATA( mTpIndexes[ax], row->mT ) );

    mTpIndexes[ ax ]++;

    return ret;
}
int deviceMRV::tpEndSend( int ax )
{
    int ret;

    checked_call( setPVT_END( ax ) );

    return 0;
}

}
