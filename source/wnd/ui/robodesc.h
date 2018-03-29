#ifndef ROBODESC_H
#define ROBODESC_H

#include "modelview.h"

namespace Ui {
class RoboDesc;
}

class RoboDesc : public modelView
{
    Q_OBJECT

public:
    explicit RoboDesc(QWidget *parent = 0);
    ~RoboDesc();

public:
    virtual void setModelObj( mcModelObj *pObj );

protected:
    void updateUi();

private:
    Ui::RoboDesc *ui;
};

#endif // ROBODESC_H
