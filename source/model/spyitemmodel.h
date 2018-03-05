#ifndef SPYITEMMODEL_H
#define SPYITEMMODEL_H

#include <QtCore>
#include <QAbstractTableModel>

#include "../../include/mcstd.h"

class spyItem
{
public:
    enum viewFormat
    {
        view_asc =  0x1,
        view_hex = 0x02,
        view_int_8 = 0x04,
        view_int_16 = 0x8,
        view_int_32 = 0x10,
        view_float = 0x20,
        view_double = 0x40,

        view_uint_8 = 0x80,
        view_uint_16 = 0x100,
        view_uint_32 = 0x200,

        view_little_endian = 0x0,
        view_big_endian = 0x10000000,
    };

    static int columns();
    static QString header( int index );

    static viewFormat toViewFormat( int fmtIndex );
    static QString toString( viewFormat fmt );

public:
    spyItem();
    spyItem( const QByteArray &ary );
    ~spyItem();

public:
    QString format( viewFormat fmt );

public:
    qint64 mMs;
    QByteArray mVal;
};

class spyItemModel : public QAbstractTableModel
{
public:
    spyItemModel();
    ~spyItemModel();

    void setViewFmt( spyItem::viewFormat );
    spyItem::viewFormat getViewFmt();

public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;

//    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
//    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

//    virtual bool insertRows(int position, int rows, const QModelIndex &parent);
//    virtual bool removeRows(int position, int rows, const QModelIndex &parent);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public:
    void append( const QByteArray &dat );
    void append( spyItem *pItem );

protected:
    QList< spyItem *> mItems;

    spyItem::viewFormat mViewFmt;
};

#endif // SPYITEMMODEL_H
