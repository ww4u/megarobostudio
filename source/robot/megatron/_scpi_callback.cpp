#include <QtCore>

#include "scpi/scpi.h"

#include "megatron.h"
#include "../../com/comassist.h"
#include "../../com/scpiassist.h"

#define DEF_ROBO()      robotMegatron *pRobo;\
                        pRobo = ((robotMegatron*)context->user_context);\
                        Q_ASSERT( NULL != pRobo );

#define LOCAL_ROBO()    pRobo

#define CHECK_LINK()    if ( pRobo->checkLink() ) \
                        {}\
                        else\
                        { scpi_ret( SCPI_RES_ERR ); }

static scpi_result_t _scpi_idn( scpi_t * context )
{
    DEF_LOCAL_VAR();

    logDbg();

    QString str;

    str = ((robotMegatron*)context->user_context)->getName();

    SCPI_ResultText( context, str.toLatin1().data() );

    return SCPI_RES_OK;
}

//! ax, page
static scpi_result_t _scpi_run( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    int ax, page;
    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    CHECK_LINK();

    pRobo->run( tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! ax, page
static scpi_result_t _scpi_sync( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    int ax, page;
    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    CHECK_LINK();

    LOCAL_ROBO()->sync( tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! move ax, page
//! ( fx, ly, fz, bx, ry, bz )
//! ( fx, ly, fz, bx, ry, bz )
//! t
static scpi_result_t _scpi_move( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax,page;
    float vals[6+6+1];

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

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

//! premove ax, page
//! ( fx, ly, fz, bx, ry, bz )
//! ( fx, ly, fz, bx, ry, bz )
//! t
static scpi_result_t _scpi_preMove( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax,page;
    float vals[6+6+1];

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    MegatronKeyPoint pt1( 0,
                          vals[0], vals[1], vals[2], vals[3], vals[4], vals[5]
                            );
    MegatronKeyPoint pt2( vals[12],
                          vals[6], vals[7], vals[8], vals[9], vals[10], vals[11]
                        );

    MegatronKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->preMove( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! int ax, int page
//! int ccw, int jid
//!
static scpi_result_t _scpi_gozero( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    //! ax, page
    int ax, page;
    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    int joint, ccw;
    //! ccw
    if ( SCPI_ParamInt32(context, &ccw, true) != true )
    {
        pRobo->goZero( tpvRegion(ax,page) );

        return SCPI_RES_OK;
    }

    //! robo
    if ( SCPI_ParamInt32(context, &joint, true) != true )
    {
        scpi_ret( SCPI_RES_ERR );
    }
    //! some joint
    else
    {
        pRobo->goZero( tpvRegion(ax,page), joint, ccw > 0 );
    }

    return SCPI_RES_OK;
}

//! ax, page, file
static scpi_result_t _scpi_program( scpi_t * context )
{
    DEF_LOCAL_VAR();

    //! para
    int ax, page;
    QString file;

    if ( deload_ax_page_file( context, ax, page, file) == SCPI_RES_OK )
    {}
    else
    { scpi_ret( SCPI_RES_ERR ); }

    //! data set
    MDataSet dataSet;
    DEF_ROBO();

    MDataSection *pSec;
    pSec = dataSet.tryLoad( file,LOCAL_ROBO()->getClass(), headerlist("t/fx/ly/fz/bx/ry/bz") );

    if ( NULL == pSec )
    { scpi_ret( SCPI_RES_ERR ); }
    else
    {}

    deparse_column_index( enable, "enable" );
    deparse_column_index( t, "t" );
    deparse_column_index( fx, "fx" );
    deparse_column_index( ly, "ly" );
    deparse_column_index( fz, "fz" );
    deparse_column_index( bx, "bx" );
    deparse_column_index( ry, "ry" );
    deparse_column_index( bz, "bz" );

    //! deload
    MegatronKeyPointList curve;
    MegatronKeyPoint tp;
    bool bEn;
    for ( int i = 0; i < pSec->rows(); i++ )
    {
        //! disabled
        if ( pSec->cellValue( i, c_enable, bEn, true, true ) && !bEn )
        { continue; }

        if ( !pSec->cellValue( i, c_t, tp.t, 0, false ) )
        { continue; }

        if ( !pSec->cellValue( i, c_fx, tp.fx, 0, false ) )
        { continue; }
        if ( !pSec->cellValue( i, c_ly, tp.ly, 0, false ) )
        { continue; }
        if ( !pSec->cellValue( i, c_fz, tp.fz, 0, false ) )
        { continue; }

        if ( !pSec->cellValue( i, c_bx, tp.bx, 0, false ) )
        { continue; }
        if ( !pSec->cellValue( i, c_ry, tp.ry, 0, false ) )
        { continue; }
        if ( !pSec->cellValue( i, c_bz, tp.bz, 0, false ) )
        { continue; }

        curve.append( tp );
    }

    //! check curve
    if ( curve.size() < 2 )
    { scpi_ret( SCPI_RES_ERR ); }

    CHECK_LINK();

    if ( 0 != LOCAL_ROBO()->program( curve, tpvRegion( ax, page) ) )
    { scpi_ret( SCPI_RES_ERR ); }

    return SCPI_RES_OK;

//    // read
//    DEF_LOCAL_VAR();

//    int ax, page;

//    if ( SCPI_ParamInt32(context, &ax, true) != true )
//    { scpi_ret( SCPI_RES_ERR ); }

//    if ( SCPI_ParamInt32(context, &page, true) != true )
//    { scpi_ret( SCPI_RES_ERR ); }

//    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
//    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;
//    if (strLen < 1)
//    { scpi_ret( SCPI_RES_ERR ); }

//    QList<float> dataset;
//    int col = 7;
//    QList<int> dataCols;
//    dataCols<<0<<1<<2<<3<<4<<5<<6;
//    if ( 0 != comAssist::loadDataset( pLocalStr, strLen, col, dataCols, dataset ) )
//    { scpi_ret( SCPI_RES_ERR ); }

//    //! point
//    if ( dataset.size() / col < 2 )
//    { scpi_ret( SCPI_RES_ERR ); }

//    MegatronKeyPointList curve;
//    MegatronKeyPoint tp;
//    for ( int i = 0; i < dataset.size()/col; i++ )
//    {
//        //! fx ly fz bx ry bz t
//        tp.fx = dataset.at( i * col + 0 );
//        tp.ly = dataset.at( i * col + 1 );
//        tp.fz = dataset.at( i * col + 2 );

//        tp.bx = dataset.at( i * col + 3 );
//        tp.ry = dataset.at( i * col + 4 );
//        tp.bz = dataset.at( i * col + 5 );

//        tp.t = dataset.at( i * col + 6 );

//        curve.append( tp );
//    }

//    //! robo op
//    DEF_ROBO();

//    CHECK_LINK();

//    pRobo->program( curve, tpvRegion(ax,page) );

//    return SCPI_RES_OK;
}

//! ax,page,cycle, motionMode
static scpi_result_t _scpi_call( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page, cycle, motionMode;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    cycle = 1;
    motionMode = -1;
    do
    {
        if ( SCPI_ParamInt32(context, &cycle, true) != true )
        { break; }

        if ( SCPI_ParamInt32(context, &motionMode, true) != true )
        { break; }
    }while( 0 );

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    pRobo->call( cycle, tpvRegion( ax, page, motionMode ) );

    return SCPI_RES_OK;
}

//! int
static scpi_result_t _scpi_fsmState( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    int page, ax;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

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
    COM_ITEMs(),

    CMD_ITEM( "*IDN?", _scpi_idn ),
    CMD_ITEM( "RUN",  _scpi_run ),
    CMD_ITEM( "SYNC", _scpi_sync ),

    CMD_ITEM( "MOVE", _scpi_move ),
    CMD_ITEM( "PREMOVE", _scpi_preMove ),   //! only calc, no move

    CMD_ITEM( "ZERO", _scpi_gozero ),       //! jid
                                            //! or no para

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

