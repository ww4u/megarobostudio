#include "scpishell.h"

#include "scpi/scpi.h"
extern const scpi_unit_def_t scpi_unit_def[];

#define CHECK_CONTEXT( context, fail )    if ( context == NULL || context->user_context == NULL )\
                                    { return fail; }

#define scpi_shell( context)        ((scpiShell*)context->user_context)

static int _testCnt = 0;

int setCnt( int cnt )
{
    _testCnt = cnt;
    return _testCnt;
}
int getCnt()
{
    return _testCnt;
}

//! common api
static size_t SCPI_Write(scpi_t * context, const char * data, size_t len) {
    (void) context;

    CHECK_CONTEXT( context, 0 );

    return scpi_shell(context)->SCPI_Write( context, data, len );
}

static scpi_result_t SCPI_Flush(scpi_t * context) {
    (void) context;

    CHECK_CONTEXT( context, SCPI_RES_ERR );

    return (scpi_result_t)scpi_shell(context)->SCPI_Flush( context );
}

static int SCPI_Error(scpi_t * context, int_fast16_t err) {
    (void) context;

    CHECK_CONTEXT( context, 0 );

    return scpi_shell(context)->SCPI_Error( context, err );
}

static scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val) {
    (void) context;

    CHECK_CONTEXT( context, SCPI_RES_ERR );

    return (scpi_result_t)scpi_shell(context)->SCPI_Control( context, ctrl, val );
}

static scpi_result_t SCPI_Reset(scpi_t * context) {
    (void) context;

    CHECK_CONTEXT( context, SCPI_RES_ERR );

    return (scpi_result_t)scpi_shell(context)->SCPI_Reset( context );
}

static scpi_interface_t _scpi_interface = {
    /*.error = */   SCPI_Error,
    /*.write = */   SCPI_Write,
    /*.control = */ SCPI_Control,
    /*.flush = */   SCPI_Flush,
    /*.reset = */   SCPI_Reset,
};

//! shell api
scpiShell::scpiShell()
{
    mIntputBufferSize = 0;
    mOutputBufferSize = 0;
    mErrQueueSize     = 0;

    p_scpi_interface = NULL;
    p_scpi_input_buffer = NULL;
    p_scpi_output_buffer = NULL;
    p_scpi_error_queue_data = NULL;
    p_scpi_context = NULL;

    mOutWritePos = 0;
    mOutReadPos = 0;

    m_pUserPara = NULL;

    mOpc = 1;

    m_pMutex = new QMutex( QMutex::Recursive );
    Q_ASSERT( NULL != m_pMutex );
}

scpiShell::~scpiShell()
{
    delete m_pMutex;

    gc();
}

void scpiShell::setObjPara( const QString &name, void *pPara )
{
    m_pUserPara = pPara;
}

int scpiShell::open( int inSize,
                               int outSize,
                               int errSize )
{
    //! memory config
    mIntputBufferSize = inSize;
    mOutputBufferSize = outSize;
    mErrQueueSize     = errSize;

    p_scpi_input_buffer = new char[ mIntputBufferSize ];
    p_scpi_output_buffer = new char[ mOutputBufferSize ];
    p_scpi_error_queue_data = new scpi_error_t[ mErrQueueSize ];

    p_scpi_interface = new scpi_interface_t;
    p_scpi_context = new scpi_t;

    if ( NULL == p_scpi_input_buffer || NULL == p_scpi_error_queue_data )
    { return SCPI_RES_ERR; }

    //! init context
    //! device1
    SCPI_Init( (scpi_t*)p_scpi_context,
              (const scpi_command_t *)loadScpiCmd(),
              (scpi_interface_t*)p_scpi_interface,
              NULL,
              getIdn1(), getIdn2(), getIdn3(), getIdn4(),
              p_scpi_input_buffer, mIntputBufferSize,
              (scpi_error_t*)p_scpi_error_queue_data, mErrQueueSize );

    (*(scpi_interface_t*)p_scpi_interface)=_scpi_interface;

    //! index
    ((scpi_t*)p_scpi_context)->user_context = this;

    return SCPI_RES_OK;
}
int scpiShell::close()
{
    gc();

    return SCPI_RES_OK;
}

int scpiShell::write( const char *p, int len )
{
    //! reset the output buffer
    mOutReadPos = 0;
    mOutWritePos = 0;

    Q_ASSERT( NULL != p_scpi_context );

    lock();
        //! data in
        scpi_bool_t ret;
        ret = SCPI_Input( (scpi_t*)p_scpi_context, p, len );
        if ( !ret )
        { qDebug()<<"!!!"<<__FUNCTION__<<__LINE__<<QByteArray( p, len )<<ret; }
    unlock();

    return SCPI_RES_OK;
}

int scpiShell::write( const char *p )
{
    return write( p, strlen(p) );
}

int scpiShell::size()
{
    return mOutWritePos;
}
int scpiShell::read( char *pOut, int len )
{
    //! limit the len
    int maxLen = mOutWritePos - mOutReadPos;
    if ( len > maxLen )
    { len = maxLen; }

    if ( len > 0 )
    {
        memcpy( pOut, p_scpi_output_buffer + mOutReadPos, len );
        mOutReadPos += len;

        return len;
    }
    else
    {
        return 0;
    }
}
int scpiShell::error()
{
    return SCPI_RES_OK;
}

const void* scpiShell::loadScpiCmd()
{
    return 0;
}

const char* scpiShell::getIdn1()
{ return "A"; }
const char* scpiShell::getIdn2()
{ return "B"; }
const char* scpiShell::getIdn3()
{ return "C"; }
const char* scpiShell::getIdn4()
{ return "D"; }

int scpiShell::SCPI_Write(void * context, const char * data, size_t len)
{
    if ( p_scpi_output_buffer == NULL )
    { return -1; }

    //! get context
    scpi_t *pContext = (scpi_t*)context;
    if ( pContext == NULL || pContext->user_context == NULL )
    { return 0; }

    if ( (int)(mOutWritePos + len) > mOutputBufferSize )
    { return 0; }

    memcpy( p_scpi_output_buffer + mOutWritePos, data, len );
    mOutWritePos += len;

    return len;
}

int scpiShell::SCPI_Flush(void * context)
{
    mOutReadPos = 0;

    return 0;
}

int scpiShell::SCPI_Error(void * context, int_fast16_t err)
{
    return 0;
}
int scpiShell::SCPI_Control(void * context, int ctrl, unsigned short val)
{
    return 0;
}
int scpiShell::SCPI_Reset( void *context )
{
    SCPI_Flush( context );

    return 0;
}

void scpiShell::gc()
{
    if ( NULL != p_scpi_input_buffer )
    {
        delete []p_scpi_input_buffer;
        p_scpi_input_buffer = NULL;
    }
    if ( NULL != p_scpi_output_buffer )
    {
        delete []p_scpi_output_buffer;
        p_scpi_output_buffer = NULL;
    }
    if ( NULL != p_scpi_error_queue_data )
    {
        scpi_error_t *pErr = (scpi_error_t *)p_scpi_error_queue_data;
        delete []pErr;
        p_scpi_error_queue_data = NULL;
    }
    if ( NULL != p_scpi_interface )
    {
        scpi_interface_t *pIntf = (scpi_interface_t*)p_scpi_interface;
        delete pIntf;
        p_scpi_interface = NULL;
    }

    if ( NULL != p_scpi_context )
    {
        scpi_t *pContext = (scpi_t*)p_scpi_context;
        delete pContext;
        p_scpi_context = NULL;
    }
}

void scpiShell::setOpc( int v )
{ mOpc = v; }
int scpiShell::opc()
{ return mOpc; }

void scpiShell::lock()
{ m_pMutex->lock(); }
void scpiShell::unlock()
{ m_pMutex->unlock(); }

