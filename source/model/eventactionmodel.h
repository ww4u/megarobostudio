#ifndef EVENTACTIONMODEL_H
#define EVENTACTIONMODEL_H

#include <QtCore>
#include "megatablemodel.h"

enum deviceEvent
{
    e_device_event_none,
    e_device_event_lose_step,
    e_device_event_over_pe,  //! over_power_efficency

};

enum deviceAction
{
    e_device_action_none,
    e_device_action_stop,
    e_device_action_prompt,
};

class EventAction
{
public:
    static int columns();
    static QString header( int col );

public:
    EventAction();
    ~EventAction();

public:
    void setEnable( bool b );
    bool enable();

    void setEvent( const QString& evt );
    QString event();

    void setAction( const QString& act );
    QString action();

    void setComment( const QString &cmt );
    QString comment();

protected:
    bool mEnable;
    QString mEvent;
    QString mAction;
    QString mComment;
};

class EventActionModel : public MegaTableModel
{
    Q_OBJECT
public:
    EventActionModel();
    ~EventActionModel();

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
    QList< EventAction *> *items();

    int save( const QString &fileName );
    int load( const QString &fileName );

    int serialOut( QXmlStreamWriter & writer );
    int serialIn( QXmlStreamReader & reader );

protected:
    QList< EventAction *> mItems;

};

#endif // EVENTACTIONMODEL_H
