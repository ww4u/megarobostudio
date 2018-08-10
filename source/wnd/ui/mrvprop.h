#ifndef MRVPROP_H
#define MRVPROP_H

#include <QtWidgets>

#include "modelview.h"

//! use mrq property
namespace Ui {
class mrqProperty;
}

#include "mrvinfo.h"
#include "mrvch.h"
#include "mrvsystem.h"

class MrvProp : public mrvView
{
    Q_OBJECT

public:
    MrvProp( VRobot *pMrqRobo,
             QWidget *pParent = 0 );
    ~MrvProp();

protected Q_SLOTS:
    void slot_page_changed(int index );

    void on_btnApply_clicked();
    void on_btnOK_clicked();
    void on_btnCancel_clicked();
    void on_btnReset_clicked();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual void setMcModel( mcModel *pMcModel );

    virtual void updateScreen();
    virtual void setActive();

    virtual int  save( QString &outFileName );
    virtual int  saveAs( QString &outFileName );

protected:
    void setupUi();
    void buildConnection();

    int postApply( appMsg msg, void *pPara );
    void beginApply( void *pPara);
    void endApply( int ret, void *pPara );

    int postOk( appMsg msg, void *pPara );
    void beginOk( void *pPara);
    void endOk( int ret, void *pPara );

    void saveBtnSnap( bool bNow = false );
    void restoreBtnSnap();

private:
    Ui::mrqProperty *ui;

    VRobot *m_pRefModel;

    QList<modelView *> mViewPages;
    QList< bool > mbtnEnableSnap;

    MrvSystem *m_pSystem;

    QStringList mTitles;
    QStringList mIcons;
};

#endif // MRVPROP_H
