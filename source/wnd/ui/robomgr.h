#ifndef ROBOMGR_H
#define ROBOMGR_H

#include <QtWidgets>

#include "../../model/modelrobotmgr.h"
#include "../../robot/robotfact.h"

#include "../../model/mcmodelobj.h"

#include "itemhelp.h"

//namespace Ui {
//class roboMgr;
//}

class roboMgr : public QListWidget
{
    Q_OBJECT

public:
    explicit roboMgr(QListWidget *parent = 0);
    ~roboMgr();

protected:
//    virtual QSize sizeHint() const;
    virtual QMimeData *mimeData(const QList<QListWidgetItem *> items) const;
    virtual QStringList mimeTypes() const;

public:
    void setModel( const modelRobotMgr &mgr );

    void setupData();

    void setOperable( bool b );
    bool operAble();

Q_SIGNALS:
    void itemXActivated( mcModelObj*, mcModelObj_Op op=model_obj_op_none );
    void signal_itemXHelp( eItemHelp helpId );

private slots:
    void slot_itemActivated(QListWidgetItem *item);

private:
//    Ui::roboMgr *ui;

    QList< VRobot * > mRobots;

    bool mbOperable;
};

#endif // ROBOMGR_H
