#include "mrqinfo.h"
#include "ui_mrqinfo.h"

mrqInfo::mrqInfo(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqInfo)
{
    ui->setupUi(this);

    initModel();

    buildConnection();

    SET_INFO_ATTR();
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
    mTableItems.at(0)->setText( tr("Model") );
    mTableItems.at(2)->setText( tr("Serial") );
    mTableItems.at(4)->setText( tr("Hardware version") );
    mTableItems.at(6)->setText( tr("Software version") );

    mTableItems.at(8)->setText( tr("Firmware version") );
    mTableItems.at(10)->setText( tr("Boot version") );

    mTableItems.at(12)->setText( tr("Send ID") );
    mTableItems.at(14)->setText( tr("Receive ID") );
    mTableItems.at(16)->setText( tr("Boradcast ID") );
    mTableItems.at(18)->setText( tr("Signature") );
    mTableItems.at(20)->setText( tr("Capacity") );
    mTableItems.at(22)->setText( tr("Buffer") );
}

void mrqInfo::deinitModel()
{
//    foreach( QTableWidgetItem *pItem, mTableItems )
//    {
//        delete pItem;
//    }
}

void mrqInfo::updateUi()
{
    if ( m_pMrqModel == NULL )
    { return; }

    //! read only
    MegaDevice::MRQ_model *pModel;
    pModel = m_pMrqModel->getModel();

    Q_ASSERT( NULL != pModel );

    mTableItems.at(1)->setText( pModel->getDesc() );
    mTableItems.at(3)->setText( pModel->getSN() );
    mTableItems.at(5)->setText( pModel->getHwVer() );
//    mTableItems.at(7)->setText( pModel->getSwVer() );
    mTableItems.at(7)->setText( pModel->getSeqVer() );

    mTableItems.at(9)->setText( pModel->getFwVer() );
    mTableItems.at(11)->setText( pModel->getBtVer() );

    uint32 id;
    pModel->getCAN_SENDID( &id );
    mTableItems.at(13)->setText( "0X" + QString::number( id, 16 ).toUpper() );

    pModel->getCAN_RECEIVEID( &id );
    mTableItems.at(15)->setText( "0X" + QString::number( id, 16 ).toUpper() );

    pModel->getCAN_BROADCASTID( &id );
    mTableItems.at(17)->setText( "0X" + QString::number( id, 16 ).toUpper() );

    id = pModel->getSignature();
    mTableItems.at(19)->setText( "0X" + QString::number( id, 16 ).toUpper() );

    //! caps
    int cap;
    QString strCaps;
    QString sep;
    for ( int i = 0; i < m_pMrqModel->regions(); i++ )
    {
        cap = m_pMrqModel->getTpvCap( tpvRegion(0,i) );
        strCaps += sep + QString::number( cap );
        sep = "/";
    }
    mTableItems.at(21)->setText( strCaps );

    //! buffers
    int bufSize;
    QString strBufs;
    sep = "";
    for ( int i = 0; i < m_pMrqModel->regions(); i++ )
    {
        bufSize = m_pMrqModel->getTpvBuf( tpvRegion(0,i) );
        strBufs += sep + QString("%1M").arg( bufSize / (1024*1024.0));
        sep = "/";
    }
    mTableItems.at(23)->setText( strBufs );

    //! image
    ui->labImg->setPixmap( QPixmap::fromImage( pModel->getImage() ) );
}

void mrqInfo::modelChanged()
{
    updateUi();
}


