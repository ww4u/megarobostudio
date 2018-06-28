#include "tpitem.h"

static QString _headers[]=
{
    QStringLiteral("t(s)"),
    QString("p(\%)"),
};

int TpItem::columns()
{
    return sizeof_array( _headers );
}

QString TpItem::header( int col )
{
    Q_ASSERT( col >= 0 && col <= sizeof_array(_headers) );

    if ( col == 0 )
    { return QObject::tr("t(s)"); }
    else if ( col == 1 )
    { return QObject::tr("p(\%)"); }
    else
    { return ""; }
}

TpItem::TpItem( tpvType t, tpvType p )
{
    mT = t;
    mP = p;
}

TpItem::TpItem( const TpItem &item )
{
    mT = item.mT;
    mP = item.mP;
}

int TpItem::serialOut(ImcStream &stream)
{
    stream<<mT<<COL_SEP<<mP<<ROW_SEP;

    return 0;
}
int TpItem::serialIn( ImcStream &stream )
{
    QString str = stream.readLine();

    QStringList strList = str.split( COL_SEP );

    if ( strList.size() > 0 )
    { mT = strList[0].toFloat(); }
    else
    { return -1; }

    if ( strList.size() > 1 )
    { mP = strList[1].toFloat(); }
    else
    { return -1; }

    return 0;
}

void TpItem::set( tpvType t, tpvType p )
{
    mT = t;
    mP = p;
}

void TpItem::setT( tpvType t )
{ mT = t; }
void TpItem::setP( tpvType p )
{ mP = p; }

tpvType TpItem::getT()
{ return mT; }
tpvType TpItem::getP()
{ return mP; }
