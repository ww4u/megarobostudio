#ifndef SINANJU_MOTIONGROUP_H
#define SINANJU_MOTIONGROUP_H

#include <QtCore>
#include "megatablemodel.h"

#include "sinanjumotionitem.h"

class SinanjuMotionGroup : public MegaTableModel
{
    Q_OBJECT

public:
    SinanjuMotionGroup( const QString &className = "sinanju",
                        const QString &name="" );
    virtual ~SinanjuMotionGroup();

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
    SinanjuMotionItem *operator[]( int index );

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
    QList< SinanjuMotionItem *> mItems;
};

#endif // MOTIONGROUP_H
