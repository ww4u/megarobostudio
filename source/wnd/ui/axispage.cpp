#include "axispage.h"
//#include "ui_axispage.h"

AxisPage::AxisPage(QWidget *parent) :
    ValueComboBox(parent)
{
//    ui->setupUi(this);

    //! \note combo
    addItem( tr("MAIN"),0 );
    addItem( tr("SMALL"),1 );
    for ( int i = 1; i <= 8; i++ )
    {
        addItem( QString( tr("P%1") ).arg(i), i+1 );
    }

    //! page list
    mPageList<<"MAIN"<<"SMALL";
    for ( int i = 1; i <= 22; i++ )
    {
        mPageList<<QString("P%1").arg( i );
    }

    //! default to 1
    setCurrentIndex( 1 );
}

AxisPage::~AxisPage()
{
//    delete ui;
}

void AxisPage::setPage( int page )
{ setCurrentIndex( page ); }

int AxisPage::page()
{ return currentIndex(); }

void AxisPage::setMaxPage( int maxPage )
{
    //! clear item
    clear();

    //! change page count
    for ( int i = 0; i < maxPage && i < mPageList.size(); i++ )
    {
        addItem( mPageList.at(i), i );
    }
}
