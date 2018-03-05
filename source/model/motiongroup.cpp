#include "motiongroup.h"

motionGroup::motionGroup( const QString&name ):scriptFile(name)
{

}

motionGroup::~motionGroup()
{
    removeRows( 0, mItems.size(), QModelIndex() );
}

int motionGroup::rowCount(const QModelIndex &parent) const
{ return mItems.size(); }
int motionGroup::columnCount(const QModelIndex &parent) const
{ return motionItem::columns(); }

QVariant motionGroup::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    if ( role != Qt::DisplayRole && role != Qt::EditRole )
    { return QVariant(); }

    int col = index.column();
    int row = index.row();

    if ( col == 0 )
    { return QVariant( mItems[row]->getBreak() ); }
    if ( col == 1 )
    { return QVariant( mItems[row]->getCmd() ); }
    if ( col == 2 )
    { return QVariant( (double)mItems[row]->getT() ); }

    if ( col == 3 )
    { return QVariant( (double)mItems[row]->getX() ); }
    if ( col == 4 )
    { return QVariant( (double)mItems[row]->getY() ); }
    if ( col == 5 )
    { return QVariant( (double)mItems[row]->getZ() ); }
    if ( col == 6 )
    { return QVariant( mItems[row]->getComment() ); }

    return QVariant();
}

bool motionGroup::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( index.isValid() && role == Qt::EditRole )
    {}
    else
    { return false; }

    int col = index.column();
    int row = index.row();
    if ( col == 0 )
    {
        mItems[ row ]->setBreak( value.toBool() );
    }
    else if ( index.column() == 1 )
    {
        mItems[ row ]->setCmd( value.toString() );
    }
    else if ( index.column() == 2 )
    {
        mItems[ row ]->setT( value.toFloat() );
    }
    else if ( index.column() == 3 )
    {
        mItems[ row ]->setX( value.toFloat() );
    }
    else if ( index.column() == 4 )
    {
        mItems[ row ]->setY( value.toFloat() );
    }
    else if ( index.column() == 5 )
    {
        mItems[ row ]->setZ( value.toFloat() );
    }
    else if ( index.column() == 6 )
    {
        mItems[ row ]->setComment( value.toString() );
    }
    else
    {}

    emit dataChanged(index, index);

    return true;
}
Qt::ItemFlags motionGroup::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool motionGroup::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    motionItem *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new motionItem();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool motionGroup::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant motionGroup::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( motionItem::header(section)); }
    else
    { return QVariant(section);}
}

motionItem *motionGroup::operator[]( int index )
{
    Q_ASSERT( index >=0 && index < mItems.size() );

    return mItems[ index ];
}

int motionGroup::save( const QString &fileName )
{
    QFile file( fileName );

    if ( !file.open( QFile::WriteOnly ) )
    { return ERR_FILE_OPEN_FAIL; }

    ImcStream text( &file );

    foreach( motionItem *pItem, mItems )
    {
        if ( 0 != pItem->serialOut( text ) )
        {
            return ERR_FILE_WRITE_FAIL;
        }
    }

    return 0;
}
int motionGroup::load( const QString &fileName )
{
    QFile file( fileName );

    if ( !file.open( QFile::ReadOnly ) )
    { return ERR_FILE_OPEN_FAIL; }

    //! remove all
    removeRows( 0, mItems.count(), QModelIndex() );

    ImcStream text( &file );
    ImcStream lineStream;

    QString lineStr;

    do
    {
        lineStr = text.readLine();
        lineStr = lineStr.trimmed();

        //! comment
        if ( lineStr.startsWith("#") || lineStr.startsWith("//") )
        {
        }
        else
        {
            motionItem item;
            lineStream.setString( &lineStr, QIODevice::ReadOnly );
            if ( 0 != item.serialIn( lineStream ) )
            {
                return ERR_FILE_READ_FAIL;
            }

            insertRow( mItems.size() );
            *mItems[ mItems.size()- 1 ] = item;
        }

    }while( !text.atEnd() );

    emit dataChanged( index(0,0),
                      index(mItems.count(), motionItem::columns() - 1) );
    logDbg()<<mItems.size();
    return 0;
}
