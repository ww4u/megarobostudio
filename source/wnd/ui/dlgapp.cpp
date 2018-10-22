#include "dlgapp.h"
#include "ui_dlgapp.h"

#include "../widget/megamessagebox.h"
#include "../../app/appmgr.h"

DlgApp::DlgApp(SysPara *pPara,
               AppMgr *pMgr,
               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgApp)
{
    Q_ASSERT(NULL != pPara );
    m_pModel = &pPara->mAppModel;

    Q_ASSERT(NULL != pMgr );
    m_pMgr = pMgr;

    ui->setupUi(this);

    ui->tableView->setModel( m_pModel );
    ui->spinStartDelay->setValue( pPara->mAppStartDelay );

    //! tool bar
    connect( ui->appToolBar, SIGNAL(signal_add_clicked()),
             this, SLOT(slot_toolbar_add()));
    connect( ui->appToolBar, SIGNAL(signal_del_clicked()),
             this, SLOT(slot_toolbar_del()));
    connect( ui->appToolBar, SIGNAL(signal_clr_clicked()),
             this, SLOT(slot_toolbar_clr()));
}

DlgApp::~DlgApp()
{
    delete ui;
}

#define model_      (*m_pModel)
void DlgApp::slot_toolbar_add()
{
    if ( ui->tableView->currentIndex().isValid() )
    { model_.insertRow( ui->tableView->currentIndex().row() + 1 ); }
    else
    { model_.insertRow(0); }
}
void DlgApp::slot_toolbar_del()
{
    model_.removeRow( ui->tableView->currentIndex().row() );
}
void DlgApp::slot_toolbar_clr()
{
    MegaDeleteAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        model_.removeRows( 0, model_.mItems.size(), QModelIndex() );
    }
}

void DlgApp::on_btnStart_clicked()
{logDbg();
    //! current
    int ind;
    ind = ui->tableView->currentIndex().row();
    AppItem *pItem = m_pModel->mItems[ ind ];
    if ( NULL != pItem )
    {
        int ret = m_pMgr->startApp( pItem );
        if ( ret != 0 )
        { MegaMessageBox::warning( this, tr("Fail"), tr("Fail to start") ); }
    }
}

void DlgApp::on_btnKill_clicked()
{
    //! current
    int ind;
    ind = ui->tableView->currentIndex().row();
    AppItem *pItem = m_pModel->mItems[ ind ];
    if ( NULL != pItem )
    {
        int ret = m_pMgr->stopApp( pItem );
        if ( ret != 0 )
        { MegaMessageBox::warning( this, tr("Fail"), tr("Fail to stop") ); }
    }
}

void DlgApp::setDelay( int dly )
{ ui->spinStartDelay->setValue( dly ); }
int DlgApp::getDelay()
{ return ui->spinStartDelay->value(); }
