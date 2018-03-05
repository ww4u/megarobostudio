#include "tpvplot.h"
#include "ui_tpvplot.h"

#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot.h>

#include <QtCharts>
#include "../../../include/mcstd.h"

tpvPlot::tpvPlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tpvPlot)
{
    ui->setupUi(this);

    setAttribute( Qt::WA_DeleteOnClose );
}

tpvPlot::~tpvPlot()
{
    delete ui;
}

void tpvPlot::on_btnImage_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( ("PNG (*.png );;BMP (*.bmp);;CSV(*.csv)") );
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
//        ui->tabWidget->currentWidget()
        QChartView *pChartView = (QChartView*)ui->tabWidget->currentWidget();
        Q_ASSERT( NULL != pChartView );

        QChart *pChart = pChartView->chart();
        Q_ASSERT( NULL != pChart );

        QList<QAbstractSeries *> lineSeries = pChart->series();

        if ( lineSeries.size() < 1 )
        { return; }

        if ( ((QLineSeries*)lineSeries[0])->count() < 1 )
        { return; }

        QFile fileCsv( fDlg.selectedFiles().first() );
        if ( fileCsv.open( QIODevice::WriteOnly ) )
        {}
        else
        { return; }

        QTextStream outStream( &fileCsv );

        QLineSeries *pLine0;
        QLineSeries *pLine;
        pLine0 = (QLineSeries*)lineSeries[0];
        for ( int i = 0; i < pLine0->count(); i++ )
        {
            //! t
            outStream<<QString::number( pLine0->at(i).x() )<<",";

            //! y
            foreach( QAbstractSeries *pSer, lineSeries )
            {
                Q_ASSERT( NULL != pSer );
                pLine = (QLineSeries*)pSer;
                Q_ASSERT( NULL != pLine );

                outStream<<QString::number( pLine->at(i).y() )<<",";
            }

            outStream<<"\n";
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
    //! index invalid
    if ( ui->tabWidget->currentIndex() >= mDumpFiles.length()
         || ui->tabWidget->currentIndex() < 0 )
    { return; }

    QStringList arguments;

    //! change the path for windows
    QString winFile;
//    foreach( QString str, mDumpFiles )
    {
        winFile = mDumpFiles[ ui->tabWidget->currentIndex() ];        //! only the current file
        winFile.replace("/","\\");

        arguments<<winFile;
    }

logDbg()<<arguments;
    QProcess *myProcess = new QProcess();
    if ( NULL == myProcess )
    { return; }

    //! view by outer exe
    QString program = QCoreApplication::applicationDirPath() + "/dataview.exe";
    myProcess->start(program, arguments);
}

void tpvPlot::setDumpDir( const QString &dir )
{ mDumpDir = dir; }
QString tpvPlot::dumpDir()
{ return mDumpDir; }

void tpvPlot::setName( const QString &name )
{
    setWindowTitle( name );
}

void tpvPlot::setCurve( const QString &name,
                        double *pX, int skipX,
                        double *pY, int skipY,
                        int count )
{
    Q_ASSERT( NULL != pX && NULL != pY );

    QLineSeries* series = new QLineSeries();
    Q_ASSERT( NULL != series );

    for( int i = 0; i < count; i++ )
    {
        series->append( pX[i*skipX], pY[i*skipY] );
    }

    QChart *pChart = new QChart();
    pChart->legend()->hide();
    pChart->addSeries( series );
    pChart->createDefaultAxes();

    QChartView *pView = new QChartView();
    pView->setChart( pChart );

    //! add a view
    ui->tabWidget->addTab( pView, name );

    //! dump file
    bool bRet;
    QString fileName = mDumpDir + "/@" + name + "@" + QString::number( (quint32)this, 16 );
    bRet = dumpData( fileName, pY, skipY, count );
    if ( !bRet )
    { return; }
    mDumpFiles<<fileName;
    ui->btnZoomOut->setEnabled( true );
}

void tpvPlot::setCurves( const QString &name,
                double *pX, int skipX,
                double *y[],int pSkipY[],
                int yCount,
                int dataLen )
{
    //! chart
    QChart *pChart = new QChart();
    pChart->legend()->hide();

    for( int i = 0; i < yCount; i++ )
    {
        QLineSeries* series = new QLineSeries();
        for ( int j = 0; j < dataLen; j++ )
        {
            series->append( pX[ j*skipX ], y[i][ j * pSkipY[i] ] );
        }

        pChart->addSeries( series );
    }
    pChart->createDefaultAxes();

    QChartView *pView = new QChartView();
    pView->setChart( pChart );

    ui->tabWidget->addTab( pView, name );

}
//! name: label@addr
bool tpvPlot::dumpData( const QString &name, double *pData, int skip, int len )
{
    QFile file( name );

    bool b = file.open( QIODevice::WriteOnly );
    if ( b )
    {}
    else
    { return false; }

    for ( int i = 0; i < len; i++ )
    {
        if ( sizeof(double) != file.write( (char*)(pData+i*skip), sizeof(double)) )
        {
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}
