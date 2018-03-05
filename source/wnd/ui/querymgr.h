#ifndef QUERYMGR_H
#define QUERYMGR_H

#include <QDialog>
#include <QtWidgets>

#include "../../model/comboxdelegate.h"
#include "../../model/checkdelegate.h"
#include "../../model/spycfgmodel.h"

#include "../../inst/instmgr.h"

namespace Ui {
class queryMgr;
}

class sampleThread;
class sampleProxy;
class queryMgr : public QDialog
{
    Q_OBJECT

public:
    explicit queryMgr(QWidget *parent = 0);
    ~queryMgr();

    void setModal( spyCfgModel *pModel );

    void attachInstmgr( MegaDevice::InstMgr *pMgr );
    MegaDevice::InstMgr *instMgr();

    void attachSampleThread( sampleThread *pThread );

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

protected:
    void setupUi();
    void deleteUi();
    void buildConnection();
    void commitItem( int id );

    sampleProxy* rowSampleProxy( QModelIndex &index );

protected Q_SLOTS:
    void slot_view_data();
    void slot_model_changed();

    void slot_spy_changed( const QString &name,
                           int item,
                           int size );

    void on_btnAdd_clicked();

    void on_btnDel_clicked();

    void on_btnExport_clicked();

    void on_btnLoad_clicked();

    void on_chkSpy_clicked(bool checked);

    void on_btnClearCache_clicked();

private:
    Ui::queryMgr *ui;
    QMenu *m_pContextMenu;
    QAction *m_pDataAction;

    comboxDelegate *m_pDelegate2, *m_pDelegate3;
    checkDelegate *m_pDelegate1;

    MegaDevice::InstMgr *m_pInstMgr;
    sampleThread *m_pSampleThread;
};

#endif // QUERYMGR_H
