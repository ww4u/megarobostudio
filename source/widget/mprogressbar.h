#ifndef MPROGRESSBAR_H
#define MPROGRESSBAR_H

#include <QProgressBar>

class MProgressBar : public QProgressBar
{
    Q_OBJECT

public:
    MProgressBar( QWidget *parent = Q_NULLPTR );

Q_SIGNALS:
    void signal_clicked();

protected:
    virtual void mouseReleaseEvent( QMouseEvent *event );
};

#endif // MPROGRESSBAR_H
