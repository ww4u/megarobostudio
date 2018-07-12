#include "scpi/scpi.h"

#include "devicemrv.h"
//#include "scpi_obj.h"
#include "../_scpi_xxx_device.h"

//! context to obj
#define SET_OBJ( context )	((MegaDevice::deviceMRV*)context->user_context)
#define GET_OBJ( context )      ((MegaDevice::deviceMRV*)context->user_context)

#include "../mrv_board/_MRV_scpi_callback.cpp"

#include "../../com/comassist.h"

#define DEF_MRV()   MegaDevice::deviceMRV* _localMrv = (GET_OBJ(context));
#define LOCALMRV()  _localMrv

#define CHECK_LINK( ax, page )

static scpi_result_t _scpi_idn( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    DEF_MRV();

    QString str;
    str = LOCALMRV()->getModel()->getSN();

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
    // read
    DEF_LOCAL_VAR();

    int ax;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &ax, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    DEF_MRV();

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

static scpi_result_t _scpi_arb_write( scpi_t * context )
{
    DEF_MRV();
    DEF_LOCAL_VAR();

    if ( SCPI_ParamArbitraryBlock(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    QByteArray stream( pLocalStr, strLen );
    logDbg()<<stream;

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

    CMD_ITEM( "*IDN?", _scpi_idn ),

    CMD_ITEM( "*LRN", _scpi_lrn ),      //! setupfile
//    CMD_ITEM( "HRST", _scpi_hrst ),

    CMD_ITEM( "STATE?", _scpi_fsmState ),

    CMD_ITEM( "RUN", _scpi_run ),
    CMD_ITEM( "STOP", _scpi_stop ),


//    CMD_ITEM( "ANGLE:INCREASE?", _scpi_incangle ),
//    CMD_ITEM( "ANGLE:ABSOLUTE?", _scpi_absangle ),
//    CMD_ITEM( "DISTANCE?", _scpi_distance ),

//    CMD_ITEM( "PROGRAM", _scpi_program ),
//    CMD_ITEM( "DOWNLOAD", _scpi_program ),

//    CMD_ITEM( "CALL", _scpi_call ),

//    CMD_ITEM( "LZERO", _scpi_lightZero ),             //! light zero
//    CMD_ITEM( "PEZERO", _scpi_lightZero ),

//    CMD_ITEM( "TASK", _scpi_task ),

//    CMD_ITEM( "FANDUTY", _scpi_fanduty ),
//    CMD_ITEM( "LEDDUTY", _scpi_ledduty ),

//    CMD_ITEM( "ENCODER:ZEROVALID?", _scpi_encoderZeroValid ),
//    CMD_ITEM( "ENCODER:ZERO?", _scpi_encoderZero ),

//    CMD_ITEM( "FRAMES?", _scpi_busFrames ),

//    CMD_ITEM( "IO:OUT", _scpi_ioOut ),

//    //! PDM
//    CMD_ITEM( "PDM:REQDATA", _scpi_requestPdmData ),    //! ch,start,len
//    CMD_ITEM( "PDM:MICUPLOAD", _scpi_micUpload ),       //! ch, filename
//    CMD_ITEM( "PDM:UPLOADSTATE?", _scpi_qUploadState ),

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
