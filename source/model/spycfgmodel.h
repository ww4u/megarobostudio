#ifndef SPYCFGMODEL_H
#define SPYCFGMODEL_H

#include <QtCore>
#include <QAbstractTableModel>

#include "../../include/mcstd.h"

class spyCfgItem
{
public:
    static int columns();
    static QString header( int index );

public:
    spyCfgItem( const QString &name = "" );
    ~spyCfgItem();

    void setCacheLen( int len );
    int cacheLen();

public:
    QString mName;
    bool mbOnOff;
    QString mSource;        //! data source
    QString mType;          //! data item
    double mInterval;
    int mCacheSize;
    QString mComment;

    int mCacheLen;
};

Q_DECLARE_METATYPE( spyCfgItem )
Q_DECLARE_METATYPE( spyCfgItem * )

class spyCfgModel : public QAbstractTableModel
{
public:
    spyCfgModel();
    ~spyCfgModel();

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
    QList< spyCfgItem *> *items();

    spyCfgItem *findItem( const QString &name,
                          const QString &item,
                          int *pRow );

    void setCacheLen( const QString &name, const QString &item, int len );
    int cacheLen( const QString &name, const QString &item );

    int save( const QString &fileName );
    int load( const QString &fileName );

    void fillDbgData();

protected:
    QList< spyCfgItem *> mItems;
};

#endif // SPYCFGMODEL_H
