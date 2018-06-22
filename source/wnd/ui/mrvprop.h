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
public:
    MrvProp( VRobot *pMrqRobo,
             QWidget *pParent = 0 );
    ~MrvProp();

protected Q_SLOTS:
    void slot_page_changed(int index );

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual void setMcModel( mcModel *pMcModel );

    virtual void updateScreen();

//    virtual int  save( QString &outFileName );
//    virtual int  saveAs( QString &outFileName );

protected:
    void setupUi();
    void buildConnection();

private:
    Ui::mrqProperty *ui;

    VRobot *m_pRefModel;

    QList<modelView *> mViewPages;
    QStringList mTitles;
    QStringList mIcons;
};

#endif // MRVPROP_H
