#ifndef PAGECONTROL_H
#define PAGECONTROL_H

#include <QtWidgets>

namespace Ui {
class PageControl;
}

class PageControl : public QWidget
{
    Q_OBJECT

public:
    explicit PageControl(QWidget *parent = 0);
    ~PageControl();

protected:
    virtual void changeEvent( QEvent *event );

Q_SIGNALS:
    void signal_run_clicked();
    void signal_stop_clicked();

public:
    QComboBox *getComb();

    void setPage( int pageIndex );
    int page();

private:
    Ui::PageControl *ui;
};

#endif // PAGECONTROL_H
