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
    // read
    DEF_LOCAL_VAR();

    logDbg();

    QString str;

    str = ((robotSinanju*)context->user_context)->getName();

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

    pRobo->stop( tpvRegion(ax,page) );logDbg();

    return SCPI_RES_OK;
}

//! move ch,page, x1,y1,z1,h1, x2,y2,z2,h2, t
static scpi_result_t _scpi_move( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;

    float vals[9];

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

    //! default interp
    TraceKeyPoint pt1( 0, vals[0], vals[1], vals[2], vals[3] );
    TraceKeyPoint pt2( vals[8], vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    CHECK_LINK();

    pRobo->move( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! premove ch,page, x1,y1,z1,h1, x2,y2,z2,h2, t
static scpi_result_t _scpi_premove( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;

    float vals[9];

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

    //! default interp
    TraceKeyPoint pt1( 0, vals[0], vals[1], vals[2], vals[3] );
    TraceKeyPoint pt2( vals[8], vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    CHECK_LINK();

    pRobo->program( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! movej ch,page, x1,y1,z1,h1, x2,y2,z2,h2, t, j, tj
//! t : line time
//! tj : jump time
//! tj + t + tj
//! | ------ |
//! |        |
static scpi_result_t _scpi_movej( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;

    float vals[ 4 * 2 + 3 ];

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

    pRobo->move( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! premovej ch,page, x1,y1,z1,h1, x2,y2,z2,h2, t, j
static scpi_result_t _scpi_premovej( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;

    float vals[ 4 * 2 + 3 ];

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

    pRobo->program( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! movej ch,page, x1,y1,z1,h1, x2,y2,z2,h2, xl,yl,zl,hl, t
static scpi_result_t _scpi_movel( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;

    float vals[4*3+1];

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

    //! default interp
    TraceKeyPoint pt1( 0, vals[0], vals[1], vals[2], vals[3] );
    TraceKeyPoint ptl( vals[12]/2, vals[8], vals[9], vals[10], vals[11] );
    TraceKeyPoint pt2( vals[12], vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( ptl );
    curve.append( pt2 );

    CHECK_LINK();

    pRobo->move( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! premovej ch,page, x1,y1,z1,h1, x2,y2,z2,h2, xl,yl,zl,hl, t
static scpi_result_t _scpi_premovel( scpi_t * context )
{
    DEF_LOCAL_VAR();

    int ax, page;

    float vals[4*3+1];

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

    //! default interp
    TraceKeyPoint pt1( 0, vals[0], vals[1], vals[2], vals[3] );
    TraceKeyPoint ptl( vals[12]/2, vals[8], vals[9], vals[10], vals[11] );
    TraceKeyPoint pt2( vals[12], vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( ptl );
    curve.append( pt2 );

    CHECK_LINK();

    pRobo->program( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

//! goto x1,y1,z1,t
static scpi_result_t _scpi_goto( scpi_t * context )
{
    DEF_LOCAL_VAR();

    float vals[4];

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
    logDbg()<<pose.x<<pose.y<<pose.z;
    //! default interp
    TraceKeyPoint pt1( 0, pose.x, pose.y, pose.z, 0 );
    TraceKeyPoint pt2( vals[3], vals[0], vals[1], vals[2], 0 );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(0,0) );

    return SCPI_RES_OK;
}

//! goto x1,y1,z1,t, j, tj
static scpi_result_t _scpi_gotoj( scpi_t * context )
{
    DEF_LOCAL_VAR();

    float vals[4+2];

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
    logDbg()<<pose.x<<pose.y<<pose.z;
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

    pRobo->move( curve, tpvRegion(0,0) );

    return SCPI_RES_OK;
}

//! goto x1,y1,z1,t, xl,yl,zl
static scpi_result_t _scpi_gotol( scpi_t * context )
{
    DEF_LOCAL_VAR();

    float vals[4+3];

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
    logDbg()<<pose.x<<pose.y<<pose.z;
    //! default interp
    TraceKeyPoint pt1( 0, pose.x, pose.y, pose.z, 0 );
    TraceKeyPoint ptl( vals[3]/2, vals[4], vals[5], vals[6], 0 );
    TraceKeyPoint pt2( vals[3], vals[0], vals[1], vals[2], 0 );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( ptl );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(0,0) );

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
    { scpi_ret( SCPI_RES_ERR ); }logDbg()<<strLen<<pLocalStr;
    if (strLen < 1)
    { scpi_ret( SCPI_RES_ERR ); }

    QList<float> dataset;
    int col = 6;
    if ( 0 != comAssist::loadDataset( pLocalStr, strLen, col, dataset ) )
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

        logDbg()<<tp.t<<tp.x<<tp.y<<tp.z<<tp.hand<<tp.iMask;
    }

    DEF_ROBO();

    CHECK_LINK();

    pRobo->program( curve, tpvRegion( ax, page) );

    return SCPI_RES_OK;
}

#define server_path1     QCoreApplication::applicationDirPath() + QString( QDir::separator() )
#define server_path2     "G:\\work\\mc\\develope\\installer" + QString( QDir::separator() )
//static int _sloveFile( const QString &fileIn,
//                        const QString &fileOut )
//{
//    QStringList args;
//    QString program;
//    QString serverPath;

//    //! try path
//    program = server_path1 + QStringLiteral("deltaslove.exe");
//    if ( QFile::exists(program) )
//    { serverPath = server_path1; }
//    else
//    { serverPath = server_path2; }

//    QString cfgFile;
//    cfgFile = serverPath + QStringLiteral("deltaslove_config.txt");

//    QString inFile,outFile,configFile;
//    inFile = fileIn;
//    outFile = fileOut;
//    configFile = cfgFile;
//    args<<inFile<<outFile<<configFile;

//    logDbg()<<program<<args;

//    QProcess process;

//    process.start( program, args );

//    if ( process.waitForFinished( 120000 ) )
//    { return 0; }
//    else
//    { return -1; }
//}

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

//! int jointid
static scpi_result_t _scpi_gozero( scpi_t * context )
{
    DEF_LOCAL_VAR();
    DEF_ROBO();

    CHECK_LINK();

    int joint;

    //! robo
    if ( SCPI_ParamInt32(context, &joint, true) != true )
    {
        pRobo->goZero();
    }
    //! some joint
    //! \todo by change
    else
    {
        pRobo->goZero( joint, true );
    }

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

static scpi_command_t _scpi_cmds[]=
{

    CMD_ITEM( "*IDN?", _scpi_idn ),
    CMD_ITEM( "RUN",  _scpi_run ),
    CMD_ITEM( "STOP", _scpi_stop ),

    CMD_ITEM( "MOVE", _scpi_move ),
    CMD_ITEM( "PREMOVE", _scpi_premove ),

    CMD_ITEM( "MOVEJ", _scpi_movej ),
    CMD_ITEM( "PREMOVEJ", _scpi_premovej ),

    CMD_ITEM( "MOVEL", _scpi_movel ),
    CMD_ITEM( "PREMOVEL", _scpi_premovel ),

    CMD_ITEM( "GOTO", _scpi_goto ),
    CMD_ITEM( "GOTOJ", _scpi_gotoj ),
    CMD_ITEM( "GOTOL", _scpi_gotol ),

    CMD_ITEM( "STATE?", _scpi_fsmState ),
    CMD_ITEM( "POSE?", _scpi_pose ),
    CMD_ITEM( "DISTANCE?", _scpi_dist ),

    CMD_ITEM( "PROGRAM", _scpi_program ),
    CMD_ITEM( "DOWNLOAD", _scpi_download ),
    CMD_ITEM( "CALL", _scpi_call ),
    CMD_ITEM( "ZERO", _scpi_gozero ),

    CMD_ITEM( "JOINT:RAD?", _scpi_jointRad ),

    CMD_ITEM( "TEST1", _scpi_test1 ),
    CMD_ITEM( "TEST2", _scpi_test2 ),

    SCPI_CMD_LIST_END
};

const void* robotSinanju::loadScpiCmd()
{
    return _scpi_cmds;
}

