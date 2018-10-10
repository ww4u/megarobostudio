#include "scpi/scpi.h"

#include "../_scpi_xxx_device.h"
#include "mrh_t.h"

#define _OBJ        LOCAL_OBJ( MegaDevice::Mrh_t )

typedef scpi_result_t (*_scpi_cb)( scpi_t * context );

//static char * cbToKeyw( scpi_t * context, _scpi_cb pfCb )
//{
//    scpi_command_t *pCmdList;

//    Q_ASSERT( NULL != context );
//    Q_ASSERT( NULL != pfCb );

//    pCmdList = (scpi_command_t*)context->cmdlist;

//    do
//    {
//        if ( pCmdList->callback == pfCb )
//        { return (char*)pCmdList->pattern; }

//        if ( pCmdList->pattern == NULL || pCmdList->callback == NULL )
//        { return NULL; }

//        pCmdList++;

//    }while( 1 );

//    return NULL;
//}

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

    return transfer_query( context, false, 0 );
}

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

make_w_api( _scpi_project_write )
make_r_api( _scpi_project_xread )


//! write "adddfa,adfsdf,fafa"
static scpi_result_t _scpi_write( scpi_t * context )
{
    DEF_LOCAL_VAR();

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;
    if (strLen < 1)
    { scpi_ret( SCPI_RES_ERR ); }

    int ret = LOCAL_OBJ( MegaDevice::Mrh_t )->write( (byte*)pLocalStr, strLen );
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
    ret = LOCAL_OBJ( MegaDevice::Mrh_t )->read( pData, len, tmo, retLen );
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

    CMD_ITEM( "PROJECT:YWRITE", _scpi_project_write ),
    CMD_ITEM( "PROJECT:XREAD?", _scpi_project_xread ),

    //! base
    CMD_ITEM( "WRITE", _scpi_write ),
    CMD_ITEM( "READ?", _scpi_read ),

    SCPI_CMD_LIST_END
};


namespace MegaDevice {

const void* Mrh_t::loadScpiCmd()
{
    return _scpi_cmds;
}

}
