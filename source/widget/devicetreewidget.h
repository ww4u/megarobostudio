#ifndef DEVICETREEWIDGET_H
#define DEVICETREEWIDGET_H

#include <QTreeWidget>

class DeviceTreeWidget : public QTreeWidget
{
public:
    DeviceTreeWidget( QWidget *parent = Q_NULLPTR );

protected:
    virtual QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const;
    virtual QStringList mimeTypes() const;
};

#endif // DEVICETREEWIDGET_H
