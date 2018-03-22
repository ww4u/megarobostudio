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

//! move x1,y1,z1,h1, x2,y2,z2,h2, t
static scpi_result_t _scpi_move( scpi_t * context )
{
    DEF_LOCAL_VAR();

    float vals[9];

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { return SCPI_RES_ERR; }
    }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    { logDbg()<<vals[i]; }

    //! robo op
    DEF_ROBO();

    TraceKeyPoint pt1( 0, vals[0], vals[1], vals[2], vals[3] );
    TraceKeyPoint pt2( vals[8], vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->move( curve );

    return SCPI_RES_OK;
}

//! file
static scpi_result_t _scpi_program( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { return SCPI_RES_ERR; }logDbg()<<strLen<<pLocalStr;
    if (strLen < 1)
    { return SCPI_RES_ERR; }

    //! t,x,y,z,h
    QList<float> dataset;
    int col = 5;
    if ( 0 != comAssist::loadDataset( pLocalStr, strLen, 5, dataset ) )
    { return SCPI_RES_ERR; }

    //! point
    if ( dataset.size() / col < 2 )
    { return SCPI_RES_ERR; }

    TraceKeyPointList curve;
    TraceKeyPoint tp;
    for ( int i = 0; i < dataset.size()/col; i++ )
    {
        for ( int j = 0; j < col; j++ )
        {
            tp.datas[j] = dataset.at( i * col + j);
        }

        curve.append( tp );
    }

    //! robo op
    DEF_ROBO();
    pRobo->move( curve );

    return SCPI_RES_OK;
}

//! int
static scpi_result_t _scpi_fsmState( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    int ret = pRobo->state();

    SCPI_ResultInt32( context, ret );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_pose( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    TraceKeyPoint pose;

    int ret = pRobo->nowPose( pose );
    if ( ret != 0 )
    { return SCPI_RES_ERR; }

    //! x,y,z,h
    SCPI_ResultInt32( context, pose.x );
    SCPI_ResultInt32( context, pose.y );
    SCPI_ResultInt32( context, pose.z );
    SCPI_ResultInt32( context, pose.hand );

    return SCPI_RES_OK;
}

//! a,b,c,d
static scpi_result_t _scpi_dist( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    //! 4 dists
    QList<float> dists;

    int ret = pRobo->nowDist( dists );
    if ( ret != 0 )
    { return SCPI_RES_ERR; }

    //! return the dist
    for ( int i = 0; i <dists.size(); i++ )
    {
        SCPI_ResultFloat( context, dists.at(i) );
    }

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
    CMD_ITEM( "POSE?", _scpi_pose ),
    CMD_ITEM( "DISTANCE?", _scpi_dist ),

    CMD_ITEM( "PROGRAM", _scpi_program ),

    CMD_ITEM( "TEST1", _scpi_test1 ),
    CMD_ITEM( "TEST2", _scpi_test2 ),

    SCPI_CMD_LIST_END
};

const void* robotMegatron::loadScpiCmd()
{
    return _scpi_cmds;
}

