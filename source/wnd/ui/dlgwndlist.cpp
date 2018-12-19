#include "dlgwndlist.h"
#include "ui_dlgwndlist.h"
#include "../../include/mcdef.h"
#include "../../include/mydebug.h"
dlgWndList::dlgWndList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgWndList)
{
    ui->setupUi(this);
}

dlgWndList::~dlgWndList()
{
    delete ui;
}

void dlgWndList::changeEvent( QEvent *event )
{
    QDialog::changeEvent( event );

    if ( event->type() == QEvent::LanguageChange )
    { ui->retranslateUi( this ); }
}

void dlgWndList::addWnd( const QString &str )
{
    ui->listWidget->addItem( str );

    on_listWidget_itemSelectionChanged();
}
void dlgWndList::addWnd( const QStringList &strList )
{
    ui->listWidget->addItems( strList );

    on_listWidget_itemSelectionChanged();
}

void dlgWndList::clear()
{
    ui->listWidget->clear();
}

void dlgWndList::slot_view_removed( const QString &str )
{
    for ( int i = 0; i < ui->listWidget->count(); i++ )
    {
        if ( str_is( ui->listWidget->item( i )->text(), str ) )
        {
            delete ui->listWidget->takeItem( i );
            break;
        }
    }

    on_listWidget_itemSelectionChanged();
}
void dlgWndList::slot_view_added( const QString &str )
{ ui->listWidget->addItem( str ); }

void dlgWndList::on_btnCloseAll_clicked()
{
    emit signal_closeAll();
    ui->listWidget->clear();

    ui->btnCloseAll->setEnabled( false );
}

void dlgWndList::on_btnActivate_clicked()
{
    if ( ui->listWidget->currentItem() != NULL )
    { emit signal_activeWnd( ui->listWidget->currentItem()->data(Qt::DisplayRole).toString() ); }
}

void dlgWndList::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if ( item != NULL )
    { emit signal_activeWnd( item->data(Qt::DisplayRole).toString() ); }
}

void dlgWndList::on_listWidget_itemSelectionChanged()
{
    ui->btnCloseAll->setEnabled( ui->listWidget->count() > 0 );
    ui->btnActivate->setEnabled( ui->listWidget->count() > 0 && ui->listWidget->currentRow() >= 0 );
}

void dlgWndList::on_listWidget_currentRowChanged(int currentRow)
{
    on_listWidget_itemSelectionChanged();
}

void dlgWndList::on_listWidget_currentTextChanged(const QString &currentText)
{
    on_listWidget_itemSelectionChanged();
}
