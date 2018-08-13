#include "devicemrv.h"
#include "tpdownloader.h"
#include "../../com/comassist.h"
namespace MegaDevice
{

void deviceMRV::acquireDownloader()
{ mDownloaderSema.acquire(); }
void deviceMRV::releaseDownloader()
{ mDownloaderSema.release(); }

bool deviceMRV::isDownloading( int ax )
{
    return mDownloaders.at(ax)->isRunning();
}

int deviceMRV::tpWrite( QList<TpRow*> &list, int ax )
{   
    //! to idle
    assignStatus( MRV_MOTION_STATE_1_IDLE, tpvRegion(ax,0) );

    mDownloaders.at( ax )->append( list );

    if ( mDownloaders.at(ax)->isRunning() )
    { sysError( QObject::tr("Can not download") ); }
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
    checked_call( setMOTION_STATEREPORT( ax, MRV_MOTION_STATEREPORT_QUERY ) );
    checked_call( setMOTION_SWITCH( ax, MRV_MOTION_SWITCH_RESET, MRV_MOTION_SWITCH_1_MAIN ) );

//    checked_call( setPVT_HOLD( ax, MRV_PVT_HOLD_TRUE ) );
//    checked_call( setPVT_SAVE( ax ) );

    checked_call( setPVT_RESET( ax) );

    //! set cycle
    checked_call( setPVT_CYCLES( ax, mPVT_CYCLES[ax]) );

    checked_call( setPVT_START( ax ) );

    return 0;
}
#define p_unit  (0.1f)
#define t_unit  (0.001f)

#define valve_on    1000
#define valve_off   0
int deviceMRV::tpSend( TpRow *row, int ax )
{
    Q_ASSERT( NULL != row );
    Q_ASSERT( ax >= 0 && ax < axes() );

    int ret;

    checked_call( setPVT_TDATA( mTpIndexes[ax], (row->mTL) ) );
    checked_call( setPVT_PDATA( mTpIndexes[ax], alignP( ax, row->mP ) ) );

//    uint32 val;
//    float fVal;
//    getPVT_PDATA( mTpIndexes[ax], &val );
//    checked_call( setPVT_TDATA( mTpIndexes[ax], (row->mT) ) );
//    getPVT_TDATA( mTpIndexes[ax], &fVal );

    mTpIndexes[ ax ]++;

    return ret;
}
int deviceMRV::tpEndSend( int ax, TpRow &preRow )
{
    int ret;

    //! check the last dot
    do
    {
        if ( mPVT_EXECMODE[ax] == MRV_PVT_EXECMODE_NCYCLE )
        {
            //! loop
            if ( mPVT_CYCLES[ax] > 1 )
            { break; }
            //! single
            else
            {
                //! last data
                preRow.mTL = 0;
            }
        }
        else
        {
            //! last data
            preRow.mTL = -1;
        }

        checked_call( tpSend( &preRow, ax ) );

    }while( 0 );

    checked_call( setPVT_END( ax ) );

    return 0;
}

quint32 deviceMRV::alignP( int ax, tpvType val )
{
    if ( mVALVECTRL_DEVICE[ax] == MRV_VALVECTRL_DEVICE_VALVE )
    {
        if ( val > 0 )
        { return valve_on;}
        else
        { return valve_off; }
    }
    else
    { return comAssist::align( val,p_unit); }
}

//! only write no read
int deviceMRV::requestMotionState( int ax, int page )
{
    int ret;

    ret = m_pBus->write( DEVICE_RECEIVE_ID,
                         MRV_mc_MOTION,
                         MRV_sc_MOTION_STATE_Q,
                         (byte)ax,
                         (byte)page );

    return ret;
}

}
