#ifndef TPVPLOT_H
#define TPVPLOT_H

#include <QtWidgets>
#include <QtCharts>

namespace Ui {
class tpvPlot;
}

class NameCache
{
public:
    NameCache();
    ~NameCache();

    void setName( const QString &name );
    QString name();

    void setLength( int len );
    int length();

    int size();

    void clear();

    int alloc( int n, int size );

    double *operator[]( int id );

public:
    QString mName;
    QList< double *> mResource;

    int mLength;
};

#define curve_type  double
#define animation_time  500
#define animation_option    QChart::AllAnimations
#define chart_theme         QChart::ChartThemeBlueCerulean
//! QChart::ChartThemeQt
//!
class tpvPlot : public QDialog
{
    Q_OBJECT

public:
    explicit tpvPlot(QWidget *parent = 0);
    ~tpvPlot();

protected:
    virtual void changeEvent( QEvent *event );

protected Q_SLOTS:
    void on_btnImage_clicked();
    void on_btnZoomOut_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_cmbLineType_currentIndexChanged(int index);

public:
    void setDumpDir( const QString &dir );
    QString dumpDir();

    void setName( const QString &name );
    void setCurve( const QString &name,
                   curve_type *pX, int skipX,
                   curve_type *pY, int skipY,
                   int dataLen );
    void setCurve( const QString &name,
                   QList< QPointF> &curve  );

    void setCurves( const QString &name,
                    const QStringList &names,
                    curve_type *pX, int skipX,
                    curve_type *y[],int pSkipy[],
                    int yCount,
                    int dataLen );

    //! create
    QChartView * createCurve( const QString &name );
    QChartView * createCurves( const QString &name,
                               const QStringList &names,
                               int yCount
                                );
    void updateSeriesLineType( QLineSeries *pSeries );

    //! update
    void updateCurve( const QString &name,
                   curve_type *pX, int skipX,
                   curve_type *pY, int skipY,
                   int dataLen );

    void updateCurves( const QString &name,
                    curve_type *pX, int skipX,
                    curve_type *y[],int pSkipy[],
                    int yCount,
                    int dataLen );

protected:
    QChartView *findView( const QString &name );

    NameCache *getCache( const QString &name,
                         int n,
                         int size );

    NameCache *findCache( const QString &name );

    bool dumpData( const QString &name, double *pData, int skip, int len );

private:
    Ui::tpvPlot *ui;

    QString mDumpDir;

    QList< NameCache *> mNameCaches;

    int mLineCap;
};

#endif // TPVPLOT_H
