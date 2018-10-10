#include "tpgroup.h"

TpGroup::TpGroup() :  MegaTableModel("","")
{
    mViewMode = e_view_motor;
}

TpGroup::~TpGroup()
{ clear(); }

int TpGroup::rowCount(const QModelIndex &parent) const
{ return mItems.count(); }
int TpGroup::columnCount(const QModelIndex &parent) const
{ return TpItem::columns(); }

QVariant TpGroup::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    //! index
    int col = index.column();
    int row = index.row();

//    //! color
//    if ( role == Qt::BackgroundColorRole )
//    {
//        if ( !mItems[row]->getValid() )
//        {
//            return QVariant( QColor( Qt::lightGray ) );
//        }
//    }

    if ( role != Qt::DisplayRole && role != Qt::EditRole )
    { return QVariant(); }

    if ( col == 0 )
    { return QVariant( (double)mItems[ row ]->getT() ); }
    else if ( col == 1 )
    {
        if ( mViewMode == e_view_motor )
        { return QVariant( (double)mItems[ row ]->getP() ); }
        else
        { return mItems[ row ]->getP() > 0; }
    }
    else
    { return QVariant(); }
}

bool TpGroup::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int row = index.row();

        if ( index.column() == 0 )
        {
            mItems[ row ]->setT( value.toFloat() );
        }
        else if ( index.column() == 1 )
        {
            mItems[ row ]->setP( value.toFloat() );
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
Qt::ItemFlags TpGroup::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool TpGroup::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    TpItem *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new TpItem();
        Q_ASSERT( NULL != pItem );
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool TpGroup::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant TpGroup::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( TpItem::header((int)mViewMode, section)); }
    else
    { return QVariant(section);}
}

void TpGroup::setViewMode( eViewMode eView )
{
    mViewMode = eView;

    emit dataChanged( index(0,0),
                      index(mItems.count(), TpItem::columns() - 1) );
}
TpGroup::eViewMode TpGroup::viewMode()
{ return mViewMode; }

TpItem * TpGroup::operator[]( int id )
{
    //! check id
    Q_ASSERT( id >= 0 && id < mItems.size() );

    return mItems[id];
}

QList< TpItem *> * TpGroup::getRows()
{
    return ( &mItems );
}

QList< TpItem *> * TpGroup::getRows( QList<TpRow*> &rows )
{
    foreach (TpItem *pItem, mItems)
    {
        Q_ASSERT( NULL != pItem );

        rows.append( pItem );
    }

    return ( &mItems );
}

int TpGroup::addItem( tpvType t, tpvType p )
{
    TpItem *pItem;

    pItem = findItem( t );

    if ( NULL != pItem )
    { logDbg()<<t; return -1; }

    pItem = new TpItem( t, p);
    if ( NULL == pItem )
    { logDbg(); return -2; }

    mItems.append( pItem );

    return 0;
}
int TpGroup::removeItem( tpvType t )
{
    TpItem *pItem;

    pItem = findItem( t );

    if ( NULL != pItem )
    {
        mItems.removeAll( pItem );
        delete pItem;

        return 0;
    }

    return -1;
}

int TpGroup::remove( int id )
{
    Q_ASSERT( id >= 0 && id < mItems.size() );

    TpItem *pItem = mItems[ id ];
    mItems.removeAll( pItem );
    delete pItem;

    return 0;
}

int TpGroup::clear()
{
    delete_all( mItems );
    return 0;
}

int TpGroup::save( const QString &fileName )
{
    QFile file( fileName );

    if ( !file.open( QFile::WriteOnly ) )
    { return ERR_FILE_OPEN_FAIL; }

    ImcStream text( &file );

    foreach( TpItem *pItem, mItems )
    {
        if ( 0 != pItem->serialOut( text ) )
        {
            return ERR_FILE_WRITE_FAIL;
        }
    }

    return 0;
}
int TpGroup::load( const QString &fileName )
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
    TpItem item;
    do
    {
        lineStr = text.readLine();
        lineStr = lineStr.trimmed();

        //! comment
        if ( lineStr.startsWith("#") || lineStr.startsWith("//") || lineStr.contains("[") || lineStr.length() < 3 )
        {
        }
        else
        {
            //! to stream
            lineStream.setString( &lineStr, QIODevice::ReadOnly );
            ret = item.serialIn( lineStream );
            if ( 0 != ret )
            { return ERR_FILE_READ_FAIL; }

            //! add item
            insertRow( mItems.size() );
            *mItems[ mItems.size()- 1 ] = item;
        }

    }while( !text.atEnd() );

    emit dataChanged( index(0,0),
                      index(mItems.count(), TpItem::columns() - 1) );

    return 0;
}

TpItem * TpGroup::findItem( tpvType t )
{
    foreach( TpItem *pItem, mItems )
    {
        Q_ASSERT( NULL != pItem );

        if ( time_eq( pItem->mT ,t ) )
        { return pItem; }
    }

    return NULL;
}
