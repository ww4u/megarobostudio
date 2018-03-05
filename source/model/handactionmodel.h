#ifndef HANDACTIONMODEL_H
#define HANDACTIONMODEL_H

#include <QtCore>
#include <QAbstractTableModel>

class handAction
{
public:
    static int columns();
    static QString header( int col );

public:
    handAction();
    ~handAction();

public:
    void set( const QString &name,
              double value,
              const QString &comment = "" );

    void setName( const QString &name );
    QString name();

    void setValue( double value );
    double value();

    void setComment( const QString &comment );
    QString comment();

protected:
    QString mName;
    double mValue;
    QString mComment;
};

class handActionModel : public QAbstractTableModel
{
public:
    handActionModel();
    virtual ~handActionModel();

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
    int save( const QString &fileName );
    int load( const QString &fileName );

    int serialOut( QXmlStreamWriter & writer );
    int serialIn( QXmlStreamReader & reader );

    QMap< QString, double > actionMap();
protected:
    QList< handAction *> mItems;

};

#endif // HANDACTIONMODEL_H
