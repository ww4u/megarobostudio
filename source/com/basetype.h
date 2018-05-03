#ifndef _BASE_TYPE_H_
#define _BASE_TYPE_H_

#define tpvType float
#define time_eq( a, b )     ( fabs( (a)-(b) ) <= 1e-6f )
class tpvRow{
public:
        union
        {
            struct
            {
                tpvType mT;
                tpvType mP;
                tpvType mV;
            };
            tpvType datas[3];
        };
        bool mbGc;              //! gc on used
public:
        tpvRow( )
        {
            mbGc = false;
            mT = 0;
            mP = 0;
            mV = 0;
        }

        void setGc( bool b )
        { mbGc = b; }

        bool gc()
        { return mbGc; }
};
typedef QList< tpvRow *>  tpvList;

class SheetRow
{
public:
    bool    mbEnable;
    QString mComment;       //! comment of the row
    QString mName;          //! name

public:
    SheetRow()
    {  mbEnable = true; }
    virtual ~SheetRow()
    {}

public:
    void setEnable( bool bEn )
    { mbEnable = bEn; }
    bool enable()
    { return mbEnable; }

    void setComment( const QString &comment )
    { mComment = comment; }
    QString comment()
    { return mComment; }

    void setName( const QString &name )
    { mName = name; }
    QString name()
    { return mName; }
};

class MotionRow
{
public:
    tpvType mT;
    quint32 mIAttr;

public:
    MotionRow()
    {
        mT = 0;
        mIAttr = 1;
    }
    virtual ~MotionRow()
    {}

public:
    void setT( tpvType t )
    { mT = t; }
    tpvType T()
    { return mT; }

    void setIAttr( quint32 iAttr )
    { mIAttr = iAttr; }
    quint32 IAttr()
    { return mIAttr; }
};

class H2Row : public MotionRow
{
public:
    tpvType mX,mY,mZ;
public:
    H2Row()
    {
        mX = 0;
        mY = 0;
        mZ = 0;
    }
    virtual ~H2Row()
    {}

public:
    void setX( tpvType v )
    { mX = v; }
    tpvType X()
    { return mX; }

    void setY( tpvType v )
    { mY = v; }
    tpvType Y()
    { return mY; }

    void setZ( tpvType v )
    { mZ = v; }
    tpvType Z()
    { return mZ; }
};

//! t,x,y,z,h
class SinanjuRow : public H2Row
{
public:
    tpvType mH;

public:
    SinanjuRow()
    {
        mH = 0;
    }

    virtual ~SinanjuRow()
    {}

public:
    void setH( tpvType h )
    { mH = h; }
    tpvType H()
    { return mH; }
};

//! t,fx,fz,bx,bz,ly,ry
class MegatronRow : public MotionRow
{
public:
    tpvType mFx, mFz, mBx, mBz, mLy, mRy;

public:
    MegatronRow()
    {

        mFx = 0;
        mBx = 0;
        mFz = 0;
        mBz = 0;
        mLy = 0;
        mRy = 0;
    }
    virtual ~MegatronRow()
    {}

    void setFx( tpvType v )
    { mFx = v; }
    tpvType Fx()
    { return mFx; }

    void setFz( tpvType v )
    { mFz = v; }
    tpvType Fz()
    { return mFz; }

    void setBx( tpvType v )
    { mBx = v; }
    tpvType Bx()
    { return mBx; }

    void setBz( tpvType v )
    { mBz = v; }
    tpvType Bz()
    { return mBz; }

    void setLy( tpvType v )
    { mLy = v; }
    tpvType Ly()
    { return mLy; }

    void setRy( tpvType v )
    { mRy = v; }
    tpvType Ry()
    { return mRy; }

};


class tpvRegion
{
public:
    //! member
    int mAx;
    int mPage;

    //! prop
    int axes() const
    { return mAx; }
    int page() const
    { return mPage; }

    //! ctor
    tpvRegion( int ax = 0, int page = 0 )
    {
        mAx = ax;
        mPage = page;
    }

    tpvRegion &operator=( const tpvRegion &region )
    {
        mAx = region.axes();
        mPage = region.page();

        return *this;
    }

    tpvRegion ( const tpvRegion & region )
    {
        mAx = region.mAx;
        mPage = region.mPage;
    }

    tpvRegion &operator=( int ax )
    {
        mAx = ax;
        mPage = 0;

        return *this;
    }

//    bool operator==( const tpvRegion &region ) const
//    {
//        if ( mAx != region.axes() )
//        { return false; }
//        if ( mPage != region.page() )
//        { return false; }

//        return true;
//    }

//    void set( int ax, int page )
//    {
//        mAx = ax;
//        mPage = page;
//    }

    void setRegion( int ax, int page )
    {
        mAx = ax;
        mPage = page;
    }

    void setAx( int ax )
    { mAx = ax; }
    void setPage( int page )
    { mPage = page; }
};

inline bool operator<( const tpvRegion &region1,
                const tpvRegion &region2 )
{
//    if ( region1.axes() < region2.axes() )
//    { return true; }
//    if ( region1.page() < region2.page() )
//    { return true; }
//    return false;

    if ( region1.axes() != region2.axes() )
    { return region1.axes() < region2.axes(); }
    else
    { return region1.page() < region2.page(); }
}

//inline bool operator==( const tpvRegion &region1,
//                const tpvRegion &region2 )
//{
//    if ( region1.axes() != region2.axes() )
//    { return false; }
//    if ( region1.page() != region2.page() )
//    { return false; }

//    return true;
//}

//inline bool operator>( const tpvRegion &region1,
//                const tpvRegion &region2 )
//{
//    if ( region1.axes() > region2.axes() )
//    { return true; }
//    if ( region1.page() > region2.page() )
//    { return true; }

//    return false;
//}

#include <QtCore>
Q_DECLARE_METATYPE(tpvRegion)

#define TPV_REGEION_TYPE_ID      (QMetaType::Type)(QMetaType::User )
#define ROBO_MSG_TYPE_ID         (QMetaType::Type)(QMetaType::User )


template <typename T>
class xxxGroup
{
protected:
    int mSize;
    T *mDatas;

public:
    void clear()
    {
        mSize = 0;
        if ( NULL != mDatas )
        {
            delete []mDatas;
            mDatas = NULL;
        }
    }
    int size()
    { return mSize; }
    T *data()
    { return mDatas; }

    int alloc( int n )
    {
        Q_ASSERT( n > 0 );
        mDatas = new T[ n ];
        if ( NULL == mDatas )
        { return -1; }

        mSize = n;
        return 0;
    }

public:
    xxxGroup()
    {
        mSize = 0;
        mDatas = NULL;
    }

    ~xxxGroup()
    {
        clear();
    }
};

#endif
