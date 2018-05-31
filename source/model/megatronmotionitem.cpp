#include "megatronmotionitem.h"

static QString _headers[]=
{
    QStringLiteral("Enable"),
    QStringLiteral("Name"),

    QStringLiteral("t(s)"),
    QStringLiteral("Fx(mm)"),
    QStringLiteral("Fz(mm)"),
    QStringLiteral("Bx(mm)"),
    QStringLiteral("Bz(mm)"),
    QStringLiteral("Ly(mm)"),
    QStringLiteral("Ry(mm)"),

    QStringLiteral("Comment"),
};

int MegatronMotionItem::columns()
{
    return sizeof_array(_headers);
}
QString MegatronMotionItem::header( int col )
{
    Q_ASSERT( col >= 0 && col < MegatronMotionItem::columns() );

    if ( col == 0 )
    { return QObject::tr("Enable"); }
    else if ( col == 1 )
    { return QObject::tr("Name"); }

    else if ( col == 2 )
    { return QObject::tr("t(s)"); }

    else if ( col == 3 )
    { return QObject::tr("Fx(mm)"); }
    else if ( col == 4 )
    { return QObject::tr("Fz(mm)"); }
    else if ( col == 5 )
    { return QObject::tr("Bx(mm)"); }
    else if ( col == 6 )
    { return QObject::tr("Bz(mm)"); }
    else if ( col == 7 )
    { return QObject::tr("Ly(mm)"); }
    else if ( col == 8 )
    { return QObject::tr("Ry(mm)"); }

    else
    { return QObject::tr("Comment"); }
}

MegatronMotionItem::MegatronMotionItem()
{
}

MegatronMotionItem::MegatronMotionItem( const MegatronMotionItem &item )
{
    *this = item;
}


int MegatronMotionItem::serialOut( ImcStream &stream )
{
    stream<<mbEnable<<COL_SEP
          <<mName<<COL_SEP
          <<mT<<COL_SEP
          <<mFx<<COL_SEP
          <<mFz<<COL_SEP
          <<mBx<<COL_SEP
          <<mBz<<COL_SEP
          <<mLy<<COL_SEP
          <<mRy<<COL_SEP
          <<mComment<<ROW_SEP;
    return 0;
}
int MegatronMotionItem::serialIn( ImcStream &stream )
{
    QString str = stream.readLine();

    QStringList strList = str.split( COL_SEP );

    if ( strList.size() > 0 )
    { mbEnable = strList.at(0).toInt() > 0; }

    if ( strList.size() > 1 )
    { mName = strList[1]; }

    if ( strList.size() > 2 )
    { mT = strList[2].toFloat(); }

    if ( strList.size() > 3 )
    { mFx = strList[3].toFloat(); }

    if ( strList.size() > 4 )
    { mFz = strList[4].toFloat(); }

    if ( strList.size() > 5 )
    { mBx = strList[5].toFloat(); }

    if ( strList.size() > 6 )
    { mBz = strList[6].toFloat(); }

    if ( strList.size() > 7 )
    { mLy = strList[7].toFloat(); }

    if ( strList.size() > 8 )
    { mRy = strList[8].toFloat(); }

    if ( strList.size() > 9 )
    { mComment = strList[9]; }

    return 0;

}
