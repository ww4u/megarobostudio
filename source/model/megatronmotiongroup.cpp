#include "megatronmotiongroup.h"

MegatronMotionGroup::MegatronMotionGroup( const QString &className, const QString &name )
                                         : MegaTableModel( className, name)
{
    mExportOptList.clear();
    mExportOptList<<"t-joints";

    mSectionAble.clear();
    mSectionAble<<true;
}

MegatronMotionGroup::~MegatronMotionGroup()
{
    removeRows( 0, mItems.size(), QModelIndex() );
}

int MegatronMotionGroup::rowCount(const QModelIndex &parent) const
{ return mItems.size(); }
int MegatronMotionGroup::columnCount(const QModelIndex &parent) const
{ return MegatronMotionItem::columns(); }

QVariant MegatronMotionGroup::data(const QModelIndex &index, int role) const
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
    { return QVariant( (double)mItems[row]->Fx() ); }
    if ( col == 4 )
    { return QVariant( (double)mItems[row]->Fz() ); }
    if ( col == 5 )
    { return QVariant( (double)mItems[row]->Bx() ); }
    if ( col == 6 )
    { return QVariant( (double)mItems[row]->Bz() ); }
    if ( col == 7 )
    { return QVariant( (double)mItems[row]->Ly() ); }
    if ( col == 8 )
    { return QVariant( (double)mItems[row]->Ry() ); }

    if ( col == 9 )
    { return QVariant( mItems[row]->comment() ); }

    return QVariant();
}

bool MegatronMotionGroup::setData(const QModelIndex &index, const QVariant &value, int role)
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
    { mItems[ row ]->setFx( value.toFloat() ); }
    else if ( index.column() == 4 )
    { mItems[ row ]->setFz( value.toFloat() ); }
    else if ( index.column() == 5 )
    { mItems[ row ]->setBx( value.toFloat() ); }
    else if ( index.column() == 6 )
    { mItems[ row ]->setBz( value.toFloat() ); }
    else if ( index.column() == 7 )
    { mItems[ row ]->setLy( value.toFloat() ); }
    else if ( index.column() == 8 )
    { mItems[ row ]->setRy( value.toFloat() ); }

    else if ( index.column() == 9 )
    { mItems[ row ]->setComment( value.toString() ); }
    else
    {}

    emit dataChanged(index, index);

    return true;
}
Qt::ItemFlags MegatronMotionGroup::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool MegatronMotionGroup::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    MegatronMotionItem *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new MegatronMotionItem();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool MegatronMotionGroup::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant MegatronMotionGroup::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( MegatronMotionItem::header(section)); }
    else
    { return QVariant(section);}
}

MegatronMotionItem *MegatronMotionGroup::operator[]( int index )
{
    Q_ASSERT( index >=0 && index < mItems.size() );

    return mItems[ index ];
}

int MegatronMotionGroup::save( const QString &fileName )
{
    MDataSet dataSet;

    dataSet.setModel( mClassName );
    QStringList headers;
    headers<<"enable"<<"name"
           <<"t"
           <<"fx"<<"fz"
           <<"bx"<<"bz"
           <<"ly"<<"ry"
           <<"comment";
    dataSet.setHeaders( headers );

    MDataSection *pSec;
    pSec = dataSet.addSection();
    if ( NULL == pSec )
    { return -1; }

    //! export data
    QString fmtStr = fmtString( headers );

    bool bRet;
    foreach( MegatronMotionItem *pItem, mItems )
    {
        Q_ASSERT( NULL != pItem );

        bRet = pSec->addRow( QString("%1").arg( fmtStr )
                                               .arg( pItem->mbEnable )
                                               .arg( pItem->mName )
                                               .arg( pItem->mT )
                                               .arg( pItem->mFx ).arg( pItem->mFz )
                                               .arg( pItem->mBx ).arg( pItem->mBz )
                                               .arg( pItem->mLy ).arg( pItem->mRy )
                                               .arg( pItem->mComment ) );
        if ( !bRet )
        { return -1; }
    }

    //! save
    return dataSet.save( fileName );


//    QFile file( fileName );

//    if ( !file.open( QFile::WriteOnly ) )
//    { return ERR_FILE_OPEN_FAIL; }

//    ImcStream text( &file );
//    text<<HEAD_SEP<<className()<<ROW_SEP;
//    text<<HEAD_SEP
//        <<"enable"<<COL_SEP
//        <<"name"<<COL_SEP
//        <<"t"<<COL_SEP
//        <<"fx"<<COL_SEP
//        <<"fz"<<COL_SEP
//        <<"bx"<<COL_SEP
//        <<"bz"<<COL_SEP
//        <<"ly"<<COL_SEP
//        <<"ry"<<COL_SEP
//        <<"comment"<<ROW_SEP;
//    foreach( MegatronMotionItem *pItem, mItems )
//    {
//        if ( 0 != pItem->serialOut( text ) )
//        {
//            return ERR_FILE_WRITE_FAIL;
//        }
//    }

//    return 0;
}
int MegatronMotionGroup::load( const QString &fileName )
{
    int ret;

    //! load
    MDataSet dataSet;
    ret = dataSet.load( fileName );
    if ( ret != 0 )
    { return ret; }

//    if ( dataSet.isEmpty() )
//    { return -1; }

    if ( dataSet.verifyHeader("t", "fx", "ly", "fz") )
    {}
    else
    { return -1; }

    if ( dataSet.verifyHeader("t", "bx", "ry", "bz") )
    {}
    else
    { return -1; }

    MDataSection *pSec;
    pSec = dataSet.section( 0 );
    if ( NULL == pSec )
    { return -2; }

    deparse_column_index( t, "t" );
    deparse_column_index( fx, "fx" );
    deparse_column_index( ly, "ly" );
    deparse_column_index( fz, "fz" );

    deparse_column_index( bx, "bx" );
    deparse_column_index( ry, "ry" );
    deparse_column_index( bz, "bz" );
    deparse_column_index( enable, "enable" );

    deparse_column_index( comment, "comment" );
    deparse_column_index( name, "name" );


    removeRows( 0, mItems.count(), QModelIndex() );

    //! deload
    if ( dataSet.model().isEmpty() )
    {}
    else
    { mClassName = dataSet.model(); }

    //! data
    MegatronMotionItem item;
    for ( int i = 0; i < pSec->rows(); i++ )
    {
        if ( !pSec->cellValue( i, c_t, item.mT, 0 ) )
        { continue; }
        if ( !pSec->cellValue( i, c_fx, item.mFx, 0 ) )
        { continue; }
        if ( !pSec->cellValue( i, c_fz, item.mFz, 0 ) )
        { continue; }
        if ( !pSec->cellValue( i, c_ly, item.mLy, 0 ) )
        { continue; }

        if ( !pSec->cellValue( i, c_bx, item.mBx, 0 ) )
        { continue; }
        if ( !pSec->cellValue( i, c_bz, item.mBz, 0 ) )
        { continue; }
        if ( !pSec->cellValue( i, c_ry, item.mRy, 0 ) )
        { continue; }

        pSec->cellValue( i, c_name, item.mName, "" );
        pSec->cellValue( i, c_enable, item.mbEnable, true );
        pSec->cellValue( i, c_comment, item.mComment, "" );

        //! append the item
        insertRow( mItems.size() );
        *mItems[ mItems.size()- 1 ] = item;
    }

//    QFile file( fileName );

//    if ( !file.open( QFile::ReadOnly ) )
//    { return ERR_FILE_OPEN_FAIL; }

//    //! remove all
//    removeRows( 0, mItems.count(), QModelIndex() );

//    ImcStream text( &file );
//    ImcStream lineStream;

//    QString lineStr;

//    do
//    {
//        lineStr = text.readLine();
//        lineStr = lineStr.trimmed();

//        //! comment
//        if ( lineStr.startsWith("#") || lineStr.startsWith("//") )
//        {
//        }
//        else
//        {
//            MegatronMotionItem item;
//            lineStream.setString( &lineStr, QIODevice::ReadOnly );
//            if ( 0 != item.serialIn( lineStream ) )
//            {
//                return ERR_FILE_READ_FAIL;
//            }

//            insertRow( mItems.size() );
//            *mItems[ mItems.size()- 1 ] = item;
//        }

//    }while( !text.atEnd() );

    emit dataChanged( index(0,0),
                      index(mItems.count(), MegatronMotionItem::columns() - 1) );

    return 0;
}

void MegatronMotionGroup::reverse()
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
                      index(mItems.count(), MegatronMotionItem::columns() - 1) );
}
