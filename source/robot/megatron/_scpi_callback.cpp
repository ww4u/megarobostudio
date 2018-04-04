#include <QtCore>

#include "scpi/scpi.h"

#include "megatron.h"
#include "../../com/comassist.h"

#define DEF_ROBO()      robotMegatron *pRobo;\
                        pRobo = ((robotMegatron*)context->user_context);\
                        Q_ASSERT( NULL != pRobo );


static scpi_result_t _scpi_idn( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    logDbg();

    QString str;

    str = ((robotMegatron*)context->user_context)->getName();

    logDbg()<<str;
    SCPI_ResultText( context, str.toLatin1().data() );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_run( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    pRobo->run();logDbg();

    return SCPI_RES_OK;
}

//! move x1,y1,z1, x2,y2,z2,
//! x1,y1,z1, x2,y2,z2,
//! t
static scpi_result_t _scpi_move( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax,page;
    float vals[6+6+1];

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { return SCPI_RES_ERR; }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { return SCPI_RES_ERR; }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { return SCPI_RES_ERR; }
    }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    { logDbg()<<vals[i]; }

    //! robo op
    DEF_ROBO();

    MegatronKeyPoint pt1( 0,
                          vals[0], vals[1], vals[2], vals[3], vals[4], vals[5]
                            );
    MegatronKeyPoint pt2( vals[12],
                          vals[6], vals[7], vals[8], vals[9], vals[10], vals[11]
                        );

    MegatronKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! ax, page, file
static scpi_result_t _scpi_program( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { return SCPI_RES_ERR; }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { return SCPI_RES_ERR; }

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { return SCPI_RES_ERR; }logDbg()<<strLen<<pLocalStr;
    if (strLen < 1)
    { return SCPI_RES_ERR; }

    //! x1,y1,z1,x2,y2,z2,t
    QList<float> dataset;
    int col = 7;
    if ( 0 != comAssist::loadDataset( pLocalStr, strLen, col, dataset ) )
    { return SCPI_RES_ERR; }

    //! point
    if ( dataset.size() / col < 2 )
    { return SCPI_RES_ERR; }

    MegatronKeyPointList curve;
    MegatronKeyPoint tp;
    for ( int i = 0; i < dataset.size()/col; i++ )
    {
        //! xyz,xyz
        for ( int j = 1; j < col; j++ )
        {
            tp.datas[j] = dataset.at( i * col + j-1);
        }
        //! t
        tp.datas[0] = dataset.at( i * col + col - 1 );

        curve.append( tp );
    }

    //! robo op
    DEF_ROBO();
    pRobo->program( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! page
static scpi_result_t _scpi_call( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { return SCPI_RES_ERR; }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { return SCPI_RES_ERR; }

    //! robo op
    DEF_ROBO();
    pRobo->call( tpvRegion( ax, page) );

    return SCPI_RES_OK;
}

//! int
static scpi_result_t _scpi_fsmState( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    int page, ax;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { return SCPI_RES_ERR; }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { return SCPI_RES_ERR; }

    int ret = pRobo->state( tpvRegion(ax,page) );

    SCPI_ResultInt32( context, ret );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_test1( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    pRobo->moveTest1();

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_test2( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    pRobo->moveTest2();

    return SCPI_RES_OK;
}

static scpi_command_t _scpi_cmds[]=
{

    CMD_ITEM( "*IDN?", _scpi_idn ),
    CMD_ITEM( "RUN",  _scpi_run ),
    CMD_ITEM( "MOVE", _scpi_move ),

    CMD_ITEM( "STATE?", _scpi_fsmState ),

    CMD_ITEM( "PROGRAM", _scpi_program ),
    CMD_ITEM( "CALL", _scpi_call ),

    CMD_ITEM( "TEST1", _scpi_test1 ),
    CMD_ITEM( "TEST2", _scpi_test2 ),

    SCPI_CMD_LIST_END
};

const void* robotMegatron::loadScpiCmd()
{
    return _scpi_cmds;
}

