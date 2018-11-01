#include "sinanjumotionitem.h"

static QString _headers[]=
{
    QStringLiteral("Enable"),
    QStringLiteral("Name"),
    QStringLiteral("t(s)"),
    QStringLiteral("x(mm)"),
    QStringLiteral("y(mm)"),
    QStringLiteral("z(mm)"),
    QStringLiteral("h(mm)"),
    QStringLiteral("Interp"),
    QStringLiteral("Comment"),
};

int SinanjuMotionItem::columns()
{
    return sizeof_array(_headers);
}
QString SinanjuMotionItem::header( int col )
{
    Q_ASSERT( col >= 0 && col < SinanjuMotionItem::columns() );

    if ( col == 0 )
    { return QObject::tr("Enable"); }
    else if ( col == 1 )
    { return QObject::tr("Name"); }
    else if ( col == 2 )
    { return QObject::tr("t(s)"); }
    else if ( col == 3 )
    { return QObject::tr("x(mm)"); }
    else if ( col == 4 )
    { return QObject::tr("y(mm)"); }
    else if ( col == 5 )
    { return QObject::tr("z(mm)"); }
    else if ( col == 6 )
    { return QObject::tr("h(mm)"); }
    else if ( col == 7 )
    { return QObject::tr("Interp"); }
    else
    { return QObject::tr("Comment"); }
}

QString SinanjuMotionItem::encodeAttr( quint32 attr )
{
    if ( attr != 0 )
    { return QString("line=( enable = 1;)"); }
    else
    { return QString(); }
}

quint32 SinanjuMotionItem::decodeAttr( const QString &str )
{
    if ( str.isEmpty() )
    { return 0; }
    else
    { return 1; }
}

SinanjuMotionItem::SinanjuMotionItem()
{
}

SinanjuMotionItem::SinanjuMotionItem( const SinanjuMotionItem &item )
{
    *this = item;
}

SinanjuMotionItem &SinanjuMotionItem::operator=( const SinanjuMotionItem &item )
{
    //! SheetRow
    mbEnable = item.mbEnable;
    mName = item.mName;
    mComment = item.mComment;

    //! comm row
    mT = item.mT;
    mIAttr = item.mIAttr;

    //! sinanju row
    mX = item.mX;
    mY = item.mY;
    mZ = item.mZ;
    mH = item.mH;

    return *this;
}

int SinanjuMotionItem::serialOut( ImcStream &stream )
{
    stream<<mbEnable<<COL_SEP
          <<mName<<COL_SEP
          <<mT<<COL_SEP
          <<mX<<COL_SEP
          <<mY<<COL_SEP
          <<mZ<<COL_SEP
          <<mH<<COL_SEP
          <<mIAttr<<COL_SEP
          <<mComment<<ROW_SEP;

    return 0;
}
int SinanjuMotionItem::serialIn( ImcStream &stream )
{
    QString str = stream.readLine();

    QStringList strList = str.split( COL_SEP );

    if ( strList.size() > 0 )
    { mbEnable = strList[0].toInt() > 0; }
    else
    { return -1; }

    if ( strList.size() > 1 )
    { mName = strList[1]; }

    if ( strList.size() > 2 )
    { mT = strList[2].toFloat(); }

    if ( strList.size() > 3 )
    { mX = strList[3].toFloat(); }

    if ( strList.size() > 4 )
    { mY = strList[4].toFloat(); }

    if ( strList.size() > 5 )
    { mZ = strList[5].toFloat(); }

    if ( strList.size() > 6 )
    { mH = strList[6].toFloat(); }

    if ( strList.size() > 7 )
    { mIAttr = strList[7].toUInt(); }

    if ( strList.size() > 8 )
    { mComment = strList[8]; }

    return 0;
}


