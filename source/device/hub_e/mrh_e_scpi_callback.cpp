#include "scpi/scpi.h"

#include "../_scpi_xxx_device.h"
#include "mrh_e.h"

#define _OBJ        LOCAL_OBJ( MegaDevice::Mrh_e )

typedef scpi_result_t (*_scpi_cb)( scpi_t * context );

static char * cbToKeyw( scpi_t * context, _scpi_cb pfCb )
{
    scpi_command_t *pCmdList;

    Q_ASSERT( NULL != context );
    Q_ASSERT( NULL != pfCb );

    pCmdList = (scpi_command_t*)context->cmdlist;

    do
    {
        if ( pCmdList->callback == pfCb )
        { return (char*)pCmdList->pattern; }

        if ( pCmdList->pattern == NULL || pCmdList->callback == NULL )
        { return NULL; }

        pCmdList++;

    }while( 1 );

    return NULL;
}

static scpi_result_t _transfer_write( scpi_t * context,
                                      bool bTrim,
                                      byte v )
{
    int ret = _OBJ->write( (byte*)context->buffer.data,
                            context->buffer.position,
                            bTrim,
                            v );
    if ( ret != 0 )
    { logDbg()<<context->buffer.data;
        scpi_ret( SCPI_RES_ERR ); }
    else
    { return SCPI_RES_OK; }
}

static scpi_result_t _transfer_recv( scpi_t * context )
{
    int retLen;
    byte *pBuf;
    pBuf = _OBJ->recv( retLen );

    //! \todo remove the \0 from mrh-e
    if ( retLen > 0 )
    {
        if ( retLen > 1 && pBuf[retLen-1] == '\0' )
        {
            SCPI_ResultCharacters( context, (char*)pBuf, retLen - 1 );
        }
        else
        {
            SCPI_ResultCharacters( context, (char*)pBuf, retLen );
        }

        return SCPI_RES_OK;
    }
    else
    { return SCPI_RES_ERR; }
}

static scpi_result_t transfer_request( scpi_t * context,
                                       bool bTrim,
                                       byte v )
{
    scpi_result_t ret;

    receiveCache::lock();

    //! move the head
    lex_state_t * state = &context->param_list.lex_state;
    state->pos = state->buffer + state->len;

    ret = _transfer_write( context,
                           bTrim,
                           v );

    receiveCache::unlock();

    return ret;
}

static scpi_result_t transfer_query( scpi_t * context,
                                     bool bTrim,
                                     byte v)
{
    scpi_result_t ret;

    receiveCache::lock();

    //! move the head
    lex_state_t * state = &context->param_list.lex_state;
    state->pos = state->buffer + state->len;

    ret = _transfer_write( context,
                           bTrim,
                           v );

    ret = _transfer_recv( context );

    receiveCache::unlock();

    return ret;
}

//! \todo bus id
static scpi_result_t _scpi_bypass( scpi_t * context )
{
    int iVal;

    if ( SCPI_RES_OK != SCPI_ParamInt( context, &iVal, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    receiveCache::setByPass( 0, iVal > 0 );

    return SCPI_RES_OK;
}

//! \todo bus id
static scpi_result_t _scpi_bypass_q( scpi_t * context )
{
    int val = receiveCache::getByPass(0)? 1 : 0;
    SCPI_ResultInt32( context, val );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_idn( scpi_t * context )
{
    DEF_LOCAL_VAR();

//    QString str;

//    str = _OBJ->getName();

//    SCPI_ResultText( context, str.toLatin1().data() );

//    return SCPI_RES_OK;

    return transfer_query( context, false, 0 );
}

//static scpi_result_t _scpi_system_mode_switch( scpi_t * context )
//{
//    DEF_LOCAL_VAR();

//    logDbg()<<cbToKeyw( context, _scpi_system_mode_switch )
//            <<context->input_count
//            <<context->buffer.position
//            <<context->buffer.length
//            <<context->buffer.data;

//    transfer_write();
//}

//static scpi_result_t _scpi_system_mode_switch_q( scpi_t * context )
//{
//    DEF_LOCAL_VAR();

//    transfer_write();

//    transfer_recv();
//}

//! normal
#define make_w_api( api ) static scpi_result_t api( scpi_t * context )\
{\
    DEF_LOCAL_VAR();\
\
    return transfer_request( context, false, 0 );\
\
}

#define make_r_api( api ) static scpi_result_t api( scpi_t * context )\
{\
    DEF_LOCAL_VAR();\
\
    return transfer_query( context, false, 0 );\
}

//! trim
#define make_trim_w_api( api, t ) static scpi_result_t api( scpi_t * context )\
{\
    DEF_LOCAL_VAR();\
\
    return transfer_request( context, true, t );\
\
}

#define make_trim_r_api( api, t ) static scpi_result_t api( scpi_t * context )\
{\
    DEF_LOCAL_VAR();\
\
    return transfer_query( context, true, t );\
}

make_w_api( _scpi_system_mode_switch )
make_r_api( _scpi_system_mode_switch_q )
//make_r_api( _scpi_system_boardinfo_q )

make_w_api( _scpi_script_run )
make_w_api( _scpi_script_run_cycle )
make_r_api( _scpi_script_run_cycle_q )
make_r_api( _scpi_script_run_currcycle_q )

make_w_api( _scpi_script_stop )
make_r_api( _scpi_script_state_q )

make_trim_w_api( _scpi_script_file_download, '\"' )
make_r_api( _scpi_script_file_download_q )
make_trim_r_api( _scpi_script_file_size_q, '\"' )
make_trim_r_api( _scpi_script_file_read_q, '\"' )
make_r_api( _scpi_script_file_item_q )
make_trim_w_api( _scpi_script_file_delete, '\"' )
make_trim_r_api( _scpi_script_file_exist_q, '\"' )
make_trim_w_api( _scpi_script_file_save_name, '\"' )
make_w_api( _scpi_script_file_data )
make_w_api( _scpi_script_file_end )

make_w_api( _scpi_debug_factory_ioconfig )
make_w_api( _scpi_debug_factory_ioset )
make_r_api( _scpi_debug_factory_ioget )
make_r_api( _scpi_debug_factory_iogetall )


//! write "adddfa,adfsdf,fafa"
static scpi_result_t _scpi_write( scpi_t * context )
{
    DEF_LOCAL_VAR();

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;
    if (strLen < 1)
    { scpi_ret( SCPI_RES_ERR ); }

    int ret = LOCAL_OBJ( MegaDevice::Mrh_e )->write( (byte*)pLocalStr, strLen );
    if ( ret != 0 )
    { return SCPI_RES_ERR; }

    return SCPI_RES_OK;
}

//! read? len, tmo
static scpi_result_t _scpi_read( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int len, tmo;

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &len, true ) )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_RES_OK != SCPI_ParamInt32( context, &tmo, true ) )
    { tmo = 2000; }

    byte *pData = new byte[ len + 1 ];
    if ( NULL==pData )
    { return SCPI_RES_ERR; }

    int ret, retLen;
    ret = LOCAL_OBJ( MegaDevice::Mrh_e )->read( pData, len, tmo, retLen );
logDbg()<<retLen;
    if ( ret != 0 )
    {
        delete []pData;
        return SCPI_RES_ERR;
    }

    //! result
    pData[ retLen ] = 0;
    SCPI_ResultText( context, (const char*)pData );

    delete []pData;

    return SCPI_RES_OK;
}

static scpi_command_t _scpi_cmds[]=
{
    COM_ITEMs(),

    CMD_ITEM( "BYPASS", _scpi_bypass ),
    CMD_ITEM( "BYPASS?", _scpi_bypass_q ),

    CMD_ITEM( "*IDN?", _scpi_idn ),

    CMD_ITEM( "SYSTEM:MODE:SWITCH", _scpi_system_mode_switch ),
    CMD_ITEM( "SYSTEM:MODE:READ?", _scpi_system_mode_switch_q ),

//    CMD_ITEM( "SYSTEM:BOARDINFO?", _scpi_system_boardinfo_q ),

    CMD_ITEM( "SCRIPT:RUN", _scpi_script_run ),
    CMD_ITEM( "SCRIPT:RUN:CYCLE", _scpi_script_run_cycle ),
    CMD_ITEM( "SCRIPT:RUN:CYCLE?", _scpi_script_run_cycle_q ),
    CMD_ITEM( "SCRIPT:RUN:CURRCYCLE?", _scpi_script_run_currcycle_q ),

    CMD_ITEM( "SCRIPT:STOP", _scpi_script_stop ),
    CMD_ITEM( "SCRIPT:STATE?", _scpi_script_state_q ),

    CMD_ITEM( "SCRIPT:FILE:DOWNLOAD", _scpi_script_file_download ),
    CMD_ITEM( "SCRIPT:FILE:DOWNLOAD?", _scpi_script_file_download_q ),
    CMD_ITEM( "SCRIPT:FILE:SIZE?", _scpi_script_file_size_q ),
    CMD_ITEM( "SCRIPT:FILE:READ?", _scpi_script_file_read_q ),
    CMD_ITEM( "SCRIPT:FILE:ITEM?", _scpi_script_file_item_q ),
    CMD_ITEM( "SCRIPT:FILE:DELETE", _scpi_script_file_delete ),
    CMD_ITEM( "SCRIPT:FILE:EXIST?", _scpi_script_file_exist_q ),
    CMD_ITEM( "SCRIPT:FILE:SAVE:NAME", _scpi_script_file_save_name ),
    CMD_ITEM( "SCRIPT:FILE:SAVE:DATA", _scpi_script_file_data ),
    CMD_ITEM( "SCRIPT:FILE:SAVE:END", _scpi_script_file_end ),

//    CMD_ITEM( "SCRIPT:OFFLINE:SWITCH?", _scpi_idn ),
//    CMD_ITEM( "SCRIPT:OFFLINE:SWITCH:BIND", _scpi_idn ),
//    CMD_ITEM( "SCRIPT:OFFLINE:SWITCH:UNBIND", _scpi_idn ),
//    CMD_ITEM( "SCRIPT:OFFLINE:SWITCH:STOPBIND", _scpi_idn ),
//    CMD_ITEM( "SCRIPT:OFFLINE:SWITCH:STOPBIND?", _scpi_idn ),
//    CMD_ITEM( "SCRIPT:OFFLINE:SWITCH:STIOUNBIND", _scpi_idn ),
//    CMD_ITEM( "SCRIPT:OFFLINE:RESET", _scpi_idn ),

//    CMD_ITEM( "MOTION:ADJUST:RUN", _scpi_idn ),
//    CMD_ITEM( "MOTION:ADJUST:RUN?", _scpi_idn ),
//    CMD_ITEM( "MOTION:ADJUST:POINT:START", _scpi_idn ),
//    CMD_ITEM( "MOTION:ADJUST:POINT:MARK", _scpi_idn ),
//    CMD_ITEM( "MOTION:ADJUST:POINT?", _scpi_idn ),

//    CMD_ITEM( "MOTION:HOME:RUN", _scpi_idn ),
//    CMD_ITEM( "MOTION:HOME:RUN?", _scpi_idn ),

//    CMD_ITEM( "ENCODER:LINE", _scpi_idn ),
//    CMD_ITEM( "ENCODER:LINE?", _scpi_idn ),
//    CMD_ITEM( "ENCODER:CHAN", _scpi_idn ),
//    CMD_ITEM( "ENCODER:CHAN?", _scpi_idn ),
//    CMD_ITEM( "ENCODER:TYPE", _scpi_idn ),
//    CMD_ITEM( "ENCODER:TYPE?", _scpi_idn ),

//    CMD_ITEM( "REPORT:VALUE?", _scpi_idn ),

//    CMD_ITEM( "BRANCH:VALUE?", _scpi_idn ),
//    CMD_ITEM( "BRANCH:STATE", _scpi_idn ),
//    CMD_ITEM( "BRANCH:STATE?", _scpi_idn ),

//    CMD_ITEM( "DEBUG:DATA", _scpi_idn ),
//    CMD_ITEM( "DEBUG:DATA?", _scpi_idn ),

    CMD_ITEM( "DEBUG:FACTORY:IOCONFIG", _scpi_debug_factory_ioconfig ),
    CMD_ITEM( "DEBUG:FACTORY:IOSET", _scpi_debug_factory_ioset ),
    CMD_ITEM( "DEBUG:FACTORY:IOGET", _scpi_debug_factory_ioget ),
    CMD_ITEM( "DEBUG:FACTORY:IOGETALL", _scpi_debug_factory_iogetall ),
//    CMD_ITEM( "DEBUG:FACTORY:SERIAL", _scpi_idn ),
//    CMD_ITEM( "DEBUG:FACTORY:RESETSD", _scpi_idn ),

    //! base
    CMD_ITEM( "WRITE", _scpi_write ),
    CMD_ITEM( "READ?", _scpi_read ),

    SCPI_CMD_LIST_END
};


namespace MegaDevice {

const void* Mrh_e::loadScpiCmd()
{
    return _scpi_cmds;
}

}
