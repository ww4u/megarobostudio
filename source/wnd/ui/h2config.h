#ifndef H2CONFIG_H
#define H2CONFIG_H

#include <QWidget>

#include "modelview.h"

namespace Ui {
class H2Config;
}

class H2Config : public modelView
{
    Q_OBJECT

public:
    explicit H2Config(QWidget *parent = 0);
    ~H2Config();

protected:
    virtual void changeEvent( QEvent *event );

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

    virtual void updateScreen();
protected:
    void spyEdited();
    void updateData();
    void updateUi();

private:
    Ui::H2Config *ui;
};

#endif // H2CONFIG_H
