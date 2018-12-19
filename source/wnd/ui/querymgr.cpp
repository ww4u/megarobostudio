#include "querymgr.h"
#include "ui_querymgr.h"
#include "queryview.h"

#include "../../model/spycfgmodel.h"
#include "../../model/spyitemmodel.h"
#include "../../model/spycfgmodel.h"

#include "../../app/samplethread.h"

queryMgr::queryMgr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::queryMgr)
{
    ui->setupUi(this);

    m_pContextMenu = NULL;
    m_pDataAction = NULL;

    m_pInstMgr = NULL;
    m_pSampleThread = NULL;

    setupUi();

    buildConnection();
}

queryMgr::~queryMgr()
{
    deleteUi();

    delete ui;
}

void queryMgr::changeEvent(QEvent * event)
{
    QDialog::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi( this );

        m_pDataAction->setText( tr("Data...") );
    }
}

void queryMgr::setModal( spyCfgModel *pModel )
{
    Q_ASSERT( NULL != pModel );
    ui->tableView->setModel( pModel );

    ui->tableView->setItemDelegateForColumn( 1, m_pDelegate1 );
    ui->tableView->setItemDelegateForColumn( 2, m_pDelegate2 );
    ui->tableView->setItemDelegateForColumn( 3, m_pDelegate3 );

    //! build the connection
    connect( this->ui->tableView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             this, SLOT(slot_model_changed()));

    connect( this->ui->tableView->model(), SIGNAL(rowsInserted(QModelIndex,int,int)),
             this, SLOT(slot_model_changed()));

    connect( this->ui->tableView->model(), SIGNAL(rowsRemoved(QModelIndex,int,int)),
             this, SLOT(slot_model_changed()));

    connect( this->ui->tableView->model(), SIGNAL(modelReset()),
             this, SLOT(slot_model_changed()));
}

void queryMgr::attachInstmgr( MegaDevice::InstMgr *pMgr )
{
    Q_ASSERT( NULL != pMgr );

    m_pInstMgr = pMgr;

    //! set chan list
    Q_ASSERT( NULL != m_pDelegate2 );
    QStringList chanList = m_pInstMgr->getChans();
    m_pDelegate2->setItems( chanList );
}
MegaDevice::InstMgr *queryMgr::instMgr()
{
    return m_pInstMgr;
}

void queryMgr::attachSampleThread( sampleThread *pThread )
{
    Q_ASSERT( NULL != pThread );
    m_pSampleThread = pThread;
}

void queryMgr::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex index = ui->tableView->currentIndex();
    if ( !index.isValid() )
    { logDbg(); return; }

    QVariant var = ui->tableView->model()->data( index, Qt::UserRole );
    spyCfgItem *pItem = var.value<spyCfgItem*>();
    Q_ASSERT( NULL != pItem );

    sampleProxy *pSampleProxy = rowSampleProxy( index );
    if ( pSampleProxy == NULL || pSampleProxy->cacheLen()< 1 )
    {
        m_pDataAction->setEnabled( false );
    }
    else
    {
        m_pDataAction->setEnabled( true );
        m_pContextMenu->popup( mapToGlobal( event->pos() ) );
    }
}

void queryMgr::setupUi()
{
    m_pContextMenu = new QMenu(this);
    m_pDataAction = m_pContextMenu->addAction( tr("Data..."), this, SLOT(slot_view_data()) );
    m_pDelegate2 = new comboxDelegate(this);

    //! check box
    m_pDelegate1 =new checkDelegate(this);

    //! chans by inst mgr

    //! items
    m_pDelegate3 = new comboxDelegate(this);
    QStringList strList;
    strList.clear();
    //! \todo read from cfg file
    strList<<tr("TORQUE")<<tr("CYCLE")
           <<tr("SGALL")<<tr("SGSE")
           <<tr("DIST")<<tr("ABSENC");
    m_pDelegate3->setItems( strList );
}
void queryMgr::deleteUi()
{}
void queryMgr::buildConnection()
{
}

void queryMgr::commitItem( int id )
{
    spyCfgModel *pModel = (spyCfgModel*)ui->tableView->model();
    Q_ASSERT( NULL != pModel );

    QList< spyCfgItem *> *pItemList;
    pItemList = pModel->items();
    Q_ASSERT( NULL != pItemList );

    spyCfgItem *pItem;
    pItem = pItemList->at(id);

    Q_ASSERT( pItem != NULL );

    Q_ASSERT( m_pSampleThread != NULL );

    //! convert the meas item
    int queryItem;
    int ret = m_pDelegate3->value( pItem->mType, &queryItem );
    if ( ret != 0 )
    { return; }

    m_pSampleThread->attachSampleProxy(
                (MRQ_REPORT_STATE)queryItem,
                pItem->mSource,
                pItem->mbOnOff,
                pItem->mInterval * 1000000     //! us
                );
}

sampleProxy* queryMgr::rowSampleProxy( QModelIndex &index )
{
    QVariant var = ui->tableView->model()->data( index, Qt::UserRole );
    spyCfgItem *pCfgItem = var.value<spyCfgItem*>();
    if ( NULL == pCfgItem )
    { return NULL; }

    //! item
    int itemId, ret;

    ret = m_pDelegate3->value( pCfgItem->mType, &itemId );
    if ( ret != 0 )
    { return NULL; }

    sampleProxy *pSampleProxy = m_pSampleThread->getSampleProxy( itemId, pCfgItem->mSource );
    return pSampleProxy;
}

void queryMgr::slot_view_data()
{
    QModelIndex index = ui->tableView->currentIndex();
    if ( !index.isValid() )
    { return; }

    //! config item
    QVariant var = ui->tableView->model()->data( index, Qt::UserRole );
    spyCfgItem *pCfgItem = var.value<spyCfgItem*>();

    sampleProxy *pSampleProxy = rowSampleProxy( index );
    if ( NULL == pSampleProxy || pSampleProxy->cacheLen() <  1 )
    { return; }

    //! view model
    spyItemModel *pModel = new spyItemModel;
    spyItem *pItem;

    //! copy to model
    foreach( spyItem *pSpyItem, pSampleProxy->mSpyCache )
    {
        Q_ASSERT( NULL != pSpyItem );

        pItem = new spyItem();
        Q_ASSERT( NULL != pItem );
        *pItem = *pSpyItem;

        pModel->append( pItem );
    }

    //! view
    queryView *pView = new queryView(this);
    Q_ASSERT( NULL != pView );
    pView->setModel( pModel, pCfgItem->mName, pCfgItem->mComment );

    pView->show();
}

void queryMgr::slot_model_changed()
{
    Q_ASSERT( NULL != m_pSampleThread );

    //! stop all at first
    m_pSampleThread->stop();

    if  ( ui->chkSpy->isChecked() )
    {
        //! now append again
        for ( int i = 0; i < ui->tableView->model()->rowCount(); i++ )
        {
            commitItem( i );
        }
    }
}

void queryMgr::slot_spy_changed( const QString &name,
                       int item,
                       int size )
{
    spyCfgModel *pModel;
    pModel = ((spyCfgModel*)ui->tableView->model());
    if ( NULL == pModel )
    { return; }

    QString itemName;
    itemName = m_pDelegate3->toString( item );
    if ( itemName.length() > 0 )
    { }
    else
    { return; }

    pModel->setCacheLen( name, itemName, size );
}

void queryMgr::on_btnAdd_clicked()
{
    int curRow;

    //! current
    curRow = ui->tableView->currentIndex().row();

    ui->tableView->model()->insertRow( curRow + 1 );
    ui->tableView->setCurrentIndex( ui->tableView->model()->index( curRow + 1, 0 ) );
}

void queryMgr::on_btnDel_clicked()
{
    ui->tableView->model()->removeRow( ui->tableView->currentIndex().row() );
}

void queryMgr::on_btnExport_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("spy config (*.spy)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    spyCfgModel *pModel;
    pModel = ((spyCfgModel*)ui->tableView->model());
    Q_ASSERT( NULL != pModel );
    int ret = pModel->save( fDlg.selectedFiles().first() );
    if ( ret == 0 )
    { }
    else
    { }
}

void queryMgr::on_btnLoad_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptOpen );
    fDlg.setNameFilter( tr("spy config (*.spy)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    spyCfgModel *pModel;
    pModel = ((spyCfgModel*)ui->tableView->model());
    Q_ASSERT( NULL != pModel );
    int ret = pModel->load( fDlg.selectedFiles().first() );
    if ( ret == 0 )
    { /*pModel->endResetModel();*/ }
    else
    {}
}

void queryMgr::on_chkSpy_clicked(bool checked)
{
    slot_model_changed();
}

void queryMgr::on_btnClearCache_clicked()
{
    Q_ASSERT( NULL != m_pSampleThread );
    m_pSampleThread->clear();

    slot_model_changed();

    spyCfgModel *pModel;
    pModel = ((spyCfgModel*)ui->tableView->model());
    if ( NULL == pModel )
    { return; }

    pModel->resetCache();
    pModel->trigDataChanged();
}
