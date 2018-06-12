#ifndef _BASE_TYPE_H_
#define _BASE_TYPE_H_

#include <QtCore>

class RpcRequest : public QByteArray
{
//    Q_OBJECT
public:
    enum EnumRequest
    {
        e_req_none = 0,
        e_req_add_t4_key_point,
    };

    enum EnumParaType
    {
        e_type_none = 0,
        e_type_f_f_f,       //! float,float,float
    };

public:
    RpcRequest( EnumRequest eReq = e_req_add_t4_key_point,
                EnumParaType pType = e_type_f_f_f );

public:
    void setRequest( EnumRequest eReq );
    EnumRequest request();

    void setParaType( EnumParaType pType );
    EnumParaType paraType();

    bool checkRequest( EnumRequest eReq,
                       EnumParaType eType ) const ;

public:
    int push( float v );
    int push( int v );

    float popFloat();
    int   popInt();

protected:
    EnumRequest mReq;
    EnumParaType mParaType;
};

Q_DECLARE_METATYPE(RpcRequest)

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
    tpvType mX,mY;
public:
    H2Row()
    {
        mX = 0;
        mY = 0;
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
};

class H2ZRow : public H2Row
{
public:
    tpvType mZ;
public:
    H2ZRow()
    {
        mX = 0;
        mY = 0;
        mZ = 0;
    }
    virtual ~H2ZRow()
    {}

public:
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
    tpvType mZ;

public:
    SinanjuRow()
    {
        mH = 0;
        mZ = 0;
    }

    virtual ~SinanjuRow()
    {}

public:
    void setH( tpvType h )
    { mH = h; }
    tpvType H()
    { return mH; }

    void setZ( tpvType z )
    { mZ = z; }
    tpvType Z()
    { return mZ; }
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
