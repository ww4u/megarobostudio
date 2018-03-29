#ifndef SINANJUPREF_H
#define SINANJUPREF_H

#include <QtWidgets>

#include "modelview.h"

namespace Ui {
class SinanjuPref;
}

class SinanjuPref : public modelView
{
    Q_OBJECT

public:
    explicit SinanjuPref(QWidget *parent = 0);
    ~SinanjuPref();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

protected:
    void initModel();

    void updateData();
    void updateUi();

    int applyGroupId();

private:
    Ui::SinanjuPref *ui;
    QList<QTreeWidgetItem*> mItems;

    QLabel *mLabels[8];
    QLineEdit *mEdits[8];


};

#endif // ROBOPREF_H
