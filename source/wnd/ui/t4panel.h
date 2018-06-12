#ifndef T4PANEL_H
#define T4PANEL_H

#include <QtWidgets>
#include "../../device/vrobot.h"
#include "dlgview.h"

#include "../../robot/sinanju/sinanju.h"

namespace Ui {
class T4Panel;
}

class T4Panel : public DlgView
{
    Q_OBJECT

public:
    explicit T4Panel(mcModel *pModel,
                     const QString &roboName,
                     QWidget *parent = 0);
    ~T4Panel();

protected Q_SLOTS:
    void slot_timeout();
    void slot_device_changed();
protected:
    robotSinanju *Robot();

    //! move delta
    void moveD( float dx, float dy, float dz );
    //! move aim
    void moveA( float x, float y, float z, float dt );

private slots:
    void on_spinBox_valueChanged(int arg1);


    void on_btnYP_clicked();

    void on_btnYN_clicked();

    void on_btnZP_clicked();

    void on_btnZN_clicked();

    void on_btnXN_clicked();

    void on_btnXP_clicked();

    void on_btnCenter_clicked();

    void on_btnGo_clicked();

    void on_btnMarkNow_clicked();

    void on_btnMarkUser_clicked();

    void on_chkOnOff_clicked(bool checked);

private:
    Ui::T4Panel *ui;

    QTimer mTimer;
};

#endif // T4PANEL_H
