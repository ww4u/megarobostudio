#ifndef IGUSCONFIG_H
#define IGUSCONFIG_H

#include <QWidget>

#include "modelview.h"

namespace Ui {
class IgusConfig;
}

class IgusConfig : public modelView
{
    Q_OBJECT

public:
    explicit IgusConfig(QWidget *parent = 0);
    ~IgusConfig();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

    virtual void updateScreen();
protected:
    void spyEdited();
    void updateData();
    void updateUi();

private:
    Ui::IgusConfig *ui;
};

#endif // IGUSCONFIG_H
