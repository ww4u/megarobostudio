#ifndef DELTACONFIG_H
#define DELTACONFIG_H

#include <QtWidgets>

namespace Ui {
class DeltaConfig;
}
#include "modelview.h"
class DeltaConfig : public modelView
{
    Q_OBJECT

public:
    explicit DeltaConfig(QWidget *parent = 0);
    ~DeltaConfig();

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
    Ui::DeltaConfig *ui;

    QList< QDoubleSpinBox *> mSpinLens;

    QList< QDoubleSpinBox *> mSpinRanges;

    QList< QDoubleSpinBox *> mSpinP0s;
    QList< QDoubleSpinBox *> mSpinA0s;
};

#endif // DELTACONFIG_H
