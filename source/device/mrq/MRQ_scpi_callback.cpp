#include "scpi/scpi.h"

#include "deviceMRQ.h"
#include "scpi_obj.h"
#include "../_scpi_xxx_device.h"

#include "../board/_MRQ_scpi_callback.cpp"

static scpi_result_t _scpi_testAdd( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int val1, val2, ret;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &val1, true ) )
    { return SCPI_RES_ERR; }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &val2, true ) )
    { return SCPI_RES_ERR; }

    ret = ((MegaDevice::deviceMRQ*)context->user_context)->testAdd( val1, val2 );
    SCPI_ResultInt32( context, ret );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_idn( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    qDebug()<<__FUNCTION__<<__LINE__;

    QString str;

    str = ((MegaDevice::deviceMRQ*)context->user_context)->getModel()->getSN();

    qDebug()<<str<<__FUNCTION__<<__LINE__;
    SCPI_ResultText( context, str.toLatin1().data() );

    return SCPI_RES_OK;
}

//! int, float, float
static scpi_result_t _scpi_rotate( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int val1;
    float val2, val3;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &val1, true ) )
    { return SCPI_RES_ERR; }

    if ( SCPI_RES_OK != SCPI_ParamFloat( context, &val2, true ) )
    { return SCPI_RES_ERR; }

    if ( SCPI_RES_OK != SCPI_ParamFloat( context, &val3, true ) )
    { return SCPI_RES_ERR; }

    ((MegaDevice::deviceMRQ*)context->user_context)->rotate( val1, val2, val3 );

    return SCPI_RES_OK;
}

//! int
static scpi_result_t _scpi_fsmState( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int val1, ret;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &val1, true ) )
    { return SCPI_RES_ERR; }

    ret = ((MegaDevice::deviceMRQ*)context->user_context)->fsmState(  val1 );

    SCPI_ResultInt32( context, ret );

    return SCPI_RES_OK;
}

static scpi_command_t _mrq_scpi_cmds[]=
{
    #include "../board/_MRQ_scpi_cmd.h"

    CMD_ITEM( "*IDN?", _scpi_idn ),

    CMD_ITEM( "TEST:ADD", _scpi_testAdd ),

    CMD_ITEM( "ROTATE", _scpi_rotate ),

    CMD_ITEM( "FSMSTATE?", _scpi_fsmState ),

    SCPI_CMD_LIST_END
};

namespace MegaDevice
{
const void* deviceMRQ::loadScpiCmd()
{
    return _mrq_scpi_cmds;
}
}



