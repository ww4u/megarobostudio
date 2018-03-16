#ifndef MOTIONGROUP_H
#define MOTIONGROUP_H

#include <QtCore>
#include "megatablemodel.h"

#include "motionitem.h"

//#include "../../intf/iserial.h"

#include "scriptfile.h"

class motionGroup : public MegaTableModel, public scriptFile
{
    Q_OBJECT

public:
    motionGroup( const QString &name="" );
    virtual ~motionGroup();

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
    motionItem *operator[]( int index );

    template<typename fType >
    int fetech( fType *pT, int skipT,
                 fType *pX, int skipX,
                 fType *pY, int skipY,
                 fType *pZ, int skipZ,
                 int count )
    {
        Q_ASSERT( NULL != pT );
        Q_ASSERT( NULL != pX );
        Q_ASSERT( NULL != pY );
        Q_ASSERT( NULL != pZ );

        int i = 0;
        for ( i = 0; i < mItems.size() && i < count; i++ )
        {
            pT[i*skipT] = mItems[i]->mT;
            pX[i*skipX] = mItems[i]->mX;
            pY[i*skipY] = mItems[i]->mY;
            pZ[i*skipZ] = mItems[i]->mZ;
        }

        return i;
    }

    virtual int save( const QString &fileName );
    virtual int load( const QString &fileName );

public:
    QList< motionItem *> mItems;

};

#endif // MOTIONGROUP_H
