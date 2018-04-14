#ifndef SINANJUCONFIG_H
#define SINANJUCONFIG_H

#include <QWidget>
#include "modelview.h"
namespace Ui {
class SinanjuConfig;
}

class SinanjuConfig : public modelView
{
    Q_OBJECT

public:
    explicit SinanjuConfig(QWidget *parent = 0);
    ~SinanjuConfig();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

    virtual void updateScreen();

protected:
    void spyEdited();
    void updateData();
    void updateUi();

private:
    Ui::SinanjuConfig *ui;
};

#endif // SINANJUCONFIG_H
