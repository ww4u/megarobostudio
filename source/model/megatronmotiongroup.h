#ifndef MEGATRONMOTIONGROUP_H
#define MEGATRONMOTIONGROUP_H

#include <QtCore>
#include "megatablemodel.h"
#include "megatronmotionitem.h"

class MegatronMotionGroup : public MegaTableModel
{
    Q_OBJECT
public:
    MegatronMotionGroup( const QString &className = "megatron", const QString &name="" );
    virtual ~MegatronMotionGroup();

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
    MegatronMotionItem *operator[]( int index );

public:
    virtual int save( const QString &fileName );
    virtual int load( const QString &fileName );

public:
    virtual void reverse();

    virtual tpvType getAbsT( int index );
protected:
    virtual void switchTimeType( timeType pre, timeType nxt );

public:
    QList< MegatronMotionItem *> mItems;
};

#endif // MEGATRONMOTIONGROUP_H
