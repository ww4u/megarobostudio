#include "h2zmotionitem.h"

static QString _headers[]=
{
    QStringLiteral("Enable"),
    QStringLiteral("Name"),

    QStringLiteral("t(s)"),
    QStringLiteral("x(mm)"),
    QStringLiteral("y(mm)"),
    QStringLiteral("z(mm)"),

    QStringLiteral("Comment"),
};

int H2ZMotionItem::columns()
{
    return sizeof_array(_headers);
}
QString H2ZMotionItem::header( int col )
{
    Q_ASSERT( col >= 0 && col < H2ZMotionItem::columns() );

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

    else
    { return QObject::tr("Comment"); }
}

H2ZMotionItem::H2ZMotionItem()
{
    mT = 0;
    mX = 0;
    mY = 0;
    mZ = 0;
}

int H2ZMotionItem::serialOut( ImcStream &stream )
{
    stream<<mbEnable<<COL_SEP
          <<mName<<COL_SEP
          <<mT<<COL_SEP
          <<mX<<COL_SEP
          <<mY<<COL_SEP
          <<mZ<<COL_SEP
          <<mComment<<ROW_SEP;
    return 0;

}
int H2ZMotionItem::serialIn( ImcStream &stream )
{
    QString str = stream.readLine();

    QStringList strList = str.split( COL_SEP);

    if ( strList.size() > 0 )
    { mbEnable = strList.at(0).toInt() > 0; }

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
    { mComment = strList[5]; }

    return 0;
}
