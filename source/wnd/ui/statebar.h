#ifndef STATEBAR_H
#define STATEBAR_H

#include <QtWidgets>

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

    QProgressBar *progressBar();
    QLabel *progressInfo();
    QLabel *statusLabel();

private:
    Ui::stateBar *ui;
};

#endif // STATEBAR_H
