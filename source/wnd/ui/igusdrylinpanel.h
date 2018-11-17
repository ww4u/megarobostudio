#ifndef IGUSDRYLINPANEL_H
#define IGUSDRYLINPANEL_H

#include <QWidget>
#include "robopanel.h"

#include "../../robot/igus_delta/igus_delta.h"

namespace Ui {
class IgusDrylinPanel;
}

class IgusDrylinPanel : public RoboPanel
{
    Q_OBJECT

public:
    explicit IgusDrylinPanel(mcModel *pModel,
                             VRobot *pRobo,
                             const QString &roboName,
                             QWidget *parent = 0);
    ~IgusDrylinPanel();

protected:
    robotIgusDelta *Robot();

    //! move delta
    void moveD( float dx, float dy, float dz );
    //! move aim
    void moveA( float x, float y, float z, float dt );

private slots:
    void slot_timeout();

    void on_spinBox_valueChanged(int arg1);
    void on_chkOnOff_clicked(bool checked);

    void on_widget_signal_run_clicked();
    void on_widget_signal_stop_clicked();

    void on_btnZP_clicked();

    void on_btnZN_clicked();

    void on_btnYN_clicked();

    void on_btnYP_clicked();

    void on_btnXP_clicked();

    void on_btnXN_clicked();

    void on_btnCenter_clicked();

    void on_btnGo_clicked();

    void on_btnMarkNow_clicked();

    void on_btnMarkUser_clicked();

private:
    Ui::IgusDrylinPanel *ui;
    QTimer mTimer;
};

#endif // IGUSDRYLINPANEL_H
