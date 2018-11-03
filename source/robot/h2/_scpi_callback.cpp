#include <QtCore>

#include "scpi/scpi.h"

#include "h2.h"
#include "../../com/comassist.h"
#include "../../com/scpiassist.h"

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
    DEF_LOCAL_VAR();

    logDbg();

    QString str;

    str = ((robotH2*)context->user_context)->getName();

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

    LOCAL_ROBO()->run( tpvRegion(ax,page) );

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
    pSec = dataSet.tryLoad( file,LOCAL_ROBO()->getClass(), headerlist("t/x/y") );

    if ( NULL == pSec )
    { scpi_ret( SCPI_RES_ERR ); }
    else
    {}

    deparse_column_index( enable, "enable" );
    deparse_column_index( t, "t" );
    deparse_column_index( x, "x" );
    deparse_column_index( y, "y" );

    //! deload
    H2KeyPointList curve;
    H2KeyPoint tp;
    bool bEn;
    for ( int i = 0; i < pSec->rows(); i++ )
    {
        //! disabled
        if ( pSec->cellValue( i, c_enable, bEn, true, true ) && !bEn )
        { continue; }

        if ( !pSec->cellValue( i, c_t, tp.t, 0, false ) )
        { continue; }

        if ( !pSec->cellValue( i, c_x, tp.x, 0, false ) )
        { continue; }

        if ( !pSec->cellValue( i, c_y, tp.y, 0, false ) )
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
}

//! page, file
static scpi_result_t __scpi_program( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    int ax, page;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if (strLen < 1)
    { scpi_ret( SCPI_RES_ERR ); }

    QList<float> dataset;
    int ret;
    int col = 6;
    QList<int> dataCols;
    do
    {
        //! enable,name,t,x,y,comment
        col = 6;
        dataCols.clear();
        dataCols<<2<<3<<4;
        dataset.clear();
        ret = comAssist::loadDataset( pLocalStr, strLen, col, dataCols, dataset );
        if ( 0 == ret && ( dataset.size() / col ) > 1  )
        { break; }

        //! t, x1,y1,v
        col = 4;
        dataCols.clear();
        dataCols<<0<<1<<2;
        dataset.clear();
        ret = comAssist::loadDataset( pLocalStr, strLen, col, dataCols, dataset );
        if ( 0 == ret && ( dataset.size() / col ) > 1  )
        { break; }

        //! t, x, y
        col = 3;
        dataCols.clear();
        dataCols<<0<<1<<2;
        dataset.clear();
        ret = comAssist::loadDataset( pLocalStr, strLen, col, dataCols, dataset );
        if ( 0 == ret && ( dataset.size() / col ) > 1  )
        { break; }

    }while( 0 );

    //! point
    if ( dataset.size() / col < 2 )
    { scpi_ret( SCPI_RES_ERR ); }

    H2KeyPointList curve;
    H2KeyPoint tp;
    for ( int i = 0; i < dataset.size()/col; i++ )
    {
        //! t
        tp.datas[0] = dataset.at( i * col + dataCols[0] );

        tp.datas[1] = dataset.at( i * col + dataCols[1] );
        tp.datas[2] = dataset.at( i * col + dataCols[2] );

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

//! ax, page, cycle, motionMode
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

//! int ax, page, jointid
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

    //! robo
    int joint;
    if ( SCPI_ParamInt32(context, &joint, true) != true )
    {
        pRobo->goZero( tpvRegion(ax,page) );
    }
    //! some joint
    else
    {
        pRobo->goZero( tpvRegion(ax,page), joint, true );
    }

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
    COM_ITEMs(),

    CMD_ITEM( "*IDN?", _scpi_idn ),
    CMD_ITEM( "RUN",  _scpi_run ),
    CMD_ITEM( "SYNC", _scpi_sync ),

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
    CMD_ITEM( "ANGLE?", _scpi_angle ),

    CMD_ITEM( "CENTER", _scpi_center ),      //! set center
    CMD_ITEM( "CENTER?", _scpi_qCenter ),      //! set center

    CMD_ITEM( "TEST1", _scpi_test1 ),
    CMD_ITEM( "TEST2", _scpi_test2 ),

    SCPI_CMD_LIST_END
};

const void* robotH2::loadScpiCmd()
{
    return _scpi_cmds;
}

