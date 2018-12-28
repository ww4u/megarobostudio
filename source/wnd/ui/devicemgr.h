#ifndef DEVICEMGR_H
#define DEVICEMGR_H

#include <QtWidgets>

//#include "../../app/appwidget.h"

#include "modelview.h"
#include "../../inst/instmgr.h"
#include "../../model/mcmodelobj.h"
#include "../../com/drivercontroller.h"
#include "statediagram.h"
#include "itemhelp.h"
namespace Ui {
class deviceMgr;
}

class deviceMgr : public modelView
{
    Q_OBJECT

public:
    explicit deviceMgr(QWidget *parent = 0);
    ~deviceMgr();

protected:
    virtual QSize sizeHint() const;

    virtual void changeEvent( QEvent *event );

public:
    virtual void setMcModel( mcModel *pMcModel );

protected:
    void init();
    void deinit();

    void setupUi();
    void desetupUi();

    void setupMenu();
    void desetupMenu();

    void buildConnection();

Q_SIGNALS:
    void itemXActivated( mcModelObj*, mcModelObj_Op op=model_obj_op_none );
//    void signal_itemXHelp( eItemHelp helpId );

    void signalModelUpdated( mcModelObj* );

    void signalReport( int errCode, const QString &expr="" );

    void signal_instmgr_changed( bool bEnd, MegaDevice::InstMgr *pMgr );

    void signal_scene_robo_changed( MegaDevice::InstMgr *pMgr );
    void signal_device_changed( MegaDevice::InstMgr *pMgr );

    void signal_btnState_clicked();

    void signal_device_busy( bool b );

public Q_SLOTS:
    void on_pushButton_clicked();

protected Q_SLOTS:
    void on_btnTest_clicked();

    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

    void slot_device_busy( bool b );

    //! mrq
    void context_import();
    void context_export();
    void context_upload();

    void context_mrq_alias();

    void context_mrq_console();
    void context_mrq_panel();
    void context_mrq_prop();

    //! mrv
    void context_mrv_import();
    void context_mrv_export();
    void context_mrv_upload();
    void context_mrv_alias();

    void context_mrv_console();
    void context_mrv_panel();
    void context_mrv_prop();

    //! robo
    void context_robo_alias();
    void context_robo_console();
    void context_robo_axes();
    void context_robo_panel();
    void context_robo_prop();

    //! hub
    void context_hub_alias();
    void context_hub_console();

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

    int postImport( appMsg msg, void *pPara );
    void beginImport( void *pPara );
    void endImport( int ret, void *pPara );

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

    QString subGroupString( QList<int> subGps );
    QString subGroupString( int index, int id );

    void setBusy( bool b );
    bool isBusy();

    int doTest( appMsg msg, void *pPara );

public:
    void setInstMgr( MegaDevice::InstMgr *pMgr );
    void setDeviceDb( deviceDb *pDb );
    void setSysPref( modelSysPref *pref );

    void updateUi();

    void openBus();
    void closeBus();

    void requestUploadSetting( mcModelObj *pObj );

public:
    Ui::deviceMgr *ui;

    QList<QMenu*> mMenuList;        //! menu list

    QMenu *m_pDeviceMenu;
    QAction *m_pDeviceImportAction, *m_pDeviceUploadAction;
    QAction *m_pMrvImportAction, *m_pMrvUploadAction;

    QMenu *m_pMRVMenu;

    QMenu *m_pAxesMenu;

    QMenu *m_pRoboMenu;
    QAction *m_pRoboPanelAction;
    QAction *m_pRoboJoiontAction;

    QMenu *m_pHubMenu;

    MegaDevice::InstMgr *m_pMgr;
    deviceDb *m_pDeviceDb;
    modelSysPref *m_pPref;

    //! current
    MegaDevice::deviceMRQ *m_pMRQ;
    QString mImportFileName;

    VRobot *m_pRobo;
    QString mCurrNodeName;
    QTreeWidgetItem *m_pCurrTreeItem;
    int mCurrentAxes;

    QProgressDialog *m_pProgress;
    StateDiagram *m_pDiagram;
    int mAxesCount;

    bool mbDeviceBusy;
private slots:
    void on_toolState_clicked();
    void on_chkOnLine_clicked(bool checked);

};

#endif // DEVICEMGR_H
