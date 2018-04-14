#include <QtCore>

#include "scpi/scpi.h"

#include "delta.h"
#include "../../com/comassist.h"

#define DEF_ROBO()      robotDelta *pRobo;\
                        pRobo = ((robotDelta*)context->user_context);\
                        Q_ASSERT( NULL != pRobo );
#define ROBO()          pRobo

static scpi_result_t _scpi_idn( scpi_t * context )
{
    // read
    DEF_LOCAL_VAR();

    DEF_ROBO();

    QString str;
    str = ROBO()->getName();

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

    for ( int i = 0; i < sizeof_array(vals); i++ )
    { logDbg()<<vals[i]; }

    //! robo op
    DEF_ROBO();

    TraceKeyPoint pt1( 0,
                          vals[0], vals[1], vals[2], vals[3]
                            );
    TraceKeyPoint pt2( vals[8],
                          vals[4], vals[5], vals[6], vals[7] );

    TraceKeyPointList curve;
    curve.append( pt1 );
    curve.append( pt2 );

    pRobo->move( curve, tpvRegion(ax,page) );

    return SCPI_RES_OK;
}

#define server_path1     QCoreApplication::applicationDirPath() + QString( QDir::separator() )
#define server_path2     "G:\\work\\mc\\develope\\installer" + QString( QDir::separator() )
static int _sloveFile( const QString &fileIn,
                        const QString &fileOut )
{
    QStringList args;
    QString program;
    QString serverPath;

    //! try path
//    program = server_path1 + QStringLiteral("sinanjuslove.exe");
    program = server_path1 + QStringLiteral("deltaslove.exe");
    if ( QFile::exists(program) )
    { serverPath = server_path1; }
    else
    { serverPath = server_path2; }

    QString cfgFile;
//    cfgFile = serverPath + QStringLiteral("sinanjuslove_config.txt");
    cfgFile = serverPath + QStringLiteral("deltaslove_config.txt");

    QString inFile,outFile,configFile;
    inFile = fileIn;
    outFile = fileOut;
    configFile = cfgFile;
    args<<inFile<<outFile<<configFile;

    logDbg()<<program<<args;

    QProcess process;

    process.start( program, args );

    if ( process.waitForFinished( 120000 ) )
    { return 0; }
    else
    { return -1; }
}

////! ax, page, file
//static scpi_result_t _scpi_program( scpi_t * context )
//{
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

//    //! x,y,z,h,interp,t
//    QList<float> dataset;
//    int col = 6;
//    if ( 0 != comAssist::loadDataset( pLocalStr, strLen, col, dataset ) )
//    { scpi_ret( SCPI_RES_ERR ); }

//    //! point
//    if ( dataset.size() / col < 2 )
//    { scpi_ret( SCPI_RES_ERR ); }

//    TraceKeyPointList curve;
//    TraceKeyPoint tp;
//    for ( int i = 0; i < dataset.size()/col; i++ )
//    {
//        for ( int j = 0; j < col-1; j++ )
//        {
//            tp.datas[j+1] = dataset.at( i * col + j);
//        }

//        tp.t = dataset.at( i * col + col - 1);

//        curve.append( tp );
//    }

//    DEF_ROBO();
//    int ret;
//    ret = pRobo->program( curve, tpvRegion( ax, page) );
//    if ( ret != 0 )
//    { scpi_ret( SCPI_RES_ERR ); }

//    return SCPI_RES_OK;
//}

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

    //! find the input file
    QByteArray byteName( pLocalStr, strLen );
    QString fileInName( byteName );
    if ( comAssist::ammendFileName( fileInName ) )
    {}
    else
    { scpi_ret( SCPI_RES_ERR ); }

    //! slove the file
    QString fileOutName;
    DEF_ROBO();
    fileOutName = ROBO()->tempPath() + QDir::separator() + "deltapvt_pvt.csv";
    fileOutName = QDir::toNativeSeparators( fileOutName );

    if ( 0 != _sloveFile( fileInName, fileOutName ) )
    { scpi_ret( SCPI_RES_ERR ); }
logDbg()<<fileInName<<fileOutName;
    //! success
    int ret;
    ret = pRobo->program( fileOutName, tpvRegion( ax, page) );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

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
    QByteArray fileName( pLocalStr, strLen );
    int ret;
    ret = pRobo->program( fileName, tpvRegion( ax, page) );
    if ( ret != 0 )
    { scpi_ret( SCPI_RES_ERR ); }

    return SCPI_RES_OK;
}

//! page
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

    CMD_ITEM( "*IDN?", _scpi_idn ),
    CMD_ITEM( "RUN",  _scpi_run ),
    CMD_ITEM( "MOVE", _scpi_move ),

    CMD_ITEM( "STATE?", _scpi_fsmState ),

    CMD_ITEM( "PROGRAM", _scpi_program ),
    CMD_ITEM( "DOWNLOAD", _scpi_download ),
    CMD_ITEM( "CALL", _scpi_call ),

    CMD_ITEM( "TEST1", _scpi_test1 ),
    CMD_ITEM( "TEST2", _scpi_test2 ),

    SCPI_CMD_LIST_END
};

const void* robotDelta::loadScpiCmd()
{
    return _scpi_cmds;
}

