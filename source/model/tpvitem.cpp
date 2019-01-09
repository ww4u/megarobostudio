#include "tpvitem.h"

static QString _headers[]=
{
    QStringLiteral("chk"),
    QStringLiteral("t(s)"),
    QString("p(" + QString( char_deg ) + ")"),
    QString("v" + QString( char_deg ) + "/s" ),
    QStringLiteral("Comment"),
};

int tpvItem::columns()
{
    return sizeof_array( _headers );
}
QString tpvItem::header( int col )
{
    Q_ASSERT( col >= 0 && col <= sizeof_array(_headers) );

    if ( col == 0 )
    { return QObject::tr("Enable"); }
    else if ( col == 1 )
    { return QObject::tr("t(s)"); }
    else if ( col == 2 )
    { return QObject::tr("p(%1)").arg( char_deg ); }
    else if ( col == 3 )
    { return QString("v(" + QString( char_deg ) + "/s)" ); }
    else
    { return QObject::tr("Comment"); }
}

tpvItem::tpvItem( tpvType t, tpvType p, tpvType v, tpvType rise, tpvType fall )
{
    mValid = 1;

    mT = t;
    mP = p;
    mV = v;

    mRise = rise;
    mFall = fall;
}

tpvItem::tpvItem( const tpvItem &item )
{
    mValid = item.mValid;
    mT = item.mT;
    mP = item.mP;
    mV = item.mV;

    mRise = item.mRise;
    mFall = item.mFall;

    mComment = item.mComment;
}

int tpvItem::serialOut(ImcStream &stream)
{
    stream<<mValid<<COL_SEP<<mT<<COL_SEP<<mP<<COL_SEP<<mV<<COL_SEP<<mComment<<ROW_SEP;

    return 0;
}
int tpvItem::serialIn_tpv( ImcStream &stream )
{
    QString str = stream.readLine();

    QStringList strList = str.split( COL_SEP );

    if ( strList.size() > 0 )
    { mValid = strList[0].toFloat(); }

    if ( strList.size() > 1 )
    { mT = strList[1].toFloat(); }

    if ( strList.size() > 2 )
    { mP = strList[2].toFloat(); }

    if ( strList.size() > 3 )
    { mV = strList[3].toFloat(); }

    if ( strList.size() > 4 )
    { mComment = strList[4]; }

    return 0;
}

int tpvItem::serialIn_pvt( ImcStream &stream )
{
    QString str = stream.readLine();

    QStringList strList = str.split( COL_SEP );

    mValid = true;

    if ( strList.size() > 0 )
    { mP = strList[0].toFloat(); }

    if ( strList.size() > 1 )
    { mV = strList[1].toFloat(); }

    if ( strList.size() > 2 )
    { mT = strList[2].toFloat(); }

    return 0;
}

void tpvItem::setValid( bool b )
{ mValid = b; }
bool tpvItem::getValid()
{ return mValid; }

void tpvItem::setComment( const QString &comment )
{ mComment = comment; }
QString tpvItem::getComment()
{ return mComment; }

void tpvItem::set( tpvType t, tpvType p, tpvType v )
{
    mT = t;
    mP =p;
    mV = v;
}
void tpvItem::setT( tpvType t )
{ mT = t; }
void tpvItem::setP( tpvType p )
{ mP = p; }
void tpvItem::setV( tpvType v )
{ mV = v; }

void tpvItem::setSlope( tpvType rise, tpvType fall )
{
    mRise = rise;
    mFall = fall;
}

tpvType tpvItem::getT()
{ return mT; }
tpvType tpvItem::getP()
{ return mP; }
tpvType tpvItem::getV()
{ return mV; }

tpvType tpvItem::getRise()
{ return mRise; }

tpvType tpvItem::getFall()
{ return mFall; }
