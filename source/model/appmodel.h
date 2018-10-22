#ifndef APPMODEL_H
#define APPMODEL_H

#include "megatablemodel.h"

class AppItem
{
public:
    static int columns();
    static QString header( int col );
public:
    AppItem();
    ~AppItem();

public:
    void setAutoStart( bool b );
    bool autoStart();

    void setProgram( const QString &prog);
    QString program( );

    void setArgument( const QString &args );
    QString argument( );

    void setComment( const QString &comment );
    QString comment( );
protected:
    bool mbAutoStart;
    QString mProgram;
    QString mArguments;
    QString mComment;
};

class AppModel : public MegaTableModel
{
    Q_OBJECT
public:
    AppModel();
    ~AppModel();

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
    void addItem( bool autoStart,
                  const QString &prog,
                  const QString &args="",
                  const QString &comment="" );

public:
    QList<AppItem *> mItems;
};

#endif // APPMODEL_H
