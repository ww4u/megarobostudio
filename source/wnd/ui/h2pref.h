#ifndef H2PREF_H
#define H2PREF_H

#include <QtWidgets>
#include "modelview.h"
namespace Ui {
class H2Pref;
}

class H2Pref : public modelView
{
    Q_OBJECT

public:
    explicit H2Pref(QWidget *parent = 0);
    ~H2Pref();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

protected:
    void updateData();
    void updateUi();

private:
    Ui::H2Pref *ui;

    QList<QLineEdit*> mEdits;
    QList<QLabel*> mLabels;
};

#endif // H2PREF_H
