#include <QtCore>

#include "scpi/scpi.h"

#include "h2.h"
#include "../../com/comassist.h"

#define DEF_ROBO()      robotH2 *pRobo;\
                        pRobo = ((robotH2*)context->user_context);\
                        Q_ASSERT( NULL != pRobo );

#define LOCAL_ROBO()    pRobo

#define CHECK_LINK()    if ( pRobo->checkLink() ) \
                        {}\
                        else\
                        { scpi_ret( SCPI_RES_ERR ); }

static scpi_result_t _scpi_idn( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    logDbg();

    QString str;

    str = ((robotH2*)context->user_context)->getName();

    logDbg()<<str;
    SCPI_ResultText( context, str.toLatin1().data() );

    return SCPI_RES_OK;
}

//! page
static scpi_result_t _scpi_run( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    LOCAL_ROBO()->run();logDbg();

    return SCPI_RES_OK;
}

//! move ch, page
//! x1,y1, x2,y2,
//! t
static scpi_result_t _scpi_move( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;
    float vals[4+1];

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    { logDbg()<<vals[i]; }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    H2KeyPoint pt1( 0,
                          vals[0], vals[1], 0
                            );
    H2KeyPoint pt2( vals[4],
                          vals[2], vals[3], 0
                        );

    H2KeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}


//! move ch, page
//! x1,y1, x2,y2,
//! t
static scpi_result_t _scpi_preMove( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;
    float vals[4+1];

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    { logDbg()<<vals[i]; }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    H2KeyPoint pt1( 0,
                          vals[0], vals[1], 0
                            );
    H2KeyPoint pt2( vals[4],
                          vals[2], vals[3], 0
                        );

    H2KeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->preMove( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! page, file
static scpi_result_t _scpi_program( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;
    if (strLen < 1)
    { scpi_ret( SCPI_RES_ERR ); }

    //! t, x1,y1,v
    QList<float> dataset;
    int col = 4;
    if ( 0 != comAssist::loadDataset( pLocalStr, strLen, col, dataset ) )
    {  scpi_ret( SCPI_RES_ERR ); }

    //! point
    if ( dataset.size() / col < 2 )
    { scpi_ret( SCPI_RES_ERR ); }

    H2KeyPointList curve;
    H2KeyPoint tp;
    for ( int i = 0; i < dataset.size()/col; i++ )
    {
        //! t
        tp.datas[0] = dataset.at( i * col );

        //! xyv
        for ( int j = 1; j < col; j++ )
        {
            tp.datas[j] = dataset.at( i * col + j );
        }

        curve.append( tp );
    }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    int ret = LOCAL_ROBO()->program( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! ax, page
static scpi_result_t _scpi_call( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    pRobo->call( tpvRegion( ax, page) );

    return SCPI_RES_OK;
}

//! ax, page
static scpi_result_t _scpi_fsmState( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    int ax, page;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    int ret = pRobo->state( tpvRegion(ax,page) );

    SCPI_ResultInt32( context, ret );

    return SCPI_RES_OK;
}

//! int jointid
static scpi_result_t _scpi_gozero( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    int joint;
logDbg();
    //! robo
    if ( SCPI_ParamInt32(context, &joint, true) != true )
    {logDbg();
        pRobo->goZero();
    }
    //! some joint
    else
    {logDbg();
        pRobo->goZero( joint, true );
    }

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_distance( scpi_t * context )
{
//    DEF_LOCAL_VAR();
//    DEF_ROBO();

//    CHECK_LINK();

//    int joint;

//    //! robo
//    if ( SCPI_ParamInt32(context, &joint, true) != true )
//    {
//        pRobo->goZero();
//    }
//    //! some joint
//    //! \todo by change
//    else
//    {
//        pRobo->goZero( joint, true );
//    }

    return SCPI_RES_OK;
}

//! ax
static scpi_result_t _scpi_angle( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    int joint;
    //! robo
    if ( SCPI_ParamInt32(context, &joint, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    //! some joint
    float fAng;
    if ( 0 != LOCAL_ROBO()->angle( joint, fAng ) )
    { scpi_ret( SCPI_RES_ERR ); }

    SCPI_ResultFloat( context, fAng );

    return SCPI_RES_OK;
}

//!
static scpi_result_t _scpi_pose( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    int ret;
    float x, y;
    ret = LOCAL_ROBO()->pose( x, y );

    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }
    else
    {
        SCPI_ResultFloat( context, x );
        SCPI_ResultFloat( context, y );
    }

    return SCPI_RES_OK;
}

//! float, float
static scpi_result_t _scpi_center( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    float vals[2];

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    LOCAL_ROBO()->setCenter( vals[0], vals[1] );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_qCenter( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    float x,y;
    LOCAL_ROBO()->center( x, y );

    SCPI_ResultFloat( context, x );
    SCPI_ResultFloat( context, y );

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

    CMD_ITEM( "MOVE", _scpi_move ),         //! ax,page, x1,y1,x2,y2,t
    CMD_ITEM( "PREMOVE", _scpi_preMove ),   //! ax,page, x1,y1,x2,y2,t

//    CMD_ITEM( "GOTO", _scpi_angle ),        //! x,y,t

    CMD_ITEM( "STATE?", _scpi_fsmState ),   //! ax,page

    CMD_ITEM( "PROGRAM", _scpi_program ),   //! ax,page,file
                                            //! t,x,y,v

    CMD_ITEM( "CALL", _scpi_call ),         //! ax,page

    CMD_ITEM( "ZERO", _scpi_gozero ),       //! jid
                                            //! or no para

    CMD_ITEM( "POSE?", _scpi_pose ),        //! center + absangle
                                            //! center default = (32.7,0)
    CMD_ITEM( "DISTANCE?", _scpi_distance ),
//    CMD_ITEM( "ABSANGLE?", _scpi_angle ),   //!
    CMD_ITEM( "ANGLE?", _scpi_angle ),

    CMD_ITEM( "CENTER", _scpi_center ),      //! set center
    CMD_ITEM( "CENTER?", _scpi_qCenter ),      //! set center

    //! todo downlaod

    CMD_ITEM( "TEST1", _scpi_test1 ),
    CMD_ITEM( "TEST2", _scpi_test2 ),

    SCPI_CMD_LIST_END
};

const void* robotH2::loadScpiCmd()
{
    return _scpi_cmds;
}

