#ifndef ROBOHAND_H
#define ROBOHAND_H

#include <QtWidgets>

#include "modelview.h"

namespace Ui {
class RoboHand;
}

class RoboHand : public modelView
{
    Q_OBJECT

public:
    explicit RoboHand(QWidget *parent = 0);
    ~RoboHand();

protected:
    void spyEdited();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual void updateScreen();
private slots:
    void on_btnExport_clicked();

    void on_btnLoad_clicked();

    void on_btnAdd_clicked();

    void on_btnDel_clicked();



private:
    Ui::RoboHand *ui;
};

#endif // ROBOHAND_H
