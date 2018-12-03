#ifndef H2MOTIONGROUP_H
#define H2MOTIONGROUP_H

#include <QtCore>
#include "megatablemodel.h"
#include "h2motionitem.h"

class H2MotionGroup : public MegaTableModel
{
    Q_OBJECT
public:
    H2MotionGroup( const QString &className = "h2", const QString &fileName= "");
    virtual ~H2MotionGroup();

public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual bool insertRows(int position, int rows, const QModelIndex &parent);
    virtual bool removeRows(int position, int rows, const QModelIndex &parent);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual void setRpc( int row, RpcRequest &req);
public:
    H2MotionItem *operator[]( int index );

public:
    virtual int save( const QString &fileName );
    virtual int load( const QString &fileName );

public:
    virtual void reverse();
    virtual void autoTime( double speed, double speedTerminal );

public:
    QList< H2MotionItem *> mItems;
};

#endif // H2MOTIONGROUP_H
