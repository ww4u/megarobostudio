#include "scpi/scpi.h"

#include "deviceMRQ.h"
#include "scpi_obj.h"
#include "../_scpi_xxx_device.h"

#include "../board/_MRQ_scpi_callback.cpp"

#include "../../com/comassist.h"

#define DEF_MRQ()   MegaDevice::deviceMRQ* _localMrq = (GET_OBJ(context));
#define LOCALMRQ()  _localMrq

#define CHECK_LINK( ax, page )    if ( LOCALMRQ()->checkLink(ax,page) )\
                        {}\
                        else\
                        { scpi_ret( SCPI_RES_ERR ); }

static scpi_result_t _scpi_testAdd( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int val1, val2, ret;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &val1, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &val2, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    ret = ((MegaDevice::deviceMRQ*)context->user_context)->testAdd( val1, val2 );
    SCPI_ResultInt32( context, ret );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_idn( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    QString str;

    str = ((MegaDevice::deviceMRQ*)context->user_context)->getModel()->getSN();

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

    DEF_MRQ();
    QByteArray rawFileName( pLocalStr, strLen );
    QString fileName( rawFileName );
    if ( comAssist::ammendFileName( fileName ) )
    {}
    else
    { scpi_ret( SCPI_RES_ERR ); }

    Q_ASSERT( LOCALMRQ() != NULL );
    int ret = LOCALMRQ()->getModel()->load( fileName );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    ret = LOCALMRQ()->applySetting();
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    sysLog( QObject::tr("Setting Down") );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_hrst( scpi_t * context )
{
    DEF_LOCAL_VAR();

    DEF_MRQ();

    LOCALMRQ()->hRst();

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_sn( scpi_t * context )
{
    DEF_LOCAL_VAR();

    DEF_MRQ();

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;
    if (strLen < 1)
    { scpi_ret( SCPI_RES_ERR ); }

    QByteArray rawSn( pLocalStr, strLen );
    QString sn( rawSn );

    LOCALMRQ()->setSN( sn );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_qSn( scpi_t * context )
{
    DEF_LOCAL_VAR();

    DEF_MRQ();

    QString str;
    str = LOCALMRQ()->loadSN();

    SCPI_ResultText( context, str.toLatin1().data() );

    return SCPI_RES_OK;
}

//! ax,page
static scpi_result_t _scpi_run( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRQ();

    CHECK_LINK( ax, page );

    LOCALMRQ()->run( tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! ax,page
static scpi_result_t _scpi_stop( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRQ();

    CHECK_LINK( ax, page );

    LOCALMRQ()->stop( tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! int, float, float
//! ax, page, t, angle
static scpi_result_t _scpi_rotate( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;
    float val2, val3;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamFloat( context, &val2, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamFloat( context, &val3, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRQ();

    CHECK_LINK( ax, page );

    LOCALMRQ()->rotate( tpvRegion(ax,page), val2, val3 );

    return SCPI_RES_OK;
}

//! int, float, float
//! ax, page, t, angle
static scpi_result_t _scpi_preRotate( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;
    float val2, val3;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamFloat( context, &val2, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamFloat( context, &val3, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRQ();

    CHECK_LINK( ax, page );

    LOCALMRQ()->preRotate( tpvRegion(ax,page), val2, val3 );

    return SCPI_RES_OK;
}

//! ax, page, t, angle, tj, j
static scpi_result_t _scpi_movej( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;
    float vals[4];

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    //! deload para
    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, &vals[i], true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    DEF_MRQ();

    CHECK_LINK( ax, page );

    LOCALMRQ()->movej( tpvRegion(ax,page), vals[1], vals[0], vals[3], vals[2] );

    return SCPI_RES_OK;
}

//! ax, page, t, angle, tj, j
static scpi_result_t _scpi_preMovej( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;
    float vals[4];

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    //! deload para
    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, &vals[i], true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    DEF_MRQ();

    CHECK_LINK( ax, page );

    LOCALMRQ()->preMovej( tpvRegion(ax,page), vals[1], vals[0], vals[3], vals[2] );

    return SCPI_RES_OK;
}

//! CALL ax, page
static scpi_result_t _scpi_call( _scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRQ();

    CHECK_LINK( ax, page );

    LOCALMRQ()->call( tpvRegion(ax, page) );

    return SCPI_RES_OK;
}

//! ax, page, t, angle, end speed
static scpi_result_t _scpi_lightZero( _scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;
    float t, angle, speed;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamFloat( context, &t, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamFloat( context, &angle, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamFloat( context, &speed, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRQ();

    CHECK_LINK( ax, page );

    LOCALMRQ()->lightCouplingZero( tpvRegion(ax,page),
                                   t, angle, speed );

    return SCPI_RES_OK;
}

//! ax, page, t, angle, endV, invt, invangle, tmos, ticks
static scpi_result_t _scpi_task( _scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;
    float vals[ 2 + 1 + 2 + 2 ];


    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    //! deload para
    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, &vals[i], true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    DEF_MRQ();

    CHECK_LINK( ax, page );

    LOCALMRQ()->lightCouplingZero( tpvRegion(ax,page),
                                   vals[0], vals[1], vals[2],
                                   vals[3], vals[4],
                                   vals[5]*time_s(1),vals[6]*time_s(1)
                                    );

    return SCPI_RES_OK;
}

//! TPV ax,page,e:/ddd.csv
static scpi_result_t _scpi_program( scpi_t * context )
{logDbg();
    // read
    DEF_LOCAL_VAR();

    int ax, page;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<ax;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<page;

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;
    if (strLen < 1)
    { scpi_ret( SCPI_RES_ERR ); }

    //! load
    QList<float> dataSets;
    int col = 3;
    if ( 0 != comAssist::loadDataset( pLocalStr, strLen, col, dataSets ) )
    { scpi_ret( SCPI_RES_ERR ); }

    int dotSize = dataSets.size()/col;
    if ( (dotSize < 2) )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<dotSize;

    //! t,p,v
    tpvRow *pDots = new tpvRow[ dotSize ];

    if ( NULL == pDots )
    { logDbg(); scpi_ret( SCPI_RES_ERR ); }

    //! move data
    for( int i = 0; i < dotSize; i++ )
    {
        for ( int j = 0; j < col; j++ )
        {
            pDots[i].datas[j] = dataSets.at(i*col+j);
            pDots[i].setGc( true );
        }
    }

    DEF_MRQ();

    CHECK_LINK( ax, page );

    //! send
    int ret = -1;
    ret = LOCALMRQ()->pvtWrite( tpvRegion(ax, page), pDots, dotSize );

    gc_array( pDots );

    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }
    else
    { return SCPI_RES_OK; }
}

//! xxx ax,page
static scpi_result_t _scpi_fsmState( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page, ret;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &page, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRQ();

    CHECK_LINK( ax, page );

    ret = LOCALMRQ()->fsmState(  tpvRegion(ax,page) );

    SCPI_ResultInt32( context, ret );

    return SCPI_RES_OK;
}

//! XXX ax
static scpi_result_t _scpi_incangle( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    float val = 0;

    DEF_MRQ();

    CHECK_LINK( ax, 0 );

    val = LOCALMRQ()->getIncAngle( ax );

    SCPI_ResultFloat( context, val );

    return SCPI_RES_OK;
}

//! ax
static scpi_result_t _scpi_absangle( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    float val = 0;

    DEF_MRQ();

    CHECK_LINK( ax, 0 );

    val = LOCALMRQ()->getAbsAngle( ax );

    SCPI_ResultFloat( context, val );

    return SCPI_RES_OK;
}

//! ax
static scpi_result_t _scpi_distance( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    float val = 0;

    DEF_MRQ();

    CHECK_LINK( ax, 0 );

    val = LOCALMRQ()->getDist( ax );

    SCPI_ResultFloat( context, val );

    return SCPI_RES_OK;
}

//! duty
static scpi_result_t _scpi_fanduty( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int val1;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &val1, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRQ();
    int ret;
    ret =  LOCALMRQ()->setFanDuty( val1 );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    return SCPI_RES_OK;
}

//! ax,duty
static scpi_result_t _scpi_ledduty( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int val1, val2;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &val1, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &val2, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRQ();
    int ret;
    ret =  LOCALMRQ()->setLedDuty( val1, val2 );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_encoderZeroValid( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    DEF_MRQ();

    bool bRet = LOCALMRQ()->getEncoderZeroValid();

    SCPI_ResultInt32( context, bRet );

    return SCPI_RES_OK;
}

//! ax
//! valid, zeroAngle
static scpi_result_t _scpi_encoderZero( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int val1;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &val1, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRQ();

    if ( LOCALMRQ()->getEncoderZeroValid() )
    {}
    else
    { scpi_ret( SCPI_RES_ERR ); }

    float angle;
    angle =  LOCALMRQ()->getEncoderZero( val1 );

    SCPI_ResultFloat( context, angle );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_busFrames( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();


    DEF_MRQ();

    qint64 frames = LOCALMRQ()->busFrames();

    SCPI_ResultInt64( context, frames );

    return SCPI_RES_OK;
}

//! id,val
static scpi_result_t _scpi_ioOut( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int vals[2];

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamInt32( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    DEF_MRQ();

    if( 0 != LOCALMRQ()->ioOut( vals[0], vals[1]) )
    { scpi_ret( SCPI_RES_ERR ); }

    return SCPI_RES_OK;
}

static scpi_command_t _mrq_scpi_cmds[]=
{
    #include "../board/_MRQ_scpi_cmd.h"

    CMD_ITEM( "*IDN?", _scpi_idn ),
    CMD_ITEM( "*LRN", _scpi_lrn ),      //! setupfile
    CMD_ITEM( "HRST", _scpi_hrst ),

    CMD_ITEM( "SN", _scpi_sn ),
    CMD_ITEM( "SN?", _scpi_qSn ),

    CMD_ITEM( "TEST:ADD", _scpi_testAdd ),

    CMD_ITEM( "RUN", _scpi_run ),
    CMD_ITEM( "STOP", _scpi_stop ),

    CMD_ITEM( "ROTATE", _scpi_rotate ),
    CMD_ITEM( "MOVE", _scpi_rotate ),
    CMD_ITEM( "PREMOVE", _scpi_preRotate ),

    CMD_ITEM( "MOVEJ", _scpi_movej ),
    CMD_ITEM( "PREMOVEJ", _scpi_preMovej ),

    CMD_ITEM( "STATE?", _scpi_fsmState ),

    CMD_ITEM( "ANGLE:INCREASE?", _scpi_incangle ),
    CMD_ITEM( "ANGLE:ABSOLUTE?", _scpi_absangle ),
    CMD_ITEM( "DISTANCE?", _scpi_distance ),

    CMD_ITEM( "PROGRAM", _scpi_program ),
    CMD_ITEM( "DOWNLOAD", _scpi_program ),

    CMD_ITEM( "CALL", _scpi_call ),

    CMD_ITEM( "LZERO", _scpi_lightZero ),             //! light zero
    CMD_ITEM( "PEZERO", _scpi_lightZero ),

    CMD_ITEM( "TASK", _scpi_task ),

    CMD_ITEM( "FANDUTY", _scpi_fanduty ),
    CMD_ITEM( "LEDDUTY", _scpi_ledduty ),

    CMD_ITEM( "ENCODER:ZEROVALID?", _scpi_encoderZeroValid ),
    CMD_ITEM( "ENCODER:ZERO?", _scpi_encoderZero ),

    CMD_ITEM( "FRAMES?", _scpi_busFrames ),

    CMD_ITEM( "IO:OUT", _scpi_ioOut ),

    SCPI_CMD_LIST_END
};

namespace MegaDevice
{
const void* deviceMRQ::loadScpiCmd()
{
    return _mrq_scpi_cmds;
}
}



