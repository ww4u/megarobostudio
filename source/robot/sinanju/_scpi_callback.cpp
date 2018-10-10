#include <QtCore>

#include "scpi/scpi.h"

#include "sinanju.h"
#include "../../com/comassist.h"

#define DEF_ROBO()      robotSinanju *pRobo;\
                        pRobo = ((robotSinanju*)context->user_context);\
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

    str = ((robotSinanju*)context->user_context)->getName();

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

    pRobo->run( tpvRegion(ax,page) );logDbg();

    return SCPI_RES_OK;
}

//! ax, page
static scpi_result_t _scpi_stop( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    int ax, page;
    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    CHECK_LINK();

    pRobo->stop( tpvRegion(ax,page) );

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

    pRobo->sync( tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! move ch,page, motionMode,
//! x1,y1,z1,h1, x2,y2,z2,h2, t
static scpi_result_t _scpi_move( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    float vals[9];

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    //! default interp
    TraceKeyPoint pt1( 0, vals[0], vals[1], vals[2], vals[3] );
    TraceKeyPoint pt2( vals[8], vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    CHECK_LINK();

    pRobo->move( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! premove ch,page, motionMode,
//! x1,y1,z1,h1, x2,y2,z2,h2, t
static scpi_result_t _scpi_premove( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    float vals[9];

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    //! default interp
    TraceKeyPoint pt1( 0, vals[0], vals[1], vals[2], vals[3] );
    TraceKeyPoint pt2( vals[8], vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    CHECK_LINK();

    pRobo->program( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! movej ch,page, motionMode,
//! x1,y1,z1,h1, x2,y2,z2,h2, t, j, tj
//! t : line time
//! tj : jump time
//! tj + t + tj
//! | ------ |
//! |        |
//! rectangle
static scpi_result_t _scpi_mover( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    float vals[ 4 * 2 + 3 ];

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    //! default interp
    TraceKeyPoint pt1( 0, vals[0], vals[1], vals[2], vals[3] );
    TraceKeyPoint pt1z( vals[10], vals[0], vals[1], vals[2] + vals[9], vals[3]   );
    TraceKeyPoint pt2z( vals[8] + vals[10], vals[4], vals[5], vals[6] + vals[9], vals[7]  );
    TraceKeyPoint pt2( vals[8] + vals[10]*2, vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt1z );
    curve.append( pt2z );
    curve.append( pt2 );

    CHECK_LINK();

    pRobo->move( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! premovej ch,page, motionMode
//! x1,y1,z1,h1, x2,y2,z2,h2, t, j
static scpi_result_t _scpi_premover( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    float vals[ 4 * 2 + 3 ];

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    //! default interp
    TraceKeyPoint pt1( 0, vals[0], vals[1], vals[2], vals[3] );
    TraceKeyPoint pt1z( vals[10], vals[0], vals[1], vals[2] + vals[9], vals[3]   );
    TraceKeyPoint pt2z( vals[8] + vals[10], vals[4], vals[5], vals[6] + vals[9], vals[7]  );
    TraceKeyPoint pt2( vals[8] + vals[10]*2, vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt1z );
    curve.append( pt2z );
    curve.append( pt2 );

    CHECK_LINK();

    pRobo->program( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! movej ch,page, motionMode
//! x1,y1,z1,h1, x2,y2,z2,h2, xl,yl,zl,hl, t
//! triangle
static scpi_result_t _scpi_movet( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    float vals[4*3+1];

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    //! default interp
    TraceKeyPoint pt1( 0, vals[0], vals[1], vals[2], vals[3] );
    TraceKeyPoint ptl( vals[12]/2, vals[8], vals[9], vals[10], vals[11] );
    TraceKeyPoint pt2( vals[12], vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( ptl );
    curve.append( pt2 );

    CHECK_LINK();

    pRobo->move( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! premovej ch,page, motionMode
//! x1,y1,z1,h1, x2,y2,z2,h2, xl,yl,zl,hl, t
static scpi_result_t _scpi_premovet( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    float vals[4*3+1];

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    //! default interp
    TraceKeyPoint pt1( 0, vals[0], vals[1], vals[2], vals[3] );
    TraceKeyPoint ptl( vals[12]/2, vals[8], vals[9], vals[10], vals[11] );
    TraceKeyPoint pt2( vals[12], vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( ptl );
    curve.append( pt2 );

    CHECK_LINK();

    pRobo->program( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! goto ax,page,motionMode,
//! x1,y1,z1,t
static scpi_result_t _scpi_goto( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    float vals[4];

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    //! current
    TraceKeyPoint pose;
    int ret = pRobo->nowPose( pose );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    //! default interp
    TraceKeyPoint pt1( 0, pose.x, pose.y, pose.z, 0 );
    TraceKeyPoint pt2( vals[3], vals[0], vals[1], vals[2], 0 );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! goto ax,page,motionMode
//! x1,y1,z1,t, j, tj
static scpi_result_t _scpi_gotor( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    float vals[4+2];

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    //! current
    TraceKeyPoint pose;
    int ret = pRobo->nowPose( pose );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    //! default interp
    TraceKeyPoint pt1( 0, pose.x, pose.y, pose.z, 0 );
    TraceKeyPoint pt1z( vals[5], pose.x, pose.y, pose.z+vals[4], 0 );
    TraceKeyPoint pt2z( vals[3] + vals[5], vals[0], vals[1], vals[2] + vals[4], 0 );
    TraceKeyPoint pt2( vals[3] + 2 * vals[5], vals[0], vals[1], vals[2], 0 );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt1z );
    curve.append( pt2z );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! goto ax,page,motionMode
//! x1,y1,z1,t, xl,yl,zl
static scpi_result_t _scpi_gotot( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    float vals[4+3];

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    //! current
    TraceKeyPoint pose;
    int ret = pRobo->nowPose( pose );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    //! default interp
    TraceKeyPoint pt1( 0, pose.x, pose.y, pose.z, 0 );
    TraceKeyPoint ptl( vals[3]/2, vals[4], vals[5], vals[6], 0 );
    TraceKeyPoint pt2( vals[3], vals[0], vals[1], vals[2], 0 );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( ptl );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! ax, page, mode, dx, dt
static scpi_result_t _scpi_stepx( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    float vals[2];

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    //! current
    TraceKeyPoint pose;
    int ret = pRobo->nowPose( pose );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    //! default interp
    TraceKeyPoint pt1( 0, pose.x, pose.y, pose.z, 0 );
    TraceKeyPoint pt2( vals[1], pose.x + vals[0], pose.y, pose.z );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! ax, page, mode, dy, dt
static scpi_result_t _scpi_stepy( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    float vals[2];

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

    //! current
    TraceKeyPoint pose;
    int ret = pRobo->nowPose( pose );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    //! default interp
    TraceKeyPoint pt1( 0, pose.x, pose.y, pose.z, 0 );
    TraceKeyPoint pt2( vals[1], pose.x, pose.y + vals[0], pose.z );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! ax, page, mode, dz, dt
static scpi_result_t _scpi_stepz( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    float vals[2];

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    //! current
    TraceKeyPoint pose;
    int ret = pRobo->nowPose( pose );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    //! default interp
    TraceKeyPoint pt1( 0, pose.x, pose.y, pose.z, 0 );
    TraceKeyPoint pt2( vals[1], pose.x, pose.y, pose.z + vals[0] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! ax, page, mode, dh, dt
static scpi_result_t _scpi_steph( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page, motionMode;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    float vals[2];

    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    //! robo op
    DEF_ROBO();

    CHECK_LINK();

    //! current
    TraceKeyPoint pose;
    int ret = pRobo->nowPose( pose );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    //! default interp
    TraceKeyPoint pt1( 0, pose.x, pose.y, pose.z, 0 );
    TraceKeyPoint pt2( vals[1], pose.x, pose.y, pose.z, vals[0] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(ax,page,motionMode) );

    return SCPI_RES_OK;
}

//! ax, page, file, motionMode = -1
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

    //! check motion mode
    int motionMode = -1;
    if ( SCPI_ParamInt32(context, &motionMode, true) != true )
    { motionMode = -1; }
    else
    {}

    QList<float> dataset;
    //! try .mc
    int col = 0;
    //! t,x,y,z,h,imask
    QList<int> seqList;
    QList<int> dataCols;
    int ret;
    do
    {
        //! en,name,t,x,y,z,h,mode,comment
        col = 9;
        dataset.clear();
        dataCols.clear();
        dataCols<<2<<3<<4<<5<<6<<7;     //! \note some string
        ret = comAssist::loadDataset( pLocalStr, strLen, col, dataCols, dataset );
        if ( 0 == ret && ( dataset.size() / col ) > 1  )
        {
            seqList<<2<<3<<4<<5<<6<<7;
            break;
        }

        //! x,y,z,h,t,interp
        col = 6;
        dataset.clear();
        dataCols.clear();
        dataCols<<0<<1<<2<<3<<4<<5;
        if ( 0 == comAssist::loadDataset( pLocalStr, strLen, col, dataCols, dataset ) )
        {
            seqList<<4<<0<<1<<2<<3<<5;
            break;
        }

        scpi_ret( SCPI_RES_ERR );
    }while ( 0 );

    //! point
    if ( dataset.size() / col < 2 )
    { scpi_ret( SCPI_RES_ERR ); }

    TraceKeyPointList curve;
    TraceKeyPoint tp;

    Q_ASSERT( seqList.size() == 6 );
    for ( int i = 0; i < dataset.size()/col; i++ )
    {
        //! 0 1 2 3 4 5
        //! x,y,z,h,t,interp
        tp.t = dataset.at( i * col + seqList.at(0) );

        tp.x = dataset.at( i * col + seqList.at(1) );
        tp.y = dataset.at( i * col + seqList.at(2) );
        tp.z = dataset.at( i * col + seqList.at(3) );
        tp.hand = dataset.at( i * col + seqList.at(4) );

        tp.iMask = dataset.at( i * col + seqList.at(5) );

        curve.append( tp );

//        logDbg()<<tp.t<<tp.x<<tp.y<<tp.z<<tp.hand<<tp.iMask;
    }

    DEF_ROBO();

    CHECK_LINK();

    pRobo->program( curve, tpvRegion( ax, page, motionMode) );

    return SCPI_RES_OK;
}

//! ax, page, file
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


//! ax, page, cycle, motinMode
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

//! ax,page
static scpi_result_t _scpi_fsmState( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    int ax, page;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    CHECK_LINK();

    int ret = pRobo->state( tpvRegion(ax,page) );

    SCPI_ResultInt32( context, ret );

    return SCPI_RES_OK;
}

static scpi_result_t _scpi_pose( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    TraceKeyPoint pose;

    int ret = pRobo->nowPose( pose );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    //! x,y,z,h
    SCPI_ResultFloat( context, pose.x );
    SCPI_ResultFloat( context, pose.y );
    SCPI_ResultFloat( context, pose.z );
    SCPI_ResultFloat( context, pose.hand );

    return SCPI_RES_OK;
}

//! a,b,c,d
static scpi_result_t _scpi_dist( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    //! 4 dists
    QList<float> dists;

    int ret = pRobo->nowDist( dists );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    //! return the dist
    for ( int i = 0; i <dists.size(); i++ )
    {
        SCPI_ResultFloat( context, dists.at(i) );
    }

    return SCPI_RES_OK;
}

//! int page
static scpi_result_t _scpi_test1( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    int page;

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    pRobo->moveTest1( tpvRegion(0,page) );

    return SCPI_RES_OK;
}

//! int page
static scpi_result_t _scpi_test2( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    int page;

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    pRobo->moveTest2( tpvRegion(0,page) );

    return SCPI_RES_OK;
}

//! int ax, page
//! int jointid
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
        pRobo->goZero( tpvRegion(ax,page),
                       joint, true );
    }

    return SCPI_RES_OK;
}


static scpi_result_t _scpi_fold( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    int ax, page;

    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamInt32(context, &page, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    pRobo->goFactory( tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! four rad
//! ref the the zero
//! angle in rad
static scpi_result_t _scpi_jointRad( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    //! joint rad
    float jAngles[4];
    if ( 0 != LOCAL_ROBO()->nowJointAngle( jAngles ) )
    { scpi_ret( SCPI_RES_ERR ); }

    //! convert
    comAssist::degToRad( jAngles, 4 );

    //! output
    for ( int i = 0; i < 4; i++ )
    { SCPI_ResultFloat( context, jAngles[i] ); }

    return SCPI_RES_OK;
}

//! sub joint, page, dRad, dt
static scpi_result_t _scpi_jointMove( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    int subJoint, subPage;

    if ( SCPI_ParamInt32(context, &subJoint, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    if ( SCPI_ParamInt32(context, &subPage, true) != true )
    { scpi_ret( SCPI_RES_ERR ); }

    float vals[2];
    for ( int i = 0; i < sizeof_array(vals); i++ )
    {
        if ( SCPI_RES_OK != SCPI_ParamFloat( context, vals+i, true ) )
        { scpi_ret( SCPI_RES_ERR ); }
    }

    LOCAL_ROBO()->jointMove( subJoint, subPage, comAssist::radToDeg( vals[0] ), vals[1] );

    return SCPI_RES_OK;
}

static scpi_command_t _scpi_cmds[]=
{

    COM_ITEMs(),

    CMD_ITEM( "*IDN?", _scpi_idn ),
    CMD_ITEM( "RUN",  _scpi_run ),
    CMD_ITEM( "STOP", _scpi_stop ),
    CMD_ITEM( "SYNC", _scpi_sync ),

    CMD_ITEM( "MOVE", _scpi_move ),
    CMD_ITEM( "PREMOVE", _scpi_premove ),

    CMD_ITEM( "MOVEJ", _scpi_move ),
    CMD_ITEM( "PREMOVEJ", _scpi_premove ),

    CMD_ITEM( "MOVEL", _scpi_move ),
    CMD_ITEM( "PREMOVEL", _scpi_premove ),

    CMD_ITEM( "MOVER", _scpi_mover ),
    CMD_ITEM( "PREMOVER", _scpi_premover ),

    CMD_ITEM( "MOVET", _scpi_movet ),
    CMD_ITEM( "PREMOVET", _scpi_premovet ),

    CMD_ITEM( "GOTO", _scpi_goto ),
    CMD_ITEM( "GOTOJ", _scpi_goto ),
    CMD_ITEM( "GOTOL", _scpi_goto ),

    CMD_ITEM( "GOTOR", _scpi_gotor ),
    CMD_ITEM( "GOTOT", _scpi_gotot ),

    CMD_ITEM( "STEP:X", _scpi_stepx ),
    CMD_ITEM( "STEP:Y", _scpi_stepy ),
    CMD_ITEM( "STEP:Z", _scpi_stepz ),
    CMD_ITEM( "STEP:H", _scpi_steph ),

    CMD_ITEM( "STATE?", _scpi_fsmState ),
    CMD_ITEM( "POSE?", _scpi_pose ),
    CMD_ITEM( "DISTANCE?", _scpi_dist ),

    CMD_ITEM( "PROGRAM", _scpi_program ),
    CMD_ITEM( "DOWNLOAD", _scpi_download ),
    CMD_ITEM( "CALL", _scpi_call ),
    CMD_ITEM( "ZERO", _scpi_gozero ),
    CMD_ITEM( "CENTER", _scpi_gozero ),
    CMD_ITEM( "FOLD", _scpi_fold ),

    CMD_ITEM( "JOINT:RAD?", _scpi_jointRad ),
    CMD_ITEM( "JOINT:MOVE", _scpi_jointMove ),

    CMD_ITEM( "TEST1", _scpi_test1 ),
    CMD_ITEM( "TEST2", _scpi_test2 ),

    SCPI_CMD_LIST_END
};

const void* robotSinanju::loadScpiCmd()
{
    return _scpi_cmds;
}

