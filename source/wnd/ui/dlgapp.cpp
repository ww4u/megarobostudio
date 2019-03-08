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
//    m_pModel = &pPara->mAppModel;

    Q_ASSERT(NULL != pMgr );
    m_pMgr = pMgr;

    ui->setupUi(this);

    ui->tableView->setModel( &mAppModel );
    ui->spinStartDelay->setValue( pPara->mAppStartDelay );

    //! copy model
    mAppModel.assign( pPara->mAppModel );

    //! tool bar
    connect( ui->appToolBar, SIGNAL(signal_add_clicked()),
             this, SLOT(slot_toolbar_add()));
    connect( ui->appToolBar, SIGNAL(signal_del_clicked()),
             this, SLOT(slot_toolbar_del()));
    connect( ui->appToolBar, SIGNAL(signal_clr_clicked()),
             this, SLOT(slot_toolbar_clr()));

    //! model changed
    connect( &mAppModel, SIGNAL( signal_data_changed()),
             this, SLOT( slot_data_changed() ) );

    slot_data_changed();
}

DlgApp::~DlgApp()
{
    delete ui;
}

void DlgApp::changeEvent( QEvent *event )
{
    QDialog::changeEvent( event );

    if ( event->type() == QEvent::LanguageChange )
    { ui->retranslateUi( this ); }
}

#define model_      (mAppModel)
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

void DlgApp::slot_data_changed()
{
    bool bEn = ui->tableView->currentIndex().isValid();

    ui->btnStart->setEnabled( bEn );

    //! current
    int ind;
    ind = ui->tableView->currentIndex().row();
    AppItem *pItem = mAppModel.mItems.at( ind );
    if ( NULL != pItem && m_pMgr->findApp( pItem ) != NULL )
    { ui->btnKill->setEnabled( true ); }
    else
    { ui->btnKill->setEnabled( false ); }
}

void DlgApp::on_btnStart_clicked()
{
    if ( ui->tableView->currentIndex().isValid() )
    {}
    else
    {
        MegaMessageBox::warning( this, tr("Fail"), tr("Invalid item") );
        return;
    }

    //! current
    int ind;
    ind = ui->tableView->currentIndex().row();
    AppItem *pItem = mAppModel.mItems.at( ind );
    if ( NULL != pItem )
    {
        int ret = m_pMgr->startApp( pItem );
        if ( ret != 0 )
        { MegaMessageBox::warning( this, tr("Fail"), tr("Fail to start") ); }
    }
    else
    {
        MegaMessageBox::warning( this, tr("Fail"), tr("Invalid item") );
        return;
    }
}

void DlgApp::on_btnKill_clicked()
{
    if ( ui->tableView->currentIndex().isValid() )
    {}
    else
    {
        MegaMessageBox::warning( this, tr("Fail"), tr("Invalid item") );
        return;
    }

    //! current
    int ind;
    ind = ui->tableView->currentIndex().row();
    AppItem *pItem = mAppModel.mItems.at( ind );
    if ( NULL != pItem )
    {
        int ret = m_pMgr->stopApp( pItem );
        if ( ret != 0 )
        { MegaMessageBox::warning( this, tr("Fail"), tr("Fail to stop") ); }
        else
        { MegaMessageBox::information( this, tr("Success"), tr("Success to stoped") ); }
    }
    else
    {
        MegaMessageBox::warning( this, tr("Fail"), tr("Invalid item") );
        return;
    }
}

void DlgApp::setDelay( int dly )
{ ui->spinStartDelay->setValue( dly ); }
int DlgApp::getDelay()
{ return ui->spinStartDelay->value(); }

AppModel *DlgApp::getModel()
{ return &mAppModel; }

void DlgApp::on_tableView_activated(const QModelIndex &index)
{
    slot_data_changed();
}

void DlgApp::on_tableView_clicked(const QModelIndex &index)
{
    slot_data_changed();
}
