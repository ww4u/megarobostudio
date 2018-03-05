#ifndef ROBOPROP_H
#define ROBOPROP_H

#include <QWidget>

#include "roboinfo.h"
#include "robopref.h"
#include "robohand.h"

//! use mrq property
namespace Ui {
class mrqProperty;
}

class roboProp : public modelView
{
    Q_OBJECT

public:
    explicit roboProp(QWidget *parent = 0);
    ~roboProp();

    virtual void setModelObj( mcModelObj *pObj );
    virtual void setMcModel( mcModel *pMcModel );
    virtual int setApply();

    virtual int  save( QString &outFileName );
    virtual int  saveAs( QString &outFileName );

protected:
    void setupUi();
    void desetupUi();

    void buildConnection();

private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

    void on_btnApply_clicked();

private:
    Ui::mrqProperty *ui;

    roboInfo *m_pInfoPage;
    roboPref *m_pPrefPage;
    RoboHand *m_pHandPage;

};

#endif // ROBOPROP_H
