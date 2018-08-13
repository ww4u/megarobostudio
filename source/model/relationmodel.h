#ifndef RELATIONMODEL_H
#define RELATIONMODEL_H

#include "megatablemodel.h"

class Relation
{
public:
    static int columns();
    static QString header( int col );

public:
    Relation();
    ~Relation();

public:
    void setName( const QString &name );
    QString name();

    void setValue( const QString &val );
    QString value();

protected:
    QString mName;
    QString mValue;
};

class RelationModel : public MegaTableModel
{
    Q_OBJECT
public:
    RelationModel();
    ~RelationModel();

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
    void addItem( const QString &name, const QString &val );

public:
    QList<Relation *> mItems;
};

#endif // RELATIONMODEL_H
