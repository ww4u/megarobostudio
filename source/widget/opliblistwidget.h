#ifndef OPLIBLISTWIDGET_H
#define OPLIBLISTWIDGET_H

#include <QListWidget>

class OpLibListWidget : public QListWidget
{
public:
    OpLibListWidget( QWidget *parent=Q_NULLPTR );

protected:
    virtual QMimeData *mimeData(const QList<QListWidgetItem *> items) const;
    virtual QStringList mimeTypes() const;
};

#endif // OPLIBLISTWIDGET_H
