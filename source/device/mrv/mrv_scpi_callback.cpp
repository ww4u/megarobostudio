#include "scpi/scpi.h"

#include "devicemrv.h"
//#include "scpi_obj.h"
#include "../_scpi_xxx_device.h"

//! context to obj
#define SET_OBJ( context )	((MegaDevice::deviceMRV*)context->user_context)
#define GET_OBJ( context )      ((MegaDevice::deviceMRV*)context->user_context)

#include "../mrv_board/_MRV_scpi_callback.cpp"

#include "../../com/comassist.h"
#include "../../com/scpiassist.h"
#define DEF_MRV()   MegaDevice::deviceMRV* _localMrv = (GET_OBJ(context));
#define LOCALMRV()  _localMrv

#define CHECK_LINK( ax, page )

static scpi_result_t _scpi_idn( scpi_t * context )
{
    DEF_LOCAL_VAR();

    DEF_MRV();

    QString str;
    str = LOCALMRV()->getModel()->getSN();

    SCPI_ResultText( context, str.toLatin1().data() );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_qVersion( scpi_t * context )
{
    DEF_LOCAL_VAR();

    DEF_MRV();

    QString str;
    str = LOCALMRV()->getModel()->getSwVer();

    SCPI_ResultText( context, str.toLatin1().data() );

    return SCPI_RES_OK;
}

//! lrn setup.stp
static scpi_result_t _scpi_lrn( scpi_t * context )
{
    DEF_LOCAL_VAR();

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;
    if (strLen < 1)
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRV();
    QByteArray rawFileName( pLocalStr, strLen );
    QString fileName( rawFileName );
    if ( comAssist::ammendFileName( fileName ) )
    {}
    else
    { scpi_ret( SCPI_RES_ERR ); }

    Q_ASSERT( LOCALMRV() != NULL );
    int ret = LOCALMRV()->getModel()->load( fileName );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    ret = LOCALMRV()->applySetting();
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    sysLog( QObject::tr("Setting Down") );

    return SCPI_RES_OK;
}

//! int ax
static scpi_result_t _scpi_fsmState( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    page = 0;
    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { page=0; }

    DEF_MRV();

    //! if downloading
    if ( LOCALMRV()->isDownloading( ax ) )
    {
        SCPI_ResultInt32( context, MRV_MOTION_STATE_1_reserve1 );

        return SCPI_RES_OK;
    }

    //! real status
    int state = LOCALMRV()->status( tpvRegion(ax) );

    SCPI_ResultInt32( context, state );

    return SCPI_RES_OK;
}

//! int ax
static scpi_result_t _scpi_run( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRV();

    int state = LOCALMRV()->switchRun( ax );

    SCPI_ResultInt32( context, state );

    return SCPI_RES_OK;
}

//! int ax
static scpi_result_t _scpi_stop( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRV();

    int state = LOCALMRV()->switchStop( ax );

    SCPI_ResultInt32( context, state );

    return SCPI_RES_OK;
}

//! ax,page
static scpi_result_t _scpi_sync( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_MRV();

    int ax, page;
    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    LOCALMRV()->requestMotionState( ax, page );

    return SCPI_RES_OK;
}

//! TP ax,page,e:/ddd.csv
static scpi_result_t _scpi_program( scpi_t * context )
{
    DEF_LOCAL_VAR();

    DEF_MRV();

    //! para
    int ax, page;
    QString file;

    if ( deload_ax_page_file( context, ax, page, file) == SCPI_RES_OK )
    {}
    else
    { scpi_ret( SCPI_RES_ERR ); }

    //! data set
    MDataSet dataSet;

    MDataSection *pSec;
    pSec = dataSet.tryLoad( file, "", headerlist("t/p") );

    if ( NULL == pSec )
    { scpi_ret( SCPI_RES_ERR ); }
    else
    {}

    //! timebase
    MegaTableModel::timeType tType = MegaTableModel::time_abs;
    MegaTableModel::toValue( pSec->getAttribute(attr_timebase), tType );
    TimebaseHelp helpT( tType );

    deparse_column_index( enable, "enable" );
    deparse_column_index( t, "t" );
    deparse_column_index( p, "p" );

    //! deload
    QList<QPointF> curve;
    QPointF tp;
    bool bEn;
    qreal v;
    for ( int i = 0; i < pSec->rows(); i++ )
    {
        //! disabled
        if ( pSec->cellValue( i, c_enable, bEn, true, true ) && !bEn )
        { continue; }

        if ( !pSec->cellValue( i, c_t, v, 0, false ) )
        { continue; }
        tp.setX( v );

        if ( !pSec->cellValue( i, c_p, v, 0, false ) )
        { continue; }
        tp.setY( v );

        tp.setX( helpT.accT( curve.size(), tp.x() ) );

        curve.append( tp );
    }

    //! check curve
    if ( curve.size() < 2 )
    { scpi_ret( SCPI_RES_ERR ); }

    CHECK_LINK( ax, page );

    if ( 0 != LOCALMRV()->tpWrite( curve, ax ) )
    { scpi_ret( SCPI_RES_ERR ); }

    return SCPI_RES_OK;

//    DEF_LOCAL_VAR();

//    int ax, page;

//    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
//    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<ax;

//    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
//    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<page;

//    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
//    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;
//    if (strLen < 1)
//    { scpi_ret( SCPI_RES_ERR ); }

//    //! load
//    QList<float> dataSets;
//    int col = 2;
//    QList<int> dataCols;
//    dataCols<<0<<1;
//    if ( 0 != comAssist::loadDataset( pLocalStr, strLen, col, dataCols, dataSets ) )
//    { scpi_ret( SCPI_RES_ERR ); }

//    int dotSize = dataSets.size()/col;
//    if ( (dotSize < 2) )
//    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<dotSize;

//    //! move data
//    QList<QPointF> dots;
//    QPointF dot;
//    for( int i = 0; i < dotSize; i++ )
//    {
//        dot.setX( dataSets.at(i*col+ 0) );
//        dot.setY( dataSets.at(i*col+ 1) );

//        dots.append( dot );
//    }

//    DEF_MRV();

//    CHECK_LINK( ax, page );

//    //! send
//    int ret = -1;
//    ret = LOCALMRV()->tpWrite( dots, ax );

//    if ( ret != 0 )
//    { scpi_ret( SCPI_RES_ERR ); }
//    else
//    { return SCPI_RES_OK; }
}

//! TP ax,page
static scpi_result_t _scpi_call( scpi_t * context )
{
    DEF_LOCAL_VAR();

    DEF_MRV();

    int ax, page;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<ax;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<page;

    LOCALMRV()->switchRun( ax );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_arb_write( scpi_t * context )
{
    DEF_MRV();
    DEF_LOCAL_VAR();

    if ( SCPI_ParamArbitraryBlock(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    QByteArray stream( pLocalStr, strLen );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_arb_read( scpi_t * context )
{
    DEF_MRV();
    DEF_LOCAL_VAR();

    int fId, fLen, ret;
    byte frameBuf[32];
    MegaDevice::DeviceId lId = LOCALMRV()->getDeviceId();
    ret = LOCALMRV()->Bus()->receiveProxy()->readAFrame(
                lId,
                &fId,
                frameBuf,
                &fLen,
                10
                );
    if ( ret == 0 && fLen > 0 )
    { SCPI_ResultArbitraryBlock( context, frameBuf, fLen ); }
    else
    { scpi_ret( SCPI_RES_ERR ); }

    return SCPI_RES_OK;
}

static scpi_command_t _scpi_cmds[]=
{
    #include "../mrv_board/_MRV_scpi_cmd.h"

    COM_ITEMs(),

    CMD_ITEM( "*IDN?", _scpi_idn ),

    CMD_ITEM( "VERSION?", _scpi_qVersion ),

    CMD_ITEM( "*LRN", _scpi_lrn ),      //! setupfile
//    CMD_ITEM( "HRST", _scpi_hrst ),

    CMD_ITEM( "STATE?", _scpi_fsmState ),

    CMD_ITEM( "RUN", _scpi_run ),
    CMD_ITEM( "STOP", _scpi_stop ),
    CMD_ITEM( "SYNC", _scpi_sync ),

    CMD_ITEM( "PROGRAM", _scpi_program ),
    CMD_ITEM( "CALL", _scpi_call ),

    CMD_ITEM( "WRITE", _scpi_arb_write ),
    CMD_ITEM( "READ", _scpi_arb_read ),

    SCPI_CMD_LIST_END
};

namespace MegaDevice
{
const void* deviceMRV::loadScpiCmd()
{
    return _scpi_cmds;
}
}
