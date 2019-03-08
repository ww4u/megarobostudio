#ifndef DLGAPP_H
#define DLGAPP_H

#include <QDialog>

#include "../para/syspara.h"

namespace Ui {
class DlgApp;
}

class AppMgr;

class DlgApp : public QDialog
{
    Q_OBJECT

public:
    explicit DlgApp( SysPara *pPara,
                     AppMgr *pMgr,
                     QWidget *parent = 0 );
    ~DlgApp();    

protected:
    virtual void changeEvent( QEvent *event );

protected slots:
    void slot_toolbar_add();
    void slot_toolbar_del();
    void slot_toolbar_clr();

    void slot_data_changed();

private slots:
    void on_btnStart_clicked();

    void on_btnKill_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

public:
    void setDelay( int dly );
    int getDelay();

    AppModel *getModel();

private:
    Ui::DlgApp *ui;

//    AppModel *m_pModel;
    AppModel mAppModel;
    AppMgr *m_pMgr;
};

#endif // DLGAPP_H
