#ifndef AXN_PREF_H
#define AXN_PREF_H

#include <QtWidgets>
#include "modelview.h"
namespace Ui {
class AxnPref;
}

class AxnPref : public modelView
{
    Q_OBJECT

public:
    explicit AxnPref(QWidget *parent = 0);
    ~AxnPref();    

protected:
    virtual void changeEvent( QEvent *event );

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();
    virtual void updateScreen();
protected:
    void updateData();
    void updateUi();

private:
    Ui::AxnPref *ui;

    QList<QLineEdit*> mEdits;
    QList<QLabel*> mLabels;
};

#endif // AxnPref_H
