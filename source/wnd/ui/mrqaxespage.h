#ifndef MRQAXESPAGE_H
#define MRQAXESPAGE_H

#include <QWidget>
#include "mrqview.h"
namespace Ui {
class MrqAxesPage;
}

class MrqAxesPage : public mrqView
{
    Q_OBJECT

public:
    explicit MrqAxesPage(QWidget *parent = 0);
    ~MrqAxesPage();

public:
    virtual void setMcModel( mcModel *pMcModel );
    virtual void setModelObj( mcModelObj *pObj );

public:
    virtual void setAxesId( int id );
    virtual void setPage( MRQ_AX_PAGE pg );

public:
    virtual int setApply();
protected:
//    virtual void modelChanged();

protected:
    void setupUi();
    void desetupUi();

    int apply();
    int updateUi();
    int updateData();

private:
    Ui::MrqAxesPage *ui;

    QList<mrqView*> mSubViews;
};

#endif // MRQAXESPAGE_H
