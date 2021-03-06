#ifndef ROBOINFO_H
#define ROBOINFO_H

#include <QtWidgets>

#include "modelview.h"

namespace Ui {
class roboInfo;
}

class roboInfo : public modelView
{
    Q_OBJECT

public:
    explicit roboInfo(QWidget *parent = 0);
    ~roboInfo();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual void updateScreen();
protected:
    void initModel();
    void deInitModel();

    void updateUi();
private:
    Ui::roboInfo *ui;
    QList< QTableWidgetItem *> mItems;
};

#endif // ROBOINFO_H
