#ifndef OPITEM_H
#define OPITEM_H

#include <QtCore>

class OpItem
{
public:
    OpItem();
    ~OpItem();

public:
    QString mName;
    QString mRes;
    QString mToolTip;
};

class OpGroup
{
public:
    OpGroup();
    ~OpGroup();

public:
    QList<OpItem*> mItems;
    QString mName;
};

class OpLib
{
public:
    OpLib();
    ~OpLib();

public:
    int load( const QString &fileName );

public:
    QList<OpGroup *> mGroups;
};

#endif // OPITEM_H
