#include <QtCore>

#include "scpi/scpi.h"

#include "h2z.h"
#include "../../com/comassist.h"

#define DEF_ROBO()      robotH2Z *pRobo;\
                        pRobo = ((robotH2Z*)context->user_context);\
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

    DEF_ROBO();

    str = LOCAL_ROBO()->getName();

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
//! x1,y1,z1, x2,y2,z2,
//! t
static scpi_result_t _scpi_move( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;
    float vals[6+1];

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

    H2ZKeyPoint pt1( 0,
                          vals[0], vals[1], vals[2],
                          0
                            );
    H2ZKeyPoint pt2( vals[6],
                          vals[3], vals[4], vals[5],
                          0
                        );

    H2ZKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}


//! move ch, page
//! x1,y1,z1, x2,y2,z2,
//! t
static scpi_result_t _scpi_preMove( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;
    float vals[6+1];

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

    H2ZKeyPoint pt1( 0,
                          vals[0], vals[1], vals[2],
                          0
                            );
    H2ZKeyPoint pt2( vals[6],
                          vals[3], vals[4], vals[5],
                          0
                        );

    H2ZKeyPointList curve;
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


    QList<float> dataset;
    int col = 6;
    QList<int> dataCols;
    QList<int> seqList;
    int ret = 0;

    do
    {
        //! #!enable,name,t,x,y,z,comment
        col = 7;
        dataCols<<2<<3<<4<<5;
        seqList.clear();
        seqList<<2<<3<<4<<5;
        dataset.clear();

        ret = comAssist::loadDataset( pLocalStr, strLen, col, dataCols, dataset );
        if ( 0 == ret && ( dataset.size() / col ) > 1  )
        { break; }

        //! x,y,z,h,t,interp
        col = 6;
        dataCols<<0<<1<<2<<4;
        seqList.clear();
        seqList<<4<<0<<1<<2;
        dataset.clear();
        ret = comAssist::loadDataset( pLocalStr, strLen, col, dataCols, dataset );
        if ( 0 == ret && ( dataset.size() / col ) > 1  )
        { break; }

    }while( 0 );

    //! point
    if ( dataset.size() / col < 2 )
    { scpi_ret( SCPI_RES_ERR ); }

    H2ZKeyPointList curve;
    H2ZKeyPoint tp;
    for ( int i = 0; i < dataset.size()/col; i++ )
    {
        //! t
        tp.datas[0] = dataset.at( i * col + seqList[0] );

//        tp.datas[1] = dataset.at( i * col + 0 );
//        tp.datas[2] = dataset.at( i * col + 1 );
//        tp.datas[3] = dataset.at( i * col + 2 );
        for ( int k = 1; k < 4; k++ )
        {
            tp.datas[k] = dataset.at( i * col + seqList[k]  );
        }

        curve.append( tp );
    }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    ret = LOCAL_ROBO()->program( curve, tpvRegion(ax,page) );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

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

//!int ax, page, jointid
static scpi_result_t _scpi_gozero( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    int ax, page;
    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    int joint;
logDbg();
    //! robo
    if ( SCPI_ParamInt32(context, &joint, true) != true )
    {logDbg();
        pRobo->goZero( tpvRegion(ax,page) );
    }
    //! some joint
    else
    {logDbg();
        pRobo->goZero( tpvRegion(ax,page), joint, true );
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
        SCPI_ResultFloat( context, 0 );
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

const void* robotH2Z::loadScpiCmd()
{
    return _scpi_cmds;
}

