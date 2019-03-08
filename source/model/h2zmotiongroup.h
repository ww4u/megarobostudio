#ifndef H2ZMOTIONGROUP_H
#define H2ZMOTIONGROUP_H

#include "megatablemodel.h"
#include "h2zmotionitem.h"

class H2ZMotionGroup : public MegaTableModel
{
    Q_OBJECT
public:
    H2ZMotionGroup( const QString &className = "h2z", const QString &fileName= "" );
    virtual ~H2ZMotionGroup();

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
    H2ZMotionItem *operator[]( int index );

public:
    virtual int save( const QString &fileName );
    virtual int load( const QString &fileName );

public:
    virtual void reverse();
    virtual void autoTime( double speed, double speedTerminal,
                           int align, int div );

    virtual tpvType getAbsT( int index );
protected:
    virtual void switchTimeType( timeType pre, timeType nxt );

public:
    QList< H2ZMotionItem *> mItems;
};

#endif // H2ZMOTIONGROUP_H
