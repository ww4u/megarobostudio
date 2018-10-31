#ifndef ROBOSCENE_H
#define ROBOSCENE_H

#include <QtWidgets>

#include "modelview.h"
#include "../../robot/robotfact.h"
#include "../../widget/scenewidget.h"

#include "../../model/roboscenemodel.h"
#include "itemhelp.h"
namespace Ui {
class roboScene;
}

class roboScene : public modelView
{
    Q_OBJECT

public:
    explicit roboScene(QWidget *parent = 0);
    ~roboScene();

public:
//    virtual QSize sizeHint() const;

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void mouseDoubleClickEvent(QMouseEvent *event);

Q_SIGNALS:
    void itemXActivated( mcModelObj*, mcModelObj_Op op=model_obj_op_none );
    void signal_itemXHelp( eItemHelp helpId );

    void signalSceneChanged();

protected Q_SLOTS:
    void context_delete();
    void context_option();
    void context_new_mrp();

    void slot_editingFinished( sceneWidget * pWig, const QString &str );
    void slot_model_updated( mcModelObj *pBase );

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int save( QString &outFileName );
    virtual int saveAs( QString &name );

    sceneWidget * addRobot( mcModelObj *pBase );
    sceneWidget * addRobot( mcModelObj *pBase, sceneModel *pModel );

    int load( const QString &name, const QString &path );

    QList<VRobot *> complexRobotList();
    VRobot * findRobot( const QString &name );

protected:
    void toSceneModel( roboSceneModel *pSceneModel );
    void fromSceneModel( roboSceneModel *pSceneModel,
                         const QString &path );

    bool checkName( const QString &name,
                    sceneWidget *pSelf = NULL );

protected:
    void init();
    void deInit();

    void setupUi();
    void buildConnection();

    QString autoName( const QString &name );
    bool findName( const QString &name );

    void selectAll( bool bSel );
    void updateSelecte();
    int  selectCount();
    sceneWidget *theSelectedFirst();
private:
    Ui::roboScene *ui;
    QMenu *m_pContextMenu;
    QAction *m_pDelete;
    QAction *m_pOption;
    QAction *m_pNewMrp;

    QList<sceneWidget *> mItemList;
    QPoint mRefPoint;

    QLayout *m_pLayout;
};

#endif // ROBOSCENE_H
