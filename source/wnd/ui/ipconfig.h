#ifndef IPCONFIG_H
#define IPCONFIG_H

#include <QWidget>
#include "modelview.h"

namespace Ui {
class IPConfig;
}

class IPConfig : public modelView
{
    Q_OBJECT

public:
    explicit IPConfig(QWidget *parent = 0);
    ~IPConfig();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

    virtual void updateScreen();
protected:
    void spyEdited();
    void updateData();
    void updateUi();

    void adaptUi();

private:
    Ui::IPConfig *ui;
};

#endif // IPCONFIG_H
