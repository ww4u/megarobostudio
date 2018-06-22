#ifndef H2ZPANEL_H
#define H2ZPANEL_H

#include <QWidget>

#include "robopanel.h"

#include "../../robot/h2z/h2z.h"

namespace Ui {
class H2ZPanel;
}

class H2ZPanel : public RoboPanel
{
    Q_OBJECT

public:
    explicit H2ZPanel(mcModel *pModel,
                      const QString &roboName,
                      QWidget *parent = 0);
    ~H2ZPanel();

protected:
    robotH2Z *Robot();

    //! move delta
    void moveD( float dx, float dy, float dz );
    //! move aim
    void moveA( float x, float y, float z, float dt );

private slots:
    void slot_timeout();

    void on_spinBox_valueChanged(int arg1);
    void on_chkOnOff_clicked(bool checked);

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
    Ui::H2ZPanel *ui;
    QTimer mTimer;
};

#endif // H2ZPANEL_H
