#include "devicetreewidget.h"

#include "../../device/vrobot.h"

DeviceTreeWidget::DeviceTreeWidget( QWidget *parent ) : QTreeWidget( parent )
{

}

QMimeData *DeviceTreeWidget::mimeData(const QList<QTreeWidgetItem *> items) const
{
    if ( items.size() == 1 )
    {}
    else
    { return NULL; }

    QTreeWidgetItem *pItem = items.at(0);

    QMimeData *pMime = new QMimeData();
    if ( NULL == pMime )
    { return NULL; }

    //! get role
    QVariant var = pItem->data( 0,Qt::UserRole );
    VRobot *pBase = var.value<VRobot *>();

    pMime->setData( "devicemgr/robot", pBase->getClass().toLatin1() );
    pMime->setText( pBase->getName() );
    return pMime;
}

QStringList DeviceTreeWidget::mimeTypes() const
{
    QStringList mimeList;
    return mimeList<<"devicemgr/robot";
}
