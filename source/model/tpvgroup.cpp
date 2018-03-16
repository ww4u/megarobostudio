#include "tpvgroup.h"
#include <QColor>

tpvGroup* tpvGroup::createDemoGroup1()
{
    return createDemoGroup( 0.5f, 180.0f*0.5 );
}

tpvGroup* tpvGroup::createDemoGroup2()
{
    return createDemoGroup( 0.5f, 180.0f*1.0f );
}

tpvGroup* tpvGroup::createDemoGroup3()
{
    return createDemoGroup( 0.5f, 180.0f*1.5f );
}

tpvGroup* tpvGroup::createDemoGroup4()
{
    return createDemoGroup( 0.5f, 180.0f*2.0f );
}

tpvGroup* tpvGroup::createDemoGroup5()
{
    return createDemoGroup( 0.5f, 180.0f*2.5f );
}

tpvGroup* tpvGroup::createDemoGroup( float dT, float dP )
{
    tpvGroup *pGroup;

    pGroup = new tpvGroup();
    Q_ASSERT( NULL != pGroup );

    pGroup->addItem( 0, 0, 0 );
    pGroup->addItem( dT, dP, 0 );

    return pGroup;
}

tpvGroup::tpvGroup()
{

}

tpvGroup::~tpvGroup()
{
    clear();
}

int tpvGroup::rowCount(const QModelIndex &parent) const
{ return mItems.count(); }
int tpvGroup::columnCount(const QModelIndex &parent) const
{ return tpvItem::columns(); }

QVariant tpvGroup::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    //! index
    int col = index.column();
    int row = index.row();

    //! color
    if ( role == Qt::BackgroundColorRole )
    {
        if ( !mItems[row]->getValid() )
        {
            return QVariant( QColor( Qt::lightGray ) );
        }
    }

    if ( role != Qt::DisplayRole && role != Qt::EditRole )
    { return QVariant(); }

    if ( col == 0 )
    { return QVariant( mItems[ row ]->getValid() ); }
    else if ( col == 1 )
    { return QVariant( (double)mItems[ row ]->getT() ); }
    else if ( col == 2 )
    { return QVariant( (double)mItems[ row ]->getP() ); }
    else if ( col == 3 )
    { return QVariant( (double)mItems[ row ]->getV() ); }
    else if ( col == 4 )
    { return QVariant( mItems[ row ]->getComment() ); }
    else
    { return QVariant(); }
}

bool tpvGroup::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int col = index.column();
        int row = index.row();
        if ( col == 0 )
        {
            mItems[ row ]->setValid( value.toBool() );
        }
        else if ( index.column() == 1 )
        {
            mItems[ row ]->setT( value.toFloat() );
        }
        else if ( index.column() == 2 )
        {
            mItems[ row ]->setP( value.toFloat() );
        }
        else if ( index.column() == 3 )
        {
            mItems[ row ]->setV( value.toFloat() );
        }
        else if ( index.column() == 4 )
        {
            mItems[ row ]->setComment( value.toString() );
        }
        else
        {}

        emit dataChanged(index, index);

        return true;
    }
    else
    {
        return false;
    }
}
Qt::ItemFlags tpvGroup::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool tpvGroup::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    tpvItem *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new tpvItem();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool tpvGroup::removeRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 1 )
    { return false; }

    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row)
    {
        delete mItems[position];
        mItems.removeAt(position);
    }

    endRemoveRows();
    return true;
}

QVariant tpvGroup::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( tpvItem::header(section)); }
    else
    { return QVariant(section);}
}

tpvItem * tpvGroup::operator[]( int id )
{
    //! check id
    Q_ASSERT( id >= 0 && id < mItems.size() );

    return mItems[id];
}

QList< tpvItem *> * tpvGroup::getRows()
{
    return ( &mItems );
}

QList< tpvItem *> * tpvGroup::getRows( QList<tpvRow*> &rows )
{
    foreach (tpvItem *pItem, mItems)
    {
        Q_ASSERT( NULL != pItem );

        if ( pItem->mValid )
        { rows.append( pItem ); }
    }

    return ( &mItems );
}

int tpvGroup::addItem( tpvType t, tpvType p, tpvType v )
{
    tpvItem *pItem;

    pItem = findItem( t );

    if ( NULL != pItem )
    { return -1; }

    pItem = new tpvItem( t, p, v );
    if ( NULL == pItem )
    { return -2; }

    mItems.append( pItem );

    return 0;
}
int tpvGroup::removeItem( tpvType t )
{
    tpvItem *pItem;

    pItem = findItem( t );

    if ( NULL != pItem )
    {
        mItems.removeAll( pItem );
        delete pItem;

        return 0;
    }

    return -1;
}

int tpvGroup::remove( int id )
{
    Q_ASSERT( id >= 0 && id < mItems.size() );

    tpvItem *pItem = mItems[ id ];
    mItems.removeAll( pItem );
    delete pItem;

    return 0;
}

int tpvGroup::clear()
{
//    foreach( tpvItem *pItem, mItems )
//    {
//        Q_ASSERT( NULL != pItem );
//        delete pItem;
//    }

//    mItems.clear();

    delete_all( mItems );

    return 0;
}

int tpvGroup::save( const QString &fileName )
{
    QFile file( fileName );

    if ( !file.open( QFile::WriteOnly ) )
    { return ERR_FILE_OPEN_FAIL; }

    ImcStream text( &file );

    foreach( tpvItem *pItem, mItems )
    {
        if ( 0 != pItem->serialOut( text ) )
        {
            return ERR_FILE_WRITE_FAIL;
        }
    }

    return 0;
}
int tpvGroup::load( const QString &fileName, pvtType flt )
{
    QFile file( fileName );

    if ( !file.open( QFile::ReadOnly ) )
    { return ERR_FILE_OPEN_FAIL; }

    //! remove all
    removeRows( 0, mItems.count(), QModelIndex() );

    ImcStream text( &file );
    ImcStream lineStream;

    QString lineStr;

    int ret;
    do
    {
        lineStr = text.readLine();
        lineStr = lineStr.trimmed();

        //! comment
        if ( lineStr.startsWith("#") || lineStr.startsWith("//") || lineStr.contains("[") || lineStr.length() < 5 )
        {
        }
        else
        {
            tpvItem item;
            lineStream.setString( &lineStr, QIODevice::ReadOnly );
            switch( flt )
            {
            case pvt_pvt:
                ret = item.serialIn_pvt( lineStream );
                break;
            case pvt_tpv:
                ret = item.serialIn_tpv( lineStream );
                break;
            default:
                return ERR_FILE_DO_NOT_SUPPORT;
            }

            if ( 0 != ret )
            {
                return ERR_FILE_READ_FAIL;
            }

            insertRow( mItems.size() );
            *mItems[ mItems.size()- 1 ] = item;
        }

    }while( !text.atEnd() );

    emit dataChanged( index(0,0),
                      index(mItems.count(), tpvItem::columns() - 1) );

    return 0;
}

tpvItem * tpvGroup::findItem( tpvType t )
{
    foreach( tpvItem *pItem, mItems )
    {
        Q_ASSERT( NULL != pItem );

        if ( time_eq( pItem->mT ,t ) )
        { return pItem; }
    }

    return NULL;
}


