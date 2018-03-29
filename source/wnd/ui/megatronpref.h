#ifndef MEGATRONPREF_H
#define MEGATRONPREF_H

#include <QtWidgets>
#include "modelview.h"
namespace Ui {
class megatronpref;
}

class MegatronPref : public modelView
{
    Q_OBJECT

public:
    explicit MegatronPref(QWidget *parent = 0);
    ~MegatronPref();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

protected:
    void updateData();
    void updateUi();

private:
    Ui::megatronpref *ui;

    QList<QLineEdit*> mEdits;
    QList<QLabel*> mLabels;
};

#endif // MEGATRONPREF_H
