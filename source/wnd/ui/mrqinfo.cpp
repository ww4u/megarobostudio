#include "mrqinfo.h"
#include "ui_mrqinfo.h"

mrqInfo::mrqInfo(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqInfo)
{
    ui->setupUi(this);

    initModel();

    buildConnection();
}

mrqInfo::~mrqInfo()
{
    delete ui;

    deinitModel();
}

void mrqInfo::buildConnection()
{

}

void mrqInfo::initModel()
{
    QTableWidgetItem *cellItem;

    int rows, cols;
    rows = ui->tableWidget->rowCount();
    cols = ui->tableWidget->columnCount();
    for( int i = 0; i < rows * cols; i++ )
    {
         //! string item
         cellItem = new QTableWidgetItem( "" );
         Q_ASSERT( NULL != cellItem );

         cellItem->setFlags( Qt::ItemIsSelectable );
         mTableItems.append( cellItem );
    }

    for ( int i = 0; i < rows; i++ )
    {
        for ( int j = 0; j < cols; j++ )
        {
            ui->tableWidget->setItem( i, j, mTableItems[ i*cols + j ] );
        }
    }

    //! init data
    mTableItems[0]->setText( tr("Model") );
    mTableItems[2]->setText( tr("Serial") );
    mTableItems[4]->setText( tr("Hardware version") );
    mTableItems[6]->setText( tr("Software version") );

    mTableItems[8]->setText( tr("Firmware version") );
    mTableItems[10]->setText( tr("Boot version") );

    mTableItems[12]->setText( tr("Send ID") );
    mTableItems[14]->setText( tr("Receive ID") );
    mTableItems[16]->setText( tr("Boradcast ID") );
}

void mrqInfo::deinitModel()
{
    foreach( QTableWidgetItem *pItem, mTableItems )
    {
        delete pItem;
    }
}

void mrqInfo::updateData()
{
    if ( m_pMrqModel == NULL )
    { return; }

    //! read only
    MegaDevice::MRQ_model *pModel;
    pModel = m_pMrqModel->getModel();

    Q_ASSERT( NULL != pModel );

    mTableItems[1]->setText( pModel->getDesc() );
    mTableItems[3]->setText( pModel->getSN() );
    mTableItems[5]->setText( pModel->getHwVer() );
    mTableItems[7]->setText( pModel->getSwVer() );

    mTableItems[9]->setText( pModel->getFwVer() );
    mTableItems[11]->setText( pModel->getBtVer() );

    uint32 id;
    pModel->getCAN_SENDID( &id );
    mTableItems[13]->setText( QString::number( id, 16 ) );

    pModel->getCAN_RECEIVEID( &id );
    mTableItems[15]->setText( QString::number( id, 16 ) );

    pModel->getCAN_BROADCASTID( &id );
    mTableItems[17]->setText( QString::number( id, 16 ) );

    ui->labImg->setPixmap( QPixmap::fromImage( pModel->getImage() ) );
}

void mrqInfo::modelChanged()
{
    updateData();
}


