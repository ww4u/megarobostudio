#include <QtCore>

#include "scpi/scpi.h"

#include "motor.h"
#include "../../com/comassist.h"
#include "../../com/scpiassist.h"

#define DEF_ROBO()      robotMotor *pRobo;\
                        pRobo = ((robotMotor*)context->user_context);\
                        Q_ASSERT( NULL != pRobo );

#define LOCAL_ROBO()    pRobo

#define CHECK_LINK()    if ( pRobo->checkLink() ) \
                        {}\
                        else\
                        { scpi_ret( SCPI_RES_ERR ); }

static scpi_result_t _scpi_idn( scpi_t * context )
{
    DEF_LOCAL_VAR();

    QString str;
    DEF_ROBO();
    str = LOCAL_ROBO()->getName();

    logDbg()<<str;
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

    LOCAL_ROBO()->run( tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

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
//! p,t
static scpi_result_t _scpi_step( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;
    float vals[2];

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

    D1Point pt1( 0, 0 );
    D1Point pt2( vals[1], vals[0] );

    D1PointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    LOCAL_ROBO()->move( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! move ch, page
//! p1,p2
//! t
static scpi_result_t _scpi_move( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;
    float vals[2+1];

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

    D1Point pt1( 0,
                          vals[0]
                            );
    D1Point pt2( vals[2],
                          vals[1]
                        );

    D1PointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    LOCAL_ROBO()->move( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! move ch, page
//! p1,p2
//! t
static scpi_result_t _scpi_preMove( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;
    float vals[2+1];

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

    D1Point pt1( 0,
                          vals[0]
                            );
    D1Point pt2( vals[2],
                          vals[1]
                        );

    D1PointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    LOCAL_ROBO()->preMove( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! page, file, motionMode
static scpi_result_t _scpi_program( scpi_t * context )
{
    DEF_LOCAL_VAR();

    DEF_ROBO();

    //! para
    int ax, page;
    QString file;

    if ( deload_ax_page_file( context, ax, page, file) == SCPI_RES_OK )
    {}
    else
    { scpi_ret( SCPI_RES_ERR ); }

    //! data set
    MDataSet dataSet;

    MDataSection *pSec;
    pSec = dataSet.tryLoad( file, "", headerlist("t/p") );

    if ( NULL == pSec )
    { scpi_ret( SCPI_RES_ERR ); }
    else
    {}

    deparse_column_index( enable, "enable" );
    deparse_column_index( t, "t" );
    deparse_column_index( p, "p" );
    deparse_column_index( v, "v" );

    //! deload
    D1PointList curve;
    D1Point tp;
    bool bEn;
    for ( int i = 0; i < pSec->rows(); i++ )
    {
        //! disabled
        if ( pSec->cellValue( i, c_enable, bEn, true, true ) && !bEn )
        { continue; }

        if ( !pSec->cellValue( i, c_t, tp.t, 0, false ) )
        { continue; }

        if ( !pSec->cellValue( i, c_p, tp.p, 0, false ) )
        { continue; }

        pSec->cellValue( i, c_v, tp.v, 0, true );

        curve.append( tp );
    }

    //! check curve
    if ( curve.size() < 2 )
    { scpi_ret( SCPI_RES_ERR ); }

    CHECK_LINK( );

    if ( 0 != LOCAL_ROBO()->program( curve, tpvRegion(ax,page) ) )
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

//    //! check motion mode
//    int motionMode = -1;
//    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
//    { motionMode = -1; }
//    else
//    {}

//    //! t, p, v
//    QList<float> dataset;
//    int col = 3;
//    QList<int> dataCols;
//    dataCols<<0<<1<<2;
//    if ( 0 != comAssist::loadDataset( pLocalStr, strLen, col, dataCols, dataset ) )
//    {  scpi_ret( SCPI_RES_ERR ); }

//    //! point
//    if ( dataset.size() / col < 2 )
//    { scpi_ret( SCPI_RES_ERR ); }

//    D1PointList curve;
//    D1Point tp;
//    for ( int i = 0; i < dataset.size()/col; i++ )
//    {
//        //! t
//        tp.t = dataset.at( i * col + 0 );
//        tp.p = dataset.at( i * col + 1 );
//        tp.v = dataset.at( i * col + 2 );

//        curve.append( tp );
//    }

//    //! robo op
//    DEF_ROBO();

//    CHECK_LINK();

//    int ret = LOCAL_ROBO()->program( curve, tpvRegion(ax,page,motionMode) );

//    return ret == 0 ? SCPI_RES_OK : SCPI_RES_ERR;
}

//! ax, page, cycle, motionMode
static scpi_result_t _scpi_call( scpi_t * context )
{
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
    LOCAL_ROBO()->call( cycle, tpvRegion( ax, page, motionMode ) );

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

    //! robo
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

    int ret = LOCAL_ROBO()->state( tpvRegion(ax,page) );

    SCPI_ResultInt32( context, ret );

    return SCPI_RES_OK;
}

static scpi_command_t _scpi_cmds[]=
{
    COM_ITEMs(),

    CMD_ITEM( "*IDN?", _scpi_idn ),
    CMD_ITEM( "RUN",  _scpi_run ),
    CMD_ITEM( "SYNC", _scpi_sync ),

    CMD_ITEM( "MOVE", _scpi_move ),
    CMD_ITEM( "PREMOVE", _scpi_preMove ),   //! ax,page, x1,x2,t

    CMD_ITEM( "STEP", _scpi_step ),         //! ax,page, dp, dt

    CMD_ITEM( "STATE?", _scpi_fsmState ),

    CMD_ITEM( "PROGRAM", _scpi_program ),
    CMD_ITEM( "CALL", _scpi_call ),

    CMD_ITEM( "ZERO", _scpi_gozero ),       //! jid

    SCPI_CMD_LIST_END
};

const void* robotMotor::loadScpiCmd()
{
    return _scpi_cmds;
}

