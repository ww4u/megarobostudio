#ifndef MEGATABLEMODEL_H
#define MEGATABLEMODEL_H

#include <QAbstractTableModel>

class MegaTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MegaTableModel();

Q_SIGNALS:
    void signal_data_changed();
};

#endif // MEGATABLEMODEL_H
