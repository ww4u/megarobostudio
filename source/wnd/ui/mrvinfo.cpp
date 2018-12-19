#include "mrvinfo.h"
#include "ui_mrvinfo.h"

mrvInfo::mrvInfo(QWidget *parent) :
    mrvView(parent),
    ui(new Ui::mrvInfo)
{
    ui->setupUi(this);

    initModel();

    buildConnection();

    SET_INFO_ATTR();
}

mrvInfo::~mrvInfo()
{
    delete ui;

    deinitModel();
}

void mrvInfo::changeEvent(QEvent * event)
{
    mrvView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi( this );

        translate();
    }
}

void mrvInfo::buildConnection()
{

}

void mrvInfo::initModel()
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
    translate();
}

void mrvInfo::deinitModel()
{

}

void mrvInfo::translate()
{
    mTableItems.at(0)->setText( tr("Model") );
    mTableItems.at(2)->setText( tr("Serial") );
    mTableItems.at(4)->setText( tr("Hardware version") );
    mTableItems.at(6)->setText( tr("Software version") );

    mTableItems.at(8)->setText( tr("Boot version") );

    mTableItems.at(10)->setText( tr("Send ID") );
    mTableItems.at(12)->setText( tr("Receive ID") );
    mTableItems.at(14)->setText( tr("Boradcast ID") );
    mTableItems.at(16)->setText( tr("Signature") );
}

void mrvInfo::updateUi()
{
    if ( m_pProxy == NULL )
    { return; }

    Q_ASSERT( NULL != m_pProxy );

    mTableItems.at(1)->setText( m_pProxy->getDesc() );
    mTableItems.at(3)->setText( m_pProxy->getSN() );
    mTableItems.at(5)->setText( m_pProxy->getHwVer() );
    mTableItems.at(7)->setText( m_pProxy->getSwVer() );

    mTableItems.at(9)->setText( m_pProxy->getBtVer() );

    uint32 id;
    m_pProxy->getCAN_SENDID( &id );
    mTableItems.at(11)->setText( "0X" + QString::number( id, 16 ).toUpper() );

    m_pProxy->getCAN_RECEIVEID( &id );
    mTableItems.at(13)->setText( "0X" + QString::number( id, 16 ).toUpper() );

    m_pProxy->getCAN_BROADCASTID( &id );
    mTableItems.at(15)->setText( "0X" + QString::number( id, 16 ).toUpper() );

    id = m_pProxy->getSignature();
    mTableItems.at(17)->setText( "0X" + QString::number( id, 16 ).toUpper() );

    //! image
    ui->labImg->setPixmap( QPixmap::fromImage( m_pMRV->getImage() ) );
}

void mrvInfo::modelChanged()
{
    updateUi();
}

