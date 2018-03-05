#ifndef TPVPLOT_H
#define TPVPLOT_H

#include <QtWidgets>

namespace Ui {
class tpvPlot;
}

class tpvPlot : public QDialog
{
    Q_OBJECT

public:
    explicit tpvPlot(QWidget *parent = 0);
    ~tpvPlot();

protected Q_SLOTS:
    void on_btnImage_clicked();
    void on_btnZoomOut_clicked();

public:
    void setDumpDir( const QString &dir );
    QString dumpDir();

    void setName( const QString &name );
    void setCurve( const QString &name,
                   double *pX, int skipX,
                   double *pY, int skipY,
                   int dataLen );
    void setCurves( const QString &name,
                    double *pX, int skipX,
                    double *y[],int pSkipy[],
                    int yCount,
                    int dataLen );

protected:
    bool dumpData( const QString &name, double *pData, int skip, int len );


private:
    Ui::tpvPlot *ui;

    QStringList mDumpFiles;

    QString mDumpDir;
};

#endif // TPVPLOT_H
