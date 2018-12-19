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

private slots:
    void on_btnStart_clicked();

    void on_btnKill_clicked();

public:
    void setDelay( int dly );
    int getDelay();

private:
    Ui::DlgApp *ui;

    AppModel *m_pModel;
    AppMgr *m_pMgr;
};

#endif // DLGAPP_H
