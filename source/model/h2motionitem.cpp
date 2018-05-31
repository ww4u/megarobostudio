#include "h2motionitem.h"

static QString _headers[]=
{
    QStringLiteral("Enable"),
    QStringLiteral("Name"),

    QStringLiteral("t(s)"),
    QStringLiteral("x(mm)"),
    QStringLiteral("y(mm)"),

    QStringLiteral("Comment"),
};

int H2MotionItem::columns()
{
    return sizeof_array(_headers);
}
QString H2MotionItem::header( int col )
{
    Q_ASSERT( col >= 0 && col < H2MotionItem::columns() );

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

    else
    { return QObject::tr("Comment"); }
}

H2MotionItem::H2MotionItem()
{

}

//H2MotionItem &H2MotionItem::operator=( const H2MotionItem &item )
//{
//    //! \todo
//    return *this;
//}

int H2MotionItem::serialOut( ImcStream &stream )
{
    stream<<mbEnable<<COL_SEP
          <<mName<<COL_SEP
          <<mT<<COL_SEP
          <<mX<<COL_SEP
          <<mY<<COL_SEP
          <<mComment<<ROW_SEP;
    return 0;

}
int H2MotionItem::serialIn( ImcStream &stream )
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
    { mComment = strList[5]; }

    return 0;
}
