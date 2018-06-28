#ifndef TPGROUP_H
#define TPGROUP_H

#include <QtCore>
#include "megatablemodel.h"
#include "tpitem.h"

class TpGroup : public MegaTableModel
{
public:
    TpGroup();
    virtual ~TpGroup();

public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual bool insertRows(int position, int rows, const QModelIndex &parent);
    virtual bool removeRows(int position, int rows, const QModelIndex &parent);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public:
    TpItem * operator[]( int id );
    QList< TpItem *> * getRows();
    QList< TpItem *> * getRows( QList<TpRow*> &rows );

public:
    int addItem( tpvType t, tpvType p );
    int removeItem( tpvType t );

    int remove( int id );
    int clear();

public:
    int save( const QString &fileName );
    int load( const QString &fileName );

protected:
    TpItem * findItem( tpvType t );

public:
    QList< TpItem *> mItems;
};

#endif // TPGROUP_H
