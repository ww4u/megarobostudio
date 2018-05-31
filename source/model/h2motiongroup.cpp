#include "h2motiongroup.h"

H2MotionGroup::H2MotionGroup( const QString &className, const QString &fileName )
                             : MegaTableModel( className, fileName)
{
    mExportOptList.clear();
    mExportOptList<<"t-joints";

    setStepAble( false );
    setPrefAble( false );

    mSectionAble.clear();
    mSectionAble<<true;
}

H2MotionGroup::~H2MotionGroup()
{
    removeRows( 0, mItems.size(), QModelIndex() );
}

int H2MotionGroup::rowCount(const QModelIndex &parent) const
{ return mItems.size(); }
int H2MotionGroup::columnCount(const QModelIndex &parent) const
{ return H2MotionItem::columns(); }

QVariant H2MotionGroup::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    if ( role != Qt::DisplayRole && role != Qt::EditRole )
    { return QVariant(); }

    int col = index.column();
    int row = index.row();

    if ( col == 0 )
    { return QVariant( mItems[row]->enable() ); }
    if ( col == 1 )
    { return QVariant( mItems[row]->name() ); }

    if ( col == 2 )
    { return QVariant( (double)mItems[row]->T() ); }

    if ( col == 3 )
    { return QVariant( (double)mItems[row]->X() ); }
    if ( col == 4 )
    { return QVariant( (double)mItems[row]->Y() ); }
    if ( col == 5 )
    { return QVariant( mItems[row]->comment() ); }

    return QVariant();
}

bool H2MotionGroup::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( index.isValid() && role == Qt::EditRole )
    {}
    else
    { return false; }

    int col = index.column();
    int row = index.row();
    if ( col == 0 )
    { mItems[ row ]->setEnable( value.toBool() ); }
    else if ( index.column() == 1 )
    { mItems[ row ]->setName( value.toString() ); }

    else if ( index.column() == 2 )
    { mItems[ row ]->setT( value.toFloat() ); }
    else if ( index.column() == 3 )
    { mItems[ row ]->setX( value.toFloat() ); }
    else if ( index.column() == 4 )
    { mItems[ row ]->setY( value.toFloat() ); }
    else if ( index.column() == 5 )
    { mItems[ row ]->setComment( value.toString() ); }
    else
    {}

    emit dataChanged(index, index);

    return true;
}
Qt::ItemFlags H2MotionGroup::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool H2MotionGroup::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    H2MotionItem *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new H2MotionItem();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool H2MotionGroup::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant H2MotionGroup::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( H2MotionItem::header(section)); }
    else
    { return QVariant(section);}
}

H2MotionItem *H2MotionGroup::operator[]( int index )
{
    Q_ASSERT( index >=0 && index < mItems.size() );

    return mItems[ index ];
}

int H2MotionGroup::save( const QString &fileName )
{
    QFile file( fileName );

    if ( !file.open( QFile::WriteOnly ) )
    { return ERR_FILE_OPEN_FAIL; }

    ImcStream text( &file );
    text<<HEAD_SEP<<className()<<ROW_SEP;
    text<<HEAD_SEP
        <<"enable"<<COL_SEP
        <<"name"<<COL_SEP
        <<"t"<<COL_SEP
        <<"x"<<COL_SEP
        <<"y"<<COL_SEP
        <<"comment"<<ROW_SEP;
    foreach( H2MotionItem *pItem, mItems )
    {
        if ( 0 != pItem->serialOut( text ) )
        {
            return ERR_FILE_WRITE_FAIL;
        }
    }

    return 0;
}
int H2MotionGroup::load( const QString &fileName )
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
            H2MotionItem item;
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
                      index(mItems.count(), H2MotionItem::columns() - 1) );
    logDbg()<<mItems.size();
    return 0;
}

void H2MotionGroup::reverse()
{
    int count;

    count = mItems.size();

    //! reorder the item
    for ( int i = 0; i <  count/2; i++ )
    {
        mItems.swap( i, count-1-i );
    }

    //! reverse the time
    tpvType t;
    for ( int i = 0; i <count/2; i++ )
    {
        t = mItems[i]->mT;
        mItems[i]->mT = mItems[ count - i - 1 ]->mT;
        mItems[ count - i - 1 ]->mT = t;
    }

    emit dataChanged( index(0,0),
                      index(mItems.count(), H2MotionItem::columns() - 1) );
}
