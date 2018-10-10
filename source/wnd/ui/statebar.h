#ifndef STATEBAR_H
#define STATEBAR_H

#include <QtWidgets>
#include "mprogressbar.h"
namespace Ui {
class stateBar;
}

class stateBar : public QWidget
{
    Q_OBJECT

public:
    explicit stateBar(QWidget *parent = 0);
    ~stateBar();

    void showInfo( const QString &str );

    void showProgress( int from, int to, int now,
                       const QString &info="" );
    void showState( const QString &str );

    void downloadProgress( bool b, int from=0, int to=0 , int now=0 );

    QProgressBar *progressBar();
    QLabel *progressInfo();
    QLabel *statusLabel();
    MProgressBar* downloadBar();

private:
    Ui::stateBar *ui;
};

#endif // STATEBAR_H
