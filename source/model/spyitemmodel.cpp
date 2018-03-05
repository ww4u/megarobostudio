#include "spyitemmodel.h"

static const QString _headers[]=
{
    QStringLiteral("Time"),
    QStringLiteral("Value"),
};

int spyItem::columns()
{
    return sizeof_array( _headers );
}

QString spyItem::header( int col )
{
    Q_ASSERT( col >= 0 && col < spyItem::columns() );

    return _headers[ col ];
}

spyItem::viewFormat spyItem::toViewFormat( int fmtIndex )
{
    return (spyItem::viewFormat)( 1 << fmtIndex );
}

struct tFmtString
{
    spyItem::viewFormat fmtValue;
    QString fmtString;
};
static tFmtString _fmt_string[]=
{
    { spyItem::view_asc, QStringLiteral("asc") },
    { spyItem::view_hex, QStringLiteral("hex") },
    { spyItem::view_int_8, QStringLiteral("char") },
    { spyItem::view_int_16, QStringLiteral("short") },

    { spyItem::view_int_32, QStringLiteral("int") },
    { spyItem::view_float, QStringLiteral("float") },
    { spyItem::view_double, QStringLiteral("double") },
    { spyItem::view_uint_8, QStringLiteral("unsigned char") },

    { spyItem::view_uint_16, QStringLiteral("unsigned short") },
    { spyItem::view_uint_32, QStringLiteral("unsigned int") },
};

QString spyItem::toString( spyItem::viewFormat fmt )
{
    for( int i = 0; i < sizeof_array(_fmt_string); i++ )
    {
        if ( has_attr(fmt, _fmt_string[i].fmtValue) )
        { return _fmt_string[i].fmtString; }
    }

    return "";
}

spyItem::spyItem()
{
    mMs = 0;
}

spyItem::spyItem( const QByteArray &ary )
{
    mMs = QDateTime::currentDateTime().toMSecsSinceEpoch();
    mVal = ary;
}

spyItem::~spyItem()
{}

#define fmt_type_value( type ) type val; \
                        memcpy( &val, ary.data(), sizeof(val) ); \
                        return QString::number( val );

QString spyItem::format( viewFormat fmt )
{
    QByteArray ary;

    if ( mVal.length() > 0 )
    {}
    else
    { return ""; }

    ary = mVal;
    if ( has_attr( fmt, view_big_endian ) )
    {
        //! \todo reverse
    }
    else
    {}

    //! byte only
    if ( has_attr( fmt, view_asc) )
    { return QString( mVal ); }
    if ( has_attr( fmt, view_hex) )
    {
        return QString( mVal.toHex( ' ' ) );
    }
    if ( has_attr( fmt, view_int_8) )
    { fmt_type_value( qint8 ); }

    //! > 1 byte
    if ( has_attr( fmt, view_int_16) )
    { fmt_type_value( qint16 ); }
    if ( has_attr( fmt, view_int_32) )
    { fmt_type_value( qint32 ); }
    if ( has_attr( fmt, view_float) )
    { fmt_type_value( float ); }
    if ( has_attr( fmt, view_double) )
    { fmt_type_value( double ); }

    //! unsigned
    if ( has_attr( fmt, view_uint_8) )
    { fmt_type_value( quint8 ); }
    if ( has_attr( fmt, view_uint_16) )
    { fmt_type_value( quint16 ); }
    if ( has_attr( fmt, view_uint_32) )
    { fmt_type_value( quint32 ); }

    return "";
}

spyItemModel::spyItemModel()
{
    mViewFmt = spyItem::view_hex;
}

spyItemModel::~spyItemModel()
{
    removeRows( 0, mItems.size(), QModelIndex() );
}

void spyItemModel::setViewFmt( spyItem::viewFormat fmt )
{
    mViewFmt = fmt;

//    submit();

    emit headerDataChanged( Qt::Horizontal, 0, 0 );
}
spyItem::viewFormat spyItemModel::getViewFmt()
{ return mViewFmt; }

int spyItemModel::rowCount(const QModelIndex &parent) const
{ return mItems.size(); }
int spyItemModel::columnCount(const QModelIndex &parent) const
{ return spyItem::columns(); }

//! time
//! data
QVariant spyItemModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    if ( role != Qt::DisplayRole || role == Qt::EditRole )
    { return QVariant(); }

    int col = index.column();
    int row = index.row();

    if ( col == 0 )
    { return QVariant( QDateTime::fromMSecsSinceEpoch( mItems[row]->mMs).toString() ); }
    if ( col == 1 )
    { return QVariant( mItems[row]->format(mViewFmt) ); }

    return QVariant();
}

QVariant spyItemModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    {
        //! append the format
        if ( section == 1 )
        {
            QString fmt;
            fmt = spyItem::toString( mViewFmt );

            if ( fmt.length() > 0 )
            {
                return QVariant( spyItem::header(section) + "(" + fmt + ")" );
            }
            else
            { return QVariant( spyItem::header(section) ); }
        }
        else
        {
            return QVariant( spyItem::header(section) );
        }
    }
    else
    { return QVariant(section);}
}

void spyItemModel::append( const QByteArray &dat )
{
    spyItem *pItem;

    pItem = new spyItem( dat );
    Q_ASSERT( NULL != pItem );

    append( pItem );
}

void spyItemModel::append( spyItem *pItem )
{
    Q_ASSERT( NULL != pItem );

    beginInsertRows( QModelIndex(), mItems.size(), mItems.size() + 1 );

    mItems.append( pItem );

    endInsertRows();
}
