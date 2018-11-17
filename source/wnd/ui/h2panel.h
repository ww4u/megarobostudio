#ifndef H2PANEL_H
#define H2PANEL_H

#include <QWidget>

#include "robopanel.h"

#include "../../robot/h2/h2.h"

namespace Ui {
class H2Panel;
}

class H2Panel : public RoboPanel
{
    Q_OBJECT

public:
    explicit H2Panel(mcModel *pModel,
                     VRobot *pRobo,
                     const QString &roboName,
                     QWidget *parent = 0);
    ~H2Panel();

protected:
    robotH2 *Robot();

    //! move delta
    void moveD( float dx, float dy, bool bKeep = false );
    //! move aim
    void moveA( float x, float y, float dt );

    void stop();

private slots:
    void slot_timeout();

    void on_spinBox_valueChanged(int arg1);
    void on_chkOnOff_clicked(bool checked);

    void on_widget_signal_run_clicked();
    void on_widget_signal_stop_clicked();

    void on_btnYN_clicked();

    void on_btnYP_clicked();

    void on_btnXP_clicked();

    void on_btnXN_clicked();

    void on_btnCenter_clicked();

    void on_btnGo_clicked();

    void on_btnMarkUser_clicked();

    void on_btnMarkNow_clicked();

    void on_btnXP_pressed();
    void on_btnXP_released();

    void on_btnXN_pressed();
    void on_btnXN_released();

    void on_btnYP_pressed();
    void on_btnYP_released();

    void on_btnYN_pressed();
    void on_btnYN_released();

private:
    Ui::H2Panel *ui;

    QTimer mTimer;
};

#endif // H2PANEL_H
