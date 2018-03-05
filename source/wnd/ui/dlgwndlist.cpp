#include "dlgwndlist.h"
#include "ui_dlgwndlist.h"

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

void dlgWndList::on_btnCloseAll_clicked()
{
    emit signal_closeAll();
    ui->listWidget->clear();
}

void dlgWndList::on_btnActivate_clicked()
{
    emit signal_activeWnd( ui->listWidget->currentItem()->data(Qt::DisplayRole).toString() );
}

void dlgWndList::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    emit signal_activeWnd( item->data(Qt::DisplayRole).toString() );
}

void dlgWndList::on_listWidget_itemSelectionChanged()
{
    ui->btnCloseAll->setEnabled( ui->listWidget->count() > 0 );
    ui->btnActivate->setEnabled( ui->listWidget->count() > 0 && ui->listWidget->currentRow() >= 0 );
}
