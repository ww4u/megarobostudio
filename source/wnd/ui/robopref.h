#ifndef ROBOPREF_H
#define ROBOPREF_H

#include <QtWidgets>

#include "modelview.h"

namespace Ui {
class roboPref;
}

class roboPref : public modelView
{
    Q_OBJECT

public:
    explicit roboPref(QWidget *parent = 0);
    ~roboPref();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

protected:
    void initModel();

    void updateData();
    void updateUi();

    int applyGroupId();

private:
    Ui::roboPref *ui;
    QList<QTreeWidgetItem*> mItems;

    QLabel *mLabels[8];
    QLineEdit *mEdits[8];

    QIntValidator *m_pGroupIdValidaor;
    QIntValidator *m_pGroupSubIdValidator;
};

#endif // ROBOPREF_H
