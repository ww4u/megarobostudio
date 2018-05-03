#include "megatablemodel.h"

MegaTableModel::MegaTableModel( const QString &className,
                                const QString &name ) : scriptFile(name)
{
    mClassName = className;

    connect( this, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(rowsInserted(QModelIndex,int,int)),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(rowsRemoved(QModelIndex,int,int)),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(modelReset()),
             this, SIGNAL(signal_data_changed()));
}

void MegaTableModel::reverse()
{}

QString MegaTableModel::className()
{ return mClassName; }

//bool MegaTableModel::verify()
//{ return false; }

//int  MegaTableModel::build()
//{ return 0; }
