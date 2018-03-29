#ifndef DEVICEMGR_H
#define DEVICEMGR_H

#include <QtWidgets>

//#include "../../app/appwidget.h"

#include "modelview.h"
#include "../../inst/instmgr.h"
#include "../../model/mcmodelobj.h"
#include "../../com/drivercontroller.h"

namespace Ui {
class deviceMgr;
}

class deviceMgr : public modelView
{
    Q_OBJECT

public:
    explicit deviceMgr(QWidget *parent = 0);
    ~deviceMgr();

public:
    virtual void setMcModel( mcModel *pMcModel );

protected:
    void init();
    void deinit();

    void setupUi();
    void desetupUi();

    void buildConnection();

Q_SIGNALS:
    void itemXActivated( mcModelObj* );

    void signalReport( int errCode, const QString &expr="" );

    void signal_instmgr_changed( bool bEnd, MegaDevice::InstMgr *pMgr );

    void signal_btnState_clicked();

protected Q_SLOTS:
    void on_btnTest_clicked();
    void on_pushButton_clicked();

    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

    void context_import();
    void context_export();
    void context_upload();

    void context_mrq_alias();
    void context_robo_alias();

    void context_mrq_console();
    void context_robo_console();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

protected:
    void matchDevices();
    void updateData();

    void updatePhyBusTree( VRoboList *pRoboList );
    void updateVirBusTree( VRoboList *pRoboList );

    int postSearchDevice( appMsg msg, void *pPara );
    void beginSearchDevice( void *pPara );
    void endSearchDevice( int ret, void *pPara );

    int postLoadOn( appMsg msg, void *pPara );
    void beginLoadOn( void *pPara );
    void endLoadOn( int ret, void *pPara );

    //! motor test
    int postMotorTest( appMsg msg, void *pPara );
    void beginMotorTest( void *pPara );
    void endMotorTest( int ret, void *pPara );

    void execMotorTest();

    //! robo test
    int postRoboTest( appMsg msg, void *pPara );
    void beginRoboTest( void *pPara );
    void endRoboTest( int ret, void *pPara );

    void execRoboTest();

    int doTest( appMsg msg, void *pPara );

public:
    void setInstMgr( MegaDevice::InstMgr *pMgr );
    void setDeviceDb( deviceDb *pDb );
    void setSysPref( modelSysPref *pref );

    void updateUi();

public:
    Ui::deviceMgr *ui;

    QMenu *m_pDeviceMenu;
    QMenu *m_pRoboMenu;

    MegaDevice::InstMgr *m_pMgr;
    deviceDb *m_pDeviceDb;
    modelSysPref *m_pPref;

    //! current
    MegaDevice::deviceMRQ *m_pMRQ;
    VRobot *m_pRobo;
    QTreeWidgetItem *m_pCurrTreeItem;
    int mCurrentAxes;

    QProgressDialog *m_pProgress;

    int mAxesCount;

};

#endif // DEVICEMGR_H
