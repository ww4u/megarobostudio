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

//    LOCAL_MGR()->on_actionReset_triggered();

    scpi_result_t ret;

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;

    QByteArray ary( pLocalStr, strLen );
    QString content = ary;

    LOCAL_MGR()->slot_com_receive( content );

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

    CMD_ITEM( "BUS:OPEN", _scpi_bus_open ),
    CMD_ITEM( "BUS:CLOSE", _scpi_bus_close ),

    CMD_ITEM( "DEVICE:STOP", _scpi_device_stop ),
    CMD_ITEM( "DEVICE:RESET", _scpi_device_reset ),

    CMD_ITEM( "TO", _scpi_to ),

//    CMD_ITEM( "*LRN", _scpi_lrn ),      //! setupfile
//    CMD_ITEM( "HRST", _scpi_hrst ),

    SCPI_CMD_LIST_END
};

const void* MainWindow::loadScpiCmd()
{
    return _mrq_scpi_cmds;
}
