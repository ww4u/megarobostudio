#include "tpvplot.h"
#include "ui_tpvplot.h"

#include "../../../include/mcstd.h"

#include "../../com/comassist.h"

NameCache::NameCache()
{
    mLength = 0;
}
NameCache::~NameCache()
{
    clear();
}

void NameCache::setName( const QString &name )
{ mName = name; }
QString NameCache::name()
{ return mName; }

void NameCache::setLength( int len )
{ mLength = len; }
int NameCache::length()
{ return mLength; }

int NameCache::size()
{ return mResource.size(); }

void NameCache::clear()
{
    for( int i = 0; i < mResource.size(); i++ )
    {
        delete []mResource.at(i);
    }
    mLength = 0;

    mResource.clear();
}

int NameCache::alloc( int n, int size )
{
    clear();

    double *ptr;
    for ( int i = 0; i < n; i++ )
    {
        ptr = new double[ size ];
        if ( NULL != ptr )
        { mResource.append( ptr ); }
        else
        { return ERR_ALLOC_FAIL; }
    }

    mLength = size;

    return 0;
}

double *NameCache::operator[]( int id )
{
    return mResource.at( id );
}

tpvPlot::tpvPlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tpvPlot)
{
    ui->setupUi(this);

    mLineCap = 2048;
}

tpvPlot::~tpvPlot()
{
    delete_all( mNameCaches );

    delete ui;
}

void tpvPlot::on_btnImage_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( ("CSV(*.csv);;PNG (*.png );;BMP (*.bmp)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    QString nameFilter;
    nameFilter = fDlg.selectedNameFilter();

    //! image
    if ( nameFilter == "PNG (*.png )"
         || nameFilter == "BMP(*.bmp)" )
    {
        QPixmap pixmap = ui->tabWidget->currentWidget()->grab();
        pixmap.save( fDlg.selectedFiles().first() );
    }
    //! csv
    else if ( nameFilter == "CSV(*.csv)" )
    {
        //! export the current data
        QString name = ui->tabWidget->tabText( ui->tabWidget->currentIndex() );

        NameCache *pCache = findCache( name );
        Q_ASSERT( NULL != pCache );

        QFile fileCsv( fDlg.selectedFiles().first() );
        if ( fileCsv.open( QIODevice::WriteOnly ) )
        {}
        else
        { return; }

        QTextStream outStream( &fileCsv );

        for ( int i = 0; i < pCache->length(); i++ )
        {
            //! t
            outStream<<QString::number( (*pCache)[0][i] );

            //! y
            for( int j = 1; j < pCache->size(); j++ )
            {
                outStream<<CSV_COL_SEP<<QString::number( (*pCache)[j][i] );
            }

            outStream<<CSV_LINE_SEP;
        }

        fileCsv.close();

    }
    else
    {
        Q_ASSERT( false );
    }
}

void tpvPlot::on_btnZoomOut_clicked()
{
    //! dump file
    QString name = ui->tabWidget->tabText( ui->tabWidget->currentIndex() );
    QString fileName = mDumpDir + QDir::separator() + "@" + name + "@" + QString::number( rand(), 16 );

    NameCache *pCache = findCache( name );
    if ( NULL == pCache )
    { return; }

    bool bRet;
    fileName = QDir::toNativeSeparators( fileName );
    bRet = dumpData( fileName, (*pCache)[1], 1, pCache->length() );
    if ( !bRet )
    { return; }

    //! arguments
    QStringList arguments;

    arguments<<fileName;

    //! process
    QProcess *myProcess = new QProcess();
    if ( NULL == myProcess )
    { return; }

    //! view by outer exe
    QString program = QCoreApplication::applicationDirPath() + "/dataview.exe";
    myProcess->start(program, arguments);
}

void tpvPlot::on_tabWidget_currentChanged(int index)
{
    QString name;

    name = ui->tabWidget->tabText( index );

    NameCache *pCache = findCache( name );
    if ( NULL == pCache )
    {
        ui->btnZoomOut->setEnabled( false );
        return;
    }

    //! only t-y can be zoom out
    if ( pCache->size() != 2 )
    {
        ui->btnZoomOut->setEnabled( false );
        return;
    }

    ui->btnZoomOut->setEnabled( true );
    return ;
}

void tpvPlot::on_cmbLineType_currentIndexChanged(int index)
{
    QChartView *pView;
    for( int i = 0; i < ui->tabWidget->count(); i++ )
    {
        pView = (QChartView*)ui->tabWidget->widget(i);
        Q_ASSERT( NULL != pView );

        QList<QAbstractSeries *> seriesList = pView->chart()->series();

        foreach( QAbstractSeries *pSeries, seriesList )
        {
            Q_ASSERT( NULL != pSeries );

            updateSeriesLineType( (QLineSeries*)pSeries );
        }

        pView->update();
    }
}

void tpvPlot::setDumpDir( const QString &dir )
{ mDumpDir = dir; }
QString tpvPlot::dumpDir()
{ return mDumpDir; }

void tpvPlot::setName( const QString &name )
{
    setWindowTitle( name );
}

//! create + update
void tpvPlot::setCurve( const QString &name,
                        curve_type *pX, int skipX,
                        curve_type *pY, int skipY,
                        int count )
{
    //! find the curve
    QChartView *pView ;
    pView = findView( name );

    //! find fail
    if ( NULL == pView )
    { pView = createCurve( name ); }
    else
    {
    }

    //! invalid view
    if ( NULL == pView )
    { return; }

    //! add data
    QLineSeries *pLine;
    pLine = (QLineSeries*)( pView->chart()->series().at( 0 ) );
    Q_ASSERT( NULL != pLine );

    //! cache the data
    NameCache *pCache = getCache( name, 2, count );
    if ( NULL == pCache )
    { return; }

    for( int i = 0; i < count; i++ )
    {
        //! cache the data
        (*pCache)[0][i] = pX[i*skipX];
        (*pCache)[1][i] = pY[i*skipX];
    }

    //! cache
    curve_type *pXY;
    pXY = new curve_type[ 2*mLineCap ];
    if ( NULL == pXY )
    { return; }

    //! compress
    int retSize;
    retSize = dataCompress( (*pCache)[1], count, 1,
                            pXY + mLineCap, mLineCap,
                            (*pCache)[0], pXY, 1 );
    pLine->clear();
    for ( int i = 0; i < retSize; i++ )
    {
        pLine->append( pXY[i], pXY[ mLineCap + i ] );
    }
    delete []pXY;

    //! remove and add again
    pView->chart()->removeSeries( pLine );
    pView->chart()->addSeries( pLine );

    //! udpate the axes
    pView->chart()->createDefaultAxes();
}

void tpvPlot::setCurve( const QString &name,
               QList< QPointF> &curve  )
{
    //! find the curve
    QChartView *pView ;
    pView = findView( name );

    //! find fail
    if ( NULL == pView )
    { pView = createCurve( name ); }
    else
    {
    }

    //! invalid view
    if ( NULL == pView )
    { return; }

    //! add data
    QLineSeries *pLine;
    pLine = (QLineSeries*)( pView->chart()->series().at( 0 ) );
    Q_ASSERT( NULL != pLine );

    //! cache the data
    NameCache *pCache = getCache( name, 2, curve.size() );
    if ( NULL == pCache )
    { return; }

    for( int i = 0; i < curve.size(); i++ )
    {
        //! cache the data
        (*pCache)[0][i] = curve[i].x();
        (*pCache)[1][i] = curve[i].y();
    }

    //! cache
    curve_type *pXY;
    pXY = new curve_type[ 2*mLineCap ];
    if ( NULL == pXY )
    { return; }

    //! compress
    int retSize;
    retSize = dataCompress( (*pCache)[1], curve.size(), 1,
                             pXY + mLineCap, mLineCap,
                             (*pCache)[0], pXY, 1 );

    pLine->clear();
    for ( int i = 0; i < retSize; i++ )
    {
        pLine->append( pXY[i], pXY[ mLineCap + i ] );
    }
    delete []pXY;

    //! remove and add again
    pView->chart()->removeSeries( pLine );
    pView->chart()->addSeries( pLine );

    //! udpate the axes
    pView->chart()->createDefaultAxes();
}

void tpvPlot::setCurves( const QString &name,
                         const QStringList &names,
                curve_type *pX, int skipX,
                curve_type *y[],int pSkipY[],
                int yCount,
                int dataLen
                         )
{
    //! find the curve
    QChartView *pView ;
    pView = findView( name );

    //! find fail
    if ( NULL == pView )
    { pView = createCurves( name, names, yCount ); }
    else
    {}

    //! invalid view
    if ( NULL == pView )
    { return; }

    //! cache the data t + y
    NameCache *pCache = getCache( name, 1 + yCount, dataLen );
    if ( NULL == pCache )
    { return; }

    //! t
    for ( int i = 0; i < dataLen; i++ )
    {
        (*pCache)[0][i] =  pX[ i * skipX ];
    }

    //! ys
    for ( int i = 0; i < yCount; i++ )
    {
        for ( int j = 0; j < dataLen; j++ )
        {
            (*pCache)[ i + 1 ][ j ] = y[i][ pSkipY[i] * j ];
        }
    }

    //! plot
    QLineSeries *pLine;

    QList< QLineSeries * > lines;
    for ( int i = 0; i < yCount; i++ )
    {
        pLine = (QLineSeries*)( pView->chart()->series().at( i ) );
        Q_ASSERT( NULL != pLine );
        lines.append( pLine );
    }

    //! detach
    for ( int i = 0; i < yCount; i++ )
    {
        pLine = lines[i];
        pLine->clear();
        pView->chart()->removeSeries( pLine );
    }

    //! cache
    curve_type *pXY;
    pXY = new curve_type[ 2*mLineCap ];
    if ( NULL == pXY )
    { return; }

    //! compress t
    int retSize;

    //! add
    for ( int i = 0; i < yCount; i++ )
    {
        pLine = lines[i];

        //! compress
        retSize = dataCompress(  y[ i ], dataLen, pSkipY[i],
                                 pXY+mLineCap, mLineCap,
                                 (*pCache)[0], pXY, 1
                                   );

        for ( int j = 0; j <retSize; j++ )
        {
            pLine->append( pXY[j], pXY[mLineCap+j] );
        }

        //! add again
        pView->chart()->addSeries( pLine );
    }

    delete []pXY;

    //! axes
    pView->chart()->createDefaultAxes();
}

QChartView * tpvPlot::createCurve( const QString &name )
{
    //! series
    QLineSeries* series = new QLineSeries();
    Q_ASSERT( NULL != series );
    updateSeriesLineType( series );

    //! chart
    QChart *pChart = new QChart();
    pChart->setAnimationDuration( animation_time );
    pChart->setAnimationOptions( animation_option );
    pChart->setTheme( chart_theme );
    pChart->legend()->hide();

    pChart->addSeries( series );

    //! view
    QChartView *pView = new QChartView();
    pView->setChart( pChart );

    //! add a view
    ui->tabWidget->addTab( pView, name );

    ui->tabWidget->currentChanged( ui->tabWidget->currentIndex() );

    return pView;
}

QChartView * tpvPlot::createCurves( const QString &name,
                                    const QStringList &names,
                                    int yCount
                                     )
{
    //! chart
    QChart *pChart = new QChart();
    pChart->setAnimationDuration( animation_time );
    pChart->setAnimationOptions( animation_option );
    pChart->setTheme( chart_theme );
    pChart->legend()->show();

    //! series ys
    for( int i = 0; i < yCount; i++ )
    {
        QLineSeries* series = new QLineSeries();
        Q_ASSERT( NULL != series );

        updateSeriesLineType( series );

        pChart->addSeries( series );
        series->setName( names.at(i) );
    }

    //! view
    QChartView *pView = new QChartView();
    pView->setChart( pChart );

    //! tab
    ui->tabWidget->addTab( pView, name );
    ui->tabWidget->currentChanged( ui->tabWidget->currentIndex() );

    return pView;
}

void tpvPlot::updateSeriesLineType( QLineSeries *pSeries )
{
    Q_ASSERT( NULL != pSeries );

    if ( ui->cmbLineType->currentIndex() == 1 )
    { pSeries->setPointsVisible(false); }
    else
    { pSeries->setPointsVisible(true); }
}

QChartView *tpvPlot::findView( const QString &name )
{
    QChartView *pView;
    for( int i = 0; i < ui->tabWidget->count(); i++ )
    {
        if ( ui->tabWidget->tabText( i ) == name )
        {
            pView = (QChartView*)ui->tabWidget->widget(i);
            return pView;
        }
    }

    return NULL;
}

NameCache *tpvPlot::getCache(
                     const QString &name,
                     int n,
                     int size )
{
    //! find the cache
    NameCache *pCache = NULL;
    pCache = findCache( name );

    //! find success
    if ( NULL != pCache )
    {}
    //! find fail
    else
    {
        pCache = new NameCache();
        if ( NULL == pCache )
        { return NULL; }

        pCache->setName( name );
        mNameCaches.append( pCache );
    }

    //! clear
    pCache->clear();

    //! alloc again
    if ( pCache->alloc( n, size ) != 0 )
    {
        return NULL;
    }

    return pCache;
}

NameCache *tpvPlot::findCache( const QString &name )
{
    NameCache *pCache = NULL;
    foreach( pCache, mNameCaches )
    {
        Q_ASSERT( NULL != pCache );
        if ( pCache->name() == name )
        { return pCache;}
    }

    return NULL;
}

//! name: label@addr
bool tpvPlot::dumpData( const QString &name,
                        curve_type *pData,
                        int skip, int len )
{
    QFile file( name );

    bool b = file.open( QIODevice::WriteOnly );
    if ( b )
    {}
    else
    { return false; }

    for ( int i = 0; i < len; i++ )
    {
        if ( sizeof(curve_type) != file.write( (char*)(pData+i*skip), sizeof(double)) )
        {
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}

