#include <QtCore>

#include "scpi/scpi.h"

#include "igus_delta.h"
#include "../../com/comassist.h"

#define DEF_ROBO()      robotIgusDelta *pRobo;\
                        pRobo = ((robotIgusDelta*)context->user_context);\
                        Q_ASSERT( NULL != pRobo );
#define ROBO()          pRobo

#define LOCAL_ROBO()    pRobo

#define CHECK_LINK()    if ( pRobo->checkLink() ) \
                        {}\
                        else\
                        { scpi_ret( SCPI_RES_ERR ); }

static scpi_result_t _scpi_idn( scpi_t * context )
{
    DEF_LOCAL_VAR();

    DEF_ROBO();

    QString str;
    str = ROBO()->getName();

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

//! move ax, page,
//! x1,y1,z1, h1,
//! x1,y1,z1, h2,
//! t
static scpi_result_t _scpi_move( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax,page;
    float vals[4+4+1];

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

    TraceKeyPoint pt1( 0,
                          vals[0], vals[1], vals[2], vals[3]
                            );
    TraceKeyPoint pt2( vals[8],
                          vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    int ret;
    ret = pRobo->move( curve, tpvRegion(ax,page) );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    return SCPI_RES_OK;
}

//! move ax, page,
//! x1,y1,z1, h1,
//! x1,y1,z1, h2,
//! t
static scpi_result_t _scpi_preMove( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax,page;
    float vals[4+4+1];

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

    TraceKeyPoint pt1( 0,
                          vals[0], vals[1], vals[2], vals[3]
                            );
    TraceKeyPoint pt2( vals[8],
                          vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    int ret;
    ret = pRobo->preMove( curve, tpvRegion(ax,page) );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    return SCPI_RES_OK;
}

//! ax, page, file
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
    { scpi_ret( SCPI_RES_ERR ); }
    if (strLen < 1)
    { scpi_ret( SCPI_RES_ERR ); }

    //! find the input file
    QByteArray byteName( pLocalStr, strLen );
    QString fileInName( byteName );
    if ( comAssist::ammendFileName( fileInName ) )
    {}
    else
    { scpi_ret( SCPI_RES_ERR ); }

    //! get the file
    QList<float> dataset;
    int col = 6;
    QList<int> dataCols;
    dataCols<<0<<1<<2<<3<<4<<5;
    if ( 0 != comAssist::loadDataset( pLocalStr, strLen, col, dataCols, dataset ) )
    { scpi_ret( SCPI_RES_ERR ); }

    //! point
    if ( dataset.size() / col < 2 )
    { scpi_ret( SCPI_RES_ERR ); }

    TraceKeyPointList curve;
    TraceKeyPoint tp;
    for ( int i = 0; i < dataset.size()/col; i++ )
    {
        //! 0 1 2 3 4 5
        //! x,y,z,h,t,interp
        tp.t = dataset.at( i * col + 4 );

        tp.x = dataset.at( i * col + 0 );
        tp.y = dataset.at( i * col + 1 );
        tp.z = dataset.at( i * col + 2 );
        tp.hand = dataset.at( i * col + 3 );

        tp.iMask = dataset.at( i * col + 5 );

        curve.append( tp );

//        logDbg()<<tp.t<<tp.x<<tp.y<<tp.z<<tp.hand<<tp.iMask;
    }

    DEF_ROBO();

    CHECK_LINK();

    pRobo->program( curve, tpvRegion( ax, page) );

    return SCPI_RES_OK;
}

//! ax, page, file
//! file is tpv for each axes
static scpi_result_t _scpi_download( scpi_t * context )
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

    DEF_ROBO();

    CHECK_LINK();

    QByteArray fileName( pLocalStr, strLen );
    int ret;
    ret = pRobo->program( fileName, tpvRegion( ax, page) );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    return SCPI_RES_OK;
}

//! ax,page, cycle, motionMode
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
static scpi_result_t _scpi_zero( scpi_t * context )
{
    DEF_LOCAL_VAR();

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    int ax, page;
    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    QList<int> jList;
    jList<<0<<1<<2;
    LOCAL_ROBO()->goZero( tpvRegion( ax, page ),
                          jList, LOCAL_ROBO()->jointZeroCcwList() );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_pose( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    float xyzs[3];
    int ret;
    ret = pRobo->getPOSE( xyzs );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    //! pose out
    SCPI_ResultFloat( context, xyzs[0] );
    SCPI_ResultFloat( context, xyzs[1] );
    SCPI_ResultFloat( context, xyzs[2] );

    return SCPI_RES_OK;
}

//! int
static scpi_result_t _scpi_fsmState( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    int page, ax;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    int ret = pRobo->state( tpvRegion(ax,page) );

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
    CMD_ITEM( "PREMOVE", _scpi_preMove ),

    CMD_ITEM( "STATE?", _scpi_fsmState ),

    CMD_ITEM( "PROGRAM", _scpi_program ),
    CMD_ITEM( "DOWNLOAD", _scpi_download ),
    CMD_ITEM( "CALL", _scpi_call ),

    CMD_ITEM( "CENTER", _scpi_zero ),

    CMD_ITEM( "POSE?", _scpi_pose ),    //! x,y,z

    SCPI_CMD_LIST_END
};

const void* robotIgusDelta::loadScpiCmd()
{
    return _scpi_cmds;
}

