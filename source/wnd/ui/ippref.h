#ifndef IPPREF_H
#define IPPREF_H

#include <QWidget>
#include "modelview.h"
namespace Ui {
class IPPref;
}

class IPPref : public modelView
{
    Q_OBJECT

public:
    explicit IPPref(QWidget *parent = 0);
    ~IPPref();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

    virtual void updateScreen();
protected:
    void spyEdited();
    void updateData();
    void updateUi();

    void adaptUi();

    void zeroJoint( int jointId, bool bCcw );

private slots:
    void on_btnZeroX_clicked();

    void on_btnZeroY_clicked();

    void on_btnZeroBody_clicked();

private:
    Ui::IPPref *ui;
};

#endif // IPPREF_H
