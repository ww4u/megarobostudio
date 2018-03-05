#ifndef ROBOMGR_H
#define ROBOMGR_H

#include <QtWidgets>

#include "../../model/modelrobotmgr.h"
#include "../../robot/robotfact.h"

#include "../../model/mcmodelobj.h"

namespace Ui {
class roboMgr;
}

class roboMgr : public QWidget
{
    Q_OBJECT

public:
    explicit roboMgr(QWidget *parent = 0);
    ~roboMgr();

    void setModel( const modelRobotMgr &mgr );

    void setupData();

Q_SIGNALS:
    void itemXActivated( mcModelObj* );

private slots:
    void on_listWidget_itemActivated(QListWidgetItem *item);

private:
    Ui::roboMgr *ui;

    QList< VRobot * > mRobots;
};

#endif // ROBOMGR_H
