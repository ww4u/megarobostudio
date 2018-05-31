#include "sinanjumotiongroup.h"
#include <QColor>

SinanjuMotionGroup::SinanjuMotionGroup( const QString &className,
                                        const QString &name ): MegaTableModel( className, name)
{
    mExportOptList.clear();
    mExportOptList<<"t-joints"<<"t-hand"<<"t-geometry";

    mSectionAble.clear();
    mSectionAble<<true<<true<<true;

    mbStepAble = true;
    mbPrefAble = true;
}

SinanjuMotionGroup::~SinanjuMotionGroup()
{
    removeRows( 0, mItems.size(), QModelIndex() );
}

int SinanjuMotionGroup::rowCount(const QModelIndex &parent) const
{ return mItems.size(); }
int SinanjuMotionGroup::columnCount(const QModelIndex &parent) const
{ return SinanjuMotionItem::columns(); }

QVariant SinanjuMotionGroup::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    int col = index.column();
    int row = index.row();

    //! color
    if ( role == Qt::BackgroundColorRole )
    {
        if ( !mItems[row]->enable() )
        {
            return QVariant( QColor( Qt::lightGray ) );
        }
    }

    if ( role != Qt::DisplayRole && role != Qt::EditRole )
    { return QVariant(); }

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
    { return QVariant( (double)mItems[row]->Z() ); }
    if ( col == 6 )
    { return QVariant( (double)mItems[row]->H() ); }
    if ( col == 7 )
    { return QVariant( (bool)(mItems[row]->IAttr() & 0x01) ); }
    if ( col == 8 )
    { return QVariant( mItems[row]->comment() ); }

    return QVariant();
}

bool SinanjuMotionGroup::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( index.isValid() && role == Qt::EditRole )
    {}
    else
    { return false; }

    int col = index.column();
    int row = index.row();
    if ( col == 0 )
    {
        mItems[ row ]->setEnable( value.toBool() );
    }
    else if ( index.column() == 1 )
    {
        mItems[ row ]->setName( value.toString() );
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
        mItems[ row ]->setH( value.toFloat() );
    }
    else if ( index.column() == 7 )
    {
//        mItems[ row ]->setH( value.toFloat() );
//        mItems[ row ]->setIAttr( (value.toBool() ? 1 : 0 )
//                                 | mItems[row]->IAttr()
//                                 ) ;
        quint32 iattr = mItems[row]->IAttr();
        if ( value.toBool() )
        { set_bit( iattr, 0); }
        else
        { unset_bit( iattr, 0 ); }

        mItems[ row ]->setIAttr( iattr );
    }
    else if ( index.column() == 8 )
    {
        mItems[ row ]->setComment( value.toString() );
    }
    else
    {}

    emit dataChanged(index, index);

    return true;
}
Qt::ItemFlags SinanjuMotionGroup::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool SinanjuMotionGroup::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    SinanjuMotionItem *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new SinanjuMotionItem();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool SinanjuMotionGroup::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant SinanjuMotionGroup::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( SinanjuMotionItem::header(section)); }
    else
    { return QVariant(section);}
}

SinanjuMotionItem *SinanjuMotionGroup::operator[]( int index )
{
    Q_ASSERT( index >=0 && index < mItems.size() );

    return mItems[ index ];
}

int SinanjuMotionGroup::save( const QString &fileName )
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
        <<"z"<<COL_SEP
        <<"h"<<COL_SEP
        <<"attr"<<COL_SEP
        <<"comment"<<ROW_SEP;
    foreach( SinanjuMotionItem *pItem, mItems )
    {
        if ( 0 != pItem->serialOut( text ) )
        {
            return ERR_FILE_WRITE_FAIL;
        }
    }

    return 0;
}
int SinanjuMotionGroup::load( const QString &fileName )
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
            SinanjuMotionItem item;
            lineStream.setString( &lineStr, QIODevice::ReadOnly );
            if ( 0 != item.serialIn( lineStream ) )
            {
                //! next item
            }
            else
            {
                insertRow( mItems.size() );
                *mItems[ mItems.size()- 1 ] = item;
            }
        }

    }while( !text.atEnd() );

    emit dataChanged( index(0,0),
                      index(mItems.count(), SinanjuMotionItem::columns() - 1) );
    logDbg()<<mItems.size();
    return 0;
}

void SinanjuMotionGroup::reverse()
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
                      index(mItems.count(), SinanjuMotionItem::columns() - 1) );
}
