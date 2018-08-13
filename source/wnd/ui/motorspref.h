#ifndef MOTORSPREF_H
#define MOTORSPREF_H

#include <QWidget>
#include "modelview.h"

namespace Ui {
class MotorsPref;
}

class MotorsPref : public modelView
{
    Q_OBJECT

public:
    explicit MotorsPref(QWidget *parent = 0);
    ~MotorsPref();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

    virtual void updateScreen();
protected:
    void updateUi();

protected Q_SLOTS:
    void slot_add_clicked();
    void slot_del_clicked();
    void slot_clr_clicked();

private:
    Ui::MotorsPref *ui;
};

#endif // MOTORSPREF_H
