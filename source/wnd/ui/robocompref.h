#ifndef ROBOCOMPREF_H
#define ROBOCOMPREF_H

#include <QtWidgets>
#include "modelview.h"
namespace Ui {
class RoboComPref;
}

class RoboComPref : public modelView
{
    Q_OBJECT

public:
    explicit RoboComPref(QWidget *parent = 0);
    ~RoboComPref();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

protected:
    void spyEdited();
    void updateUi();
    void updateData();

    int applyGroupId();

private:
    Ui::RoboComPref *ui;

    QList<QLabel*> mAxesLabels;
    QList<QLineEdit*> mAxesEdits;

};

#endif // ROBOCOMPREF_H
