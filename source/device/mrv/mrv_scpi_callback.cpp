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

static scpi_command_t _scpi_cmds[]=
{
    #include "../mrv_board/_MRV_scpi_cmd.h"

    CMD_ITEM( "*IDN?", _scpi_idn ),
//    CMD_ITEM( "*LRN", _scpi_lrn ),      //! setupfile
//    CMD_ITEM( "HRST", _scpi_hrst ),

//    CMD_ITEM( "SN", _scpi_sn ),
//    CMD_ITEM( "SN?", _scpi_qSn ),

//    CMD_ITEM( "TEST:ADD", _scpi_testAdd ),

//    CMD_ITEM( "RUN", _scpi_run ),
//    CMD_ITEM( "STOP", _scpi_stop ),

//    CMD_ITEM( "ROTATE", _scpi_rotate ),
//    CMD_ITEM( "MOVE", _scpi_rotate ),
//    CMD_ITEM( "PREMOVE", _scpi_preRotate ),

//    CMD_ITEM( "MOVEJ", _scpi_movej ),
//    CMD_ITEM( "PREMOVEJ", _scpi_preMovej ),

//    CMD_ITEM( "STATE?", _scpi_fsmState ),

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

//    CMD_ITEM( "WRITE", _scpi_arb_write ),
//    CMD_ITEM( "READ", _scpi_arb_read ),

    SCPI_CMD_LIST_END
};

namespace MegaDevice
{
const void* deviceMRV::loadScpiCmd()
{
    return _scpi_cmds;
}
}
