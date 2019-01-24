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

protected:
    virtual void changeEvent( QEvent *event );

public:
    virtual void setMcModel( mcModel *pMcModel );
    virtual void setModelObj( mcModelObj *pObj );

public:
    virtual void setAxesId( int id );
    virtual void setPage( MRQ_AX_PAGE pg );

public:
    virtual int setApply();
public:
    virtual void modelChanged();
    virtual void adaptToModel();
    virtual void settingChanged(enumSetting setting, const QVariant &v);
protected:
    void spyEdited();
//    void setupUi();
//    void desetupUi();

//    int apply();
//    int updateUi();
//    int updateData();

private:
    Ui::MrqAxesPage *ui;

    QList<mrqView*> mSubViews;
};

#endif // MRQAXESPAGE_H
