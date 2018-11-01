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

    //! header list
    mHeaderList.clear();
    mHeaderList<<QObject::tr("Enable")
               <<QObject::tr("Name")
               <<QObject::tr("t(s)")
               <<QObject::tr("x(mm)")
               <<QObject::tr("y(mm)")
               <<QObject::tr("Comment");

    //! title list
    mTitleList.clear();
    mTitleList<<"enable"
              <<"name"
              <<"t"
              <<"x"
              <<"y"
              <<"comment";

    mbSmartEditable = true;
    mSmartEditColumns.clear();
    mSmartEditColumns<<3<<4;
    mRpcReq = RpcRequest::e_req_add_h2_key_point;
    mRpcType = RpcRequest::e_type_f_f;
}

H2MotionGroup::~H2MotionGroup()
{
    removeRows( 0, mItems.size(), QModelIndex() );
}

int H2MotionGroup::rowCount(const QModelIndex &parent) const
{ return mItems.size(); }
int H2MotionGroup::columnCount(const QModelIndex &parent) const
{ return mHeaderList.size(); }

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
    { return Qt::ItemIsEnabled; }

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
    { return QVariant( mHeaderList.at(section) ); }
    else
    { return QVariant(section);}
}

void H2MotionGroup::setRpc( int row, RpcRequest &req)
{
    Q_ASSERT( row < mItems.size() );
    mItems[row]->mX = req.popFloat();
    mItems[row]->mY = req.popFloat();

    QModelIndex editIndex = index( row, 0 );

    emit dataChanged(editIndex, editIndex);
}

H2MotionItem *H2MotionGroup::operator[]( int index )
{
    Q_ASSERT( index >=0 && index < mItems.size() );

    return mItems[ index ];
}


//stream<<mbEnable<<COL_SEP
//      <<mName<<COL_SEP
//      <<mT<<COL_SEP
//      <<mX<<COL_SEP
//      <<mY<<COL_SEP
//      <<mComment<<ROW_SEP;
int H2MotionGroup::save( const QString &fileName )
{
    MDataSet dataSet;

    dataSet.setModel( mClassName );
    QStringList headers;
    headers<<"enable"<<"name"
           <<"t"
           <<"x"<<"y"
           <<"comment";
    dataSet.setHeaders( headers );

    MDataSection *pSec;
    pSec = dataSet.addSection();
    if ( NULL == pSec )
    { return -1; }

    //! export data
    QString fmtStr = fmtString( headers );

    //!
    bool bRet;
    foreach( H2MotionItem *pItem, mItems )
    {
        Q_ASSERT( NULL != pItem );

        bRet = pSec->addRow( QString("%1").arg( fmtStr )
                                           .arg( pItem->mbEnable )
                                           .arg( pItem->mName )
                                           .arg( pItem->mT )
                                           .arg( pItem->mX )
                                           .arg( pItem->mY )
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
//    QString colSep;

//    text<<HEAD_SEP<<className()<<ROW_SEP;
//    text<<HEAD_SEP;

//    colSep.clear();
//    foreach( QString colTitle, mTitleList )
//    {
//        text<<colSep<<colTitle;
//        colSep = COL_SEP;
//    }
//    text<<ROW_SEP;

//    foreach( H2MotionItem *pItem, mItems )
//    {
//        if ( 0 != pItem->serialOut( text ) )
//        {
//            return ERR_FILE_WRITE_FAIL;
//        }
//    }

//    return 0;
}
int H2MotionGroup::load( const QString &fileName )
{
    int ret;

    //! load
    MDataSet dataSet;
    ret = dataSet.load( fileName );
    if ( ret != 0 )
    { return ret; }

    if ( dataSet.isEmpty() )
    { return -1; }

    if ( dataSet.verifyHeader("t", "x", "y" ) )
    {}
    else
    { return -1; }

    MDataSection *pSec;
    pSec = dataSet.section( 0 );
    if ( NULL == pSec )
    { return -2; }

    deparse_column_index( t, "t" );
    deparse_column_index( x, "x" );
    deparse_column_index( y, "y" );

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
    H2MotionItem item;
    for ( int i = 0; i < pSec->rows(); i++ )
    {
        if ( !pSec->cellValue( i, c_t, item.mT, 0 ) )
        { continue; }
        if ( !pSec->cellValue( i, c_x, item.mX, 0 ) )
        { continue; }
        if ( !pSec->cellValue( i, c_y, item.mY, 0 ) )
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
//            H2MotionItem item;
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
                      index(mItems.count(),
                      mHeaderList.size() - 1
                       ) );

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
                      index(mItems.count(),
                      mHeaderList.size() - 1 ) );
}
