#ifndef TPVGROUP_H
#define TPVGROUP_H

#include <QtCore>
#include "megatablemodel.h"
#include "tpvitem.h"

class tpvGroup : public MegaTableModel
{
    Q_OBJECT

public:
    enum pvtType
    {
        pvt_unk,
        pvt_tpv,
        pvt_pvt,
    };

    static tpvGroup* createDemoGroup1();
    static tpvGroup* createDemoGroup2();
    static tpvGroup* createDemoGroup3();
    static tpvGroup* createDemoGroup4();
    static tpvGroup* createDemoGroup5();

    static tpvGroup* createDemoGroup( float dT=0.5f, float dP=360.0f ); 

public:
    static int slopelize( QList<tpvGroup*> gps,
                          QList<int> planModes,
                          QList<float> rises,
                          QList<float> falls,
                          int *pErrGp, int *pErrRow );

public:
    tpvGroup();
    virtual ~tpvGroup();

public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual bool insertRows(int position, int rows, const QModelIndex &parent);
    virtual bool removeRows(int position, int rows, const QModelIndex &parent);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);
public:
    tpvItem * operator[]( int id );
    QList< tpvItem *> * getRows();
    QList< tpvItem *> * getRows( QList<tpvRow*> &rows );
    QList< tpvItem *> * getRows( QList<tpvRow> &rows );

    void trimRows( tpvGroup &gp );
    void abstimeRows( tpvGroup &gp );

    //! 0 - cubic
    //! 1 - trapzoid
    //! 2 - s curve
    int slopelize( int motionMode,
                   float rise, float fall,
                   int *errRow );

public:
    int addItem( tpvType t, tpvType p, tpvType v=0, tpvType rise=0, tpvType fall=0 );
    int removeItem( tpvType t );

    int remove( int id );
    int clear();

    virtual void rstAbsT();
    virtual tpvType getAbsT( int index );
public:
    int save( const QString &fileName );
    int load( const QString &fileName, pvtType flt );

protected:
    tpvItem * findItem( tpvType t );
    virtual void switchTimeType( timeType pre, timeType nxt );

public:
    QList< tpvItem *> mItems;

    QVariantList mVHeader, mHHeader;

};

#endif // TPVGROUP_H
