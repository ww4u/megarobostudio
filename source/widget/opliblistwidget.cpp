#include "opliblistwidget.h"
#include <QtCore>

OpLibListWidget::OpLibListWidget( QWidget *parent ):QListWidget(parent)
{
    setDragDropMode( QAbstractItemView::DragOnly );
    setViewMode( QListView::IconMode );
}

QMimeData *OpLibListWidget::mimeData(const QList<QListWidgetItem *> items) const
{
    if ( items.size() == 1 )
    {}
    else
    { return NULL; }

    QListWidgetItem *pItem = items.at(0);

    QMimeData *pMime = new QMimeData();
    if ( NULL == pMime )
    { return NULL; }


    pMime->setText( pItem->text() );
    pMime->setData( "oplib/operator", pItem->data( Qt::UserRole).toByteArray() );


    return pMime;
}

QStringList OpLibListWidget::mimeTypes() const
{
    QStringList mimeList;
    return mimeList<<"oplib/operator";
}
