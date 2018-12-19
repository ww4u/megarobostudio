#ifndef MRVCH_H
#define MRVCH_H

#include <QWidget>
#include "mrvview.h"
namespace Ui {
class MrvCh;
}

class MrvCh : public mrvView
{
    Q_OBJECT

public:
    explicit MrvCh(QWidget *parent = 0);
    ~MrvCh();

protected:
    virtual void changeEvent( QEvent *event );

public:
    virtual void setMcModel( mcModel *pMcModel );
    virtual void setModelObj( mcModelObj *pObj );

    virtual void setAxesId(int id);

public:
    virtual int setApply();
public:
    virtual void modelChanged();

protected:
    void spyEdited();

private:
    Ui::MrvCh *ui;
    QList<mrvView*> mSubViews;
};

#endif // MRVCH_H
