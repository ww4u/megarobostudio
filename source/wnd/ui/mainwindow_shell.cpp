#include "scpi/scpi.h"
#include "mainwindow.h"

#include <QHostInfo>

#define DEF_MGR()   MainWindow * _localWnd =  dynamic_cast<MainWindow*>( ((scpiShell*)( context->user_context )) );\
                                              Q_ASSERT( NULL != _localWnd );
#define LOCAL_MGR() _localWnd

static scpi_result_t _scpi_idn( scpi_t * context )
{
    DEF_LOCAL_VAR();

    QString str;

    str = QHostInfo::localHostName();

    SCPI_ResultText( context, str.toLatin1().data() );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_qOpc( scpi_t * context )
{
    DEF_LOCAL_VAR();

    DEF_MGR();

    int val;
    val = qOpc();

    SCPI_ResultInt32( context, val );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_find( scpi_t *context )
{
    DEF_LOCAL_VAR();

    DEF_MGR();

    LOCAL_MGR()->getDeviceMgr()->on_pushButton_clicked();

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_devices( scpi_t *context )
{
    DEF_MGR();

    QStringList rsrcList = LOCAL_MGR()->getMcModel()->m_pInstMgr->getResources();
    logDbg()<<rsrcList;
    QString ary = rsrcList.join(",");

    SCPI_ResultText( context, ary.toLatin1().data() );

    return SCPI_RES_OK;
}


static scpi_result_t _scpi_robots( scpi_t *context )
{
    DEF_MGR();

    QStringList rsrcList = LOCAL_MGR()->getMcModel()->m_pInstMgr->robots();
    logDbg()<<rsrcList;
    QString ary = rsrcList.join(",");

    SCPI_ResultText( context, ary.toLatin1().data() );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_rsrc( scpi_t *context )
{
    DEF_MGR();

    QStringList rsrcList = LOCAL_MGR()->getMcModel()->m_pInstMgr->resources();
    logDbg()<<rsrcList;
    QString ary = rsrcList.join(",");

    SCPI_ResultText( context, ary.toLatin1().data() );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_diagnosis( scpi_t *context )
{
    DEF_MGR();

    QString str;
    int n;
    str = LOCAL_MGR()->exportDiagnosis( n );

    QString outStr;
    if ( n < 1 )
    { outStr = QString("%1").arg( n ); }
    else
    { outStr = QString("%1,%2").arg( n ).arg( str ); }

    QByteArray outAry, padAry;

    outAry.append( outStr );
    if ( outAry.length() > 512 )
    {
        padAry = outAry.mid( 0, 512 );
    }
    else
    { padAry = outAry; }

    //! attch \0
    padAry.append( '\0' );

    SCPI_ResultText( context, padAry.data() );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_bus_open( scpi_t *context )
{
    DEF_MGR();

    LOCAL_MGR()->getDeviceMgr()->openBus( );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_bus_close( scpi_t *context )
{
    DEF_MGR();

    LOCAL_MGR()->getDeviceMgr()->closeBus(  );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_device_stop( scpi_t *context )
{
    DEF_LOCAL_VAR();

    DEF_MGR();

    LOCAL_MGR()->on_actionStop_triggered();

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_device_terminate( scpi_t *context )
{
    DEF_LOCAL_VAR();

    DEF_MGR();

    LOCAL_MGR()->on_actionForceStop_triggered();

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_device_reset( scpi_t *context )
{
    DEF_LOCAL_VAR();

    DEF_MGR();

    LOCAL_MGR()->on_actionReset_triggered();

    return SCPI_RES_OK;
}

//! device, cmds
static scpi_result_t _scpi_to( scpi_t *context )
{
    DEF_LOCAL_VAR();

    DEF_MGR();

    scpi_result_t ret;

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;

    QByteArray ary( pLocalStr, strLen );
    QString content = ary;

    LOCAL_MGR()->slot_com_receive( content );

    return SCPI_RES_OK;
}

//! script, args..
static scpi_result_t _scpi_rpc( scpi_t *context )
{
    DEF_LOCAL_VAR();

    DEF_MGR();

    scpi_result_t ret;

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;

    QByteArray ary( pLocalStr, strLen );
    QString content = ary;

    //! call
    if ( LOCAL_MGR()->rpcMgr()->rpc( content ) )
    { scpi_ret( SCPI_RES_ERR ); }

    return SCPI_RES_OK;
}
//! script, args..
static scpi_result_t _scpi_rpq( scpi_t *context )
{
    DEF_LOCAL_VAR();

    DEF_MGR();

    scpi_result_t ret;

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;

    QByteArray ary( pLocalStr, strLen );
    QString content = ary;

    QString strq;
    strq = LOCAL_MGR()->rpcMgr()->rpq( content );
    SCPI_ResultText( context, strq.toLatin1().data() );

    return SCPI_RES_OK;
}
//! script, args..
static scpi_result_t _scpi_rpk( scpi_t *context )
{
    DEF_LOCAL_VAR();

    DEF_MGR();

    scpi_result_t ret;

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;

    QByteArray ary( pLocalStr, strLen );
    QString content = ary;

    //! kill
    if ( 0 != LOCAL_MGR()->rpcMgr()->rpk( content ) )
    { scpi_ret( SCPI_RES_ERR ); }
    else
    {}

    return SCPI_RES_OK;
}


static scpi_result_t _scpi_rpgc( scpi_t *context )
{
    DEF_LOCAL_VAR();

    DEF_MGR();

    LOCAL_MGR()->rpcMgr()->rpgc();

    return SCPI_RES_OK;
}

//!
static scpi_result_t _scpi_rpls( scpi_t *context )
{
    DEF_LOCAL_VAR();

    DEF_MGR();

    QStringList lsList;

    lsList = LOCAL_MGR()->rpcMgr()->rpls();

    QString content;
    content = lsList.join('\n');

    //! content
    SCPI_ResultText( context, content.toLatin1().data() );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_tst( scpi_t *context )
{
    DEF_LOCAL_VAR();

    DEF_MGR();

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;

    //! file
    if (strLen < 1)
    { return( SCPI_RES_ERR ); }

    //! find the input file
//    QByteArray byteName( pLocalStr, strLen );

//    QString fileInName ( byteName.data() );
//    QString fileInName = QString::fromLatin1( pLocalStr, strLen );
    QString fileInName = QString::fromUtf8( pLocalStr, strLen );

    if ( comAssist::ammendFileName( fileInName ) )
    {
        logDbg()<<fileInName;
    }
    else
    { return( SCPI_RES_ERR ); }

    return SCPI_RES_OK;
}

static scpi_command_t _mrq_scpi_cmds[]=
{

    CMD_ITEM( "*IDN?", _scpi_idn ),
    CMD_ITEM( "*OPC?", _scpi_qOpc ),

    CMD_ITEM( "FIND", _scpi_find ),
    CMD_ITEM( "DEVICE?", _scpi_devices ),
    CMD_ITEM( "ROBOT?", _scpi_robots ),
    CMD_ITEM( "RESOURCE?", _scpi_rsrc ),

    CMD_ITEM( "DIAGNOSIS?", _scpi_diagnosis ),

    CMD_ITEM( "BUS:OPEN", _scpi_bus_open ),
    CMD_ITEM( "BUS:CLOSE", _scpi_bus_close ),

    CMD_ITEM( "DEVICE:STOP", _scpi_device_stop ),
    CMD_ITEM( "DEVICE:TERMINATE", _scpi_device_terminate ),
    CMD_ITEM( "DEVICE:RESET", _scpi_device_reset ),

    CMD_ITEM( "TO", _scpi_to ),

    CMD_ITEM( "rpc", _scpi_rpc ),
    CMD_ITEM( "rpq", _scpi_rpq ),
    CMD_ITEM( "rpk", _scpi_rpk ),
    CMD_ITEM( "rpls", _scpi_rpls ),
    CMD_ITEM( "rpgc", _scpi_rpgc ),

    CMD_ITEM( "TST", _scpi_tst ),
//    CMD_ITEM( "*LRN", _scpi_lrn ),      //! setupfile
//    CMD_ITEM( "HRST", _scpi_hrst ),

    SCPI_CMD_LIST_END
};

const void* MainWindow::loadScpiCmd()
{
    return _mrq_scpi_cmds;
}
