#ifndef ROBOPROP_H
#define ROBOPROP_H

#include <QWidget>

#include "roboinfo.h"
#include "robodesc.h"
#include "robocompref.h"

#include "megatronpref.h"

#include "h2pref.h"
#include "h2config.h"

#include "deltapref.h"
#include "deltaconfig.h"

#include "sinanjupref.h"
#include "sinanjuconfig.h"
#include "axnpref.h"

#include "ippref.h"
#include "ipconfig.h"


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

    virtual int  save( QString &outFileName );
    virtual int  saveAs( QString &outFileName );

    virtual void updateScreen();
    virtual void updateModel();

protected:
    void setupUi( int roboId );
    void desetupUi();

    void buildConnection();

    int postApply( appMsg msg, void *pPara );
    void beginApply( void *pPara);
    void endApply( int ret, void *pPara );

    int postOk( appMsg msg, void *pPara );
    void beginOk( void *pPara);
    void endOk( int ret, void *pPara );

    void saveBtnSnap( bool bNow = false );
    void restoreBtnSnap();

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

    H2Pref *m_pH2Pref;
    H2Config *m_pH2Config;

    DeltaPref *m_pDeltaPref;
    DeltaConfig *m_pDeltaConfig;

    SinanjuPref *m_pSinanjuPref;
    SinanjuConfig *m_pSinanjuConfig;

    IPPref *m_pIpPref;
    IPConfig *m_pIpConfig;

    AxnPref *m_pAxnPref;

//    RoboHand *m_pHandPage;

    int mPrefId;                //! id by robot
    QList<modelView*> mPrefPages;
    QList< bool > mbtnEnableSnap;
};

#endif // ROBOPROP_H
