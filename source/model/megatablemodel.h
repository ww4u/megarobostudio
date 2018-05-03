#ifndef MEGATABLEMODEL_H
#define MEGATABLEMODEL_H

#include <QAbstractTableModel>

#include "scriptfile.h"

class MegaTableModel : public QAbstractTableModel, public scriptFile
{
    Q_OBJECT

public:
    MegaTableModel( const QString &className,
                    const QString &name="" );
    virtual ~MegaTableModel()
    {}

public:
    virtual void reverse();

public:
    QString className();

Q_SIGNALS:
    void signal_data_changed();

protected:
    QString mClassName;
};

#endif // MEGATABLEMODEL_H
