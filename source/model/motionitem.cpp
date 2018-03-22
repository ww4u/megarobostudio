#include "motionitem.h"

static QString _headers[]=
{
    QStringLiteral("Enable"),
    QStringLiteral("Action"),
    QStringLiteral("t(s)"),
    QStringLiteral("x(mm)"),
    QStringLiteral("y(mm)"),
    QStringLiteral("z(mm)"),

    QStringLiteral("Comment"),
};

int motionItem::columns()
{
    return sizeof_array(_headers);
}
QString motionItem::header( int col )
{
    Q_ASSERT( col >= 0 && col < motionItem::columns() );

    if ( col == 0 )
    { return QObject::tr("Enable"); }
    else if ( col == 1 )
    { return QObject::tr("Hand Action"); }
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

motionItem::motionItem()
{
    mBreak = true;
    mCmd = "command";
    mX = 0;
    mY = 0;
    mZ = 0;
    mT = 0;
    mComment= "";
}

motionItem::motionItem( const motionItem &item )
{
    *this = item;
}

motionItem &motionItem::operator=( const motionItem &item )
{
    mBreak = item.mBreak;
    mCmd = item.mCmd;

    mT = item.mT;

    mX = item.mX;
    mY = item.mY;
    mZ = item.mZ;

    mComment = item.mComment;

    return *this;
}

int motionItem::serialOut( ImcStream &stream )
{
    stream<<mBreak<<COL_SEP<<mCmd<<COL_SEP<<mT<<COL_SEP<<mX<<COL_SEP<<mY<<COL_SEP<<mZ<<COL_SEP<<mComment<<ROW_SEP;

    return 0;
}
int motionItem::serialIn( ImcStream &stream )
{
    QString str = stream.readLine();

    QStringList strList = str.split( COL_SEP, QString::SkipEmptyParts );

    if ( strList.size() > 0 )
    { mBreak = strList[0].toFloat(); }

    if ( strList.size() > 1 )
    { mCmd = strList[1]; }

    if ( strList.size() > 2 )
    { mT = strList[2].toFloat(); }

    if ( strList.size() > 3 )
    { mX = strList[3].toFloat(); }

    if ( strList.size() > 4 )
    { mY = strList[4].toFloat(); }

    if ( strList.size() > 5 )
    { mZ = strList[5].toFloat(); }

    if ( strList.size() > 6 )
    { mComment = strList[6]; }

    return 0;
}

void motionItem::setBreak( bool b )
{ mBreak = b; }
bool motionItem::getBreak()
{ return mBreak; }

void motionItem::setItem( const QString &cmd, tpvType x, tpvType y, tpvType z, tpvType t )
{
    mCmd = cmd;
    mX = x;
    mY = y;
    mZ = z;
    mT = t;
}
void motionItem::setCmd( const QString &cmd )
{ mCmd = cmd; }

void motionItem::setX( tpvType x )
{ mX = x; }
void motionItem::setY( tpvType y )
{ mY = y; }
void motionItem::setZ( tpvType z )
{ mZ = z; }
void motionItem::setT( tpvType t )
{ mT = t; }

void motionItem::setPara( tpvType x, tpvType y, tpvType z, tpvType t )
{
    mX = x;
    mY = y;
    mZ = z;
    mT = t;
}
void motionItem::setComment( const QString &comment )
{
    mComment = comment;
}

QString motionItem::getCmd()
{ return mCmd; }
tpvType motionItem::getX()
{ return mX; }
tpvType motionItem::getY()
{ return mY; }
tpvType motionItem::getZ()
{ return mZ; }
tpvType motionItem::getT()
{ return mT; }
QString motionItem::getComment()
{ return mComment; }
