#include "h2zmotiongroup.h"
#include "../../com/comassist.h"
#include <QColor>
H2ZMotionGroup::H2ZMotionGroup( const QString &className,
                                const QString &name ) : MegaTableModel( className, name )
{
    mExportOptList.clear();
    mExportOptList<<"t-joints";

    mSectionAble.clear();
    mSectionAble<<true;

    mbStepAble = false;
    mbPrefAble = false;
    setAutoTimeAble( true );

    mbSmartEditable = true;
    mSmartEditColumns.clear();
    mSmartEditColumns<<3<<4<<5;
    mRpcReq = RpcRequest::e_req_add_h2z_key_point;
    mRpcType = RpcRequest::e_type_f_f_f;
}

H2ZMotionGroup::~H2ZMotionGroup()
{
    removeRows( 0, mItems.size(), QModelIndex() );
}

int H2ZMotionGroup::rowCount(const QModelIndex &parent) const
{ return mItems.size(); }
int H2ZMotionGroup::columnCount(const QModelIndex &parent) const
{ return H2ZMotionItem::columns(); }

QVariant H2ZMotionGroup::data(const QModelIndex &index, int role) const
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

    //! alignment
    //! enable
    if ( role == Qt::TextAlignmentRole && col == 0 )
    {
        return QVariant( Qt::AlignCenter );
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
    { return QVariant( mItems[row]->comment() ); }

    return QVariant();
}

bool H2ZMotionGroup::setData(const QModelIndex &index, const QVariant &value, int role)
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
    { mItems[ row ]->setZ( value.toFloat() ); }

    else if ( index.column() == 6 )
    { mItems[ row ]->setComment( value.toString() ); }
    else
    {}

    emit dataChanged(index, index);

    return true;
}
Qt::ItemFlags H2ZMotionGroup::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool H2ZMotionGroup::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    H2ZMotionItem *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new H2ZMotionItem();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool H2ZMotionGroup::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant H2ZMotionGroup::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( H2ZMotionItem::header(section)); }
    else
    { return QVariant(section);}
}

void H2ZMotionGroup::setRpc( int row, RpcRequest &req)
{
    Q_ASSERT( row < mItems.size() );
    mItems[row]->mX = req.popFloat();
    mItems[row]->mY = req.popFloat();
    mItems[row]->mZ = req.popFloat();

    QModelIndex editIndex = index( row, 0 );

    emit dataChanged(editIndex, editIndex);
}

H2ZMotionItem *H2ZMotionGroup::operator[]( int index )
{
    Q_ASSERT( index >=0 && index < mItems.size() );

    return mItems[ index ];
}

int H2ZMotionGroup::save( const QString &fileName )
{
    MDataSet dataSet;

    dataSet.setModel( mClassName );
    QStringList headers;
    headers<<"enable"<<"name"
           <<"t"
           <<"x"<<"y"<<"z"
           <<"comment";
    dataSet.setHeaders( headers );

    MDataSection *pSec;
    pSec = dataSet.addSection();
    if ( NULL == pSec )
    { return -1; }

    pSec->addAttribute( attr_timebase, MegaTableModel::toString( mtType ) );

    //! export data
    QString fmtStr = fmtString( headers );

    bool bRet;
    foreach( H2ZMotionItem *pItem, mItems )
    {
        Q_ASSERT( NULL != pItem );

        bRet = pSec->addRow( QString("%1").arg( fmtStr )
                                               .arg( pItem->mbEnable )
                                               .arg( pItem->mName )
                                               .arg( pItem->mT )
                                               .arg( pItem->mX )
                                               .arg( pItem->mY )
                                               .arg( pItem->mZ )
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
//        <<"x"<<COL_SEP
//        <<"y"<<COL_SEP
//        <<"z"<<COL_SEP
//        <<"comment"<<ROW_SEP;
//    foreach( H2ZMotionItem *pItem, mItems )
//    {
//        if ( 0 != pItem->serialOut( text ) )
//        {
//            return ERR_FILE_WRITE_FAIL;
//        }
//    }

//    return 0;
}
int H2ZMotionGroup::load( const QString &fileName )
{
    int ret;

    //! load
    MDataSet dataSet;
    ret = dataSet.load( fileName );
    if ( ret != 0 )
    { return ret; }

//    if ( dataSet.isEmpty() )
//    { return -1; }

    if ( dataSet.verifyHeader("t", "x", "y" ) )
    {}
    else
    { return -1; }

    MDataSection *pSec;
    pSec = dataSet.section( 0 );
    if ( NULL == pSec )
    { return -2; }

    MegaTableModel::toValue( pSec->getAttribute( attr_timebase), mtType );

    deparse_column_index( t, "t" );
    deparse_column_index( x, "x" );
    deparse_column_index( y, "y" );
    deparse_column_index( z, "z" );

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
    H2ZMotionItem item;
    for ( int i = 0; i < pSec->rows(); i++ )
    {
        if ( !pSec->cellValue( i, c_t, item.mT, 0 ) )
        { continue; }
        if ( !pSec->cellValue( i, c_x, item.mX, 0 ) )
        { continue; }
        if ( !pSec->cellValue( i, c_y, item.mY, 0 ) )
        { continue; }
        if ( !pSec->cellValue( i, c_z, item.mZ, 0, false ) )
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
//            H2ZMotionItem item;
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
                      index(mItems.count(), H2ZMotionItem::columns() - 1) );

    return 0;
}

void H2ZMotionGroup::reverse()
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
                      index(mItems.count(), H2ZMotionItem::columns() - 1) );
}

void H2ZMotionGroup::autoTime( double speed, double speedTerminal,
                               int align, int div )
{
    //! extract the enabled item
    QList< H2ZMotionItem *> validItems;

    for ( int i = 0; i < mItems.size(); i++ )
    {
        if ( mItems.at(i)->mbEnable )
        { validItems.append( mItems.at(i)); }
    }

    if ( validItems.size() > 1 )
    { }
    else
    { return; }

    //! set the t
    float t, dt;
    validItems[0]->mT = 0;
    t = 0;
    for ( int i = 1; i < validItems.size(); i++ )
    {
        //! body time
        dt = comAssist::eulcidenTime(
                                        validItems[i - 1]->mX, validItems[ i - 1]->mY, validItems[ i - 1]->mZ,
                                        validItems[i]->mX, validItems[ i ]->mY, validItems[ i ]->mZ,
                                        speed );

        //! timeline
        if ( mtType == time_abs )
        { t += aligndT( dt, align, div ); }
        else
        { t = aligndT( dt, align, div ); }

        validItems[ i ]->mT = t;
    }

    emit dataChanged( index(0,0),
                      index(mItems.count(), H2ZMotionItem::columns() - 1) );
}

tpvType H2ZMotionGroup::getAbsT( int index )
{
    Q_ASSERT( index >= 0 && index < mItems.size() );

    if ( mtType == time_rel )
    {
        if ( index == 0 )
        { mSumT = mItems.at(index)->T(); }
        else
        {
            mSumT += mItems.at(index)->T();
        }

        return mSumT;
    }
    else
    {
        return mItems.at(index)->T();
    }
}

void H2ZMotionGroup::switchTimeType( timeType pre, timeType nxt )
{
    if ( pre == nxt )
    { return; }

    //! rel-> abs
    if ( nxt == time_abs )
    {
        for ( int i = 1; i < mItems.size(); i++ )
        {
            mItems[i]->setT( mItems[i]->T() + mItems[i-1]->T() );
        }
    }
    //! rel->abs
    else if ( nxt == time_rel )
    {
        for ( int i = mItems.size()-1; i > 0; i-- )
        {
            mItems[i]->setT( mItems[i]->T() - mItems[i-1]->T() );
        }
    }
    else
    {}

    //! changed
    emit dataChanged( index(0,0),
                      index(mItems.count(), SinanjuMotionItem::columns() - 1) );
}
