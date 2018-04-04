#ifndef ROBOPROP_H
#define ROBOPROP_H

#include <QWidget>

#include "roboinfo.h"
#include "robodesc.h"
#include "robocompref.h"

#include "megatronpref.h"
#include "deltapref.h"

#include "sinanjupref.h"
#include "axnpref.h"

#include "robohand.h"

//! use mrq property
namespace Ui {
class mrqProperty;
}

class roboProp : public modelView
{
    Q_OBJECT

public:
    explicit roboProp( int roboId, QWidget *parent = 0);
    ~roboProp();

    virtual void setModelObj( mcModelObj *pObj );
    virtual void setMcModel( mcModel *pMcModel );
    virtual int setApply();

    virtual int  save( QString &outFileName );
    virtual int  saveAs( QString &outFileName );

protected:
    void setupUi( int roboId );
    void desetupUi();

    void buildConnection();

private slots:
    void slot_page_changed( int index );

    void on_btnOK_clicked();

    void on_btnCancel_clicked();

    void on_btnApply_clicked();

private:
    Ui::mrqProperty *ui;

    roboInfo *m_pInfoPage;
    RoboDesc *m_pDetailPage;

    RoboComPref *m_pComPref;

    MegatronPref *m_pMegatronPref;
    DeltaPref *m_pDeltaPref;
    SinanjuPref *m_pSinanjuPref;
    AxnPref *m_pAxnPref;

    RoboHand *m_pHandPage;

    int mPrefId;                //! id by robot
    QList<modelView*> mPrefPages;
};

#endif // ROBOPROP_H
