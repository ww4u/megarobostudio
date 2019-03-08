#include "sinanjumotiongroup.h"
#include <QColor>

#include "../../com/comassist.h"

SinanjuMotionGroup::SinanjuMotionGroup( const QString &className,
                                        const QString &name ): MegaTableModel( className, name)
{
    mExportOptList.clear();
    mExportOptList<<"t-joints"<<"t-hand"<<"t-geometry";

    mSectionAble.clear();
    mSectionAble<<true<<true<<true;

    mbStepAble = true;
    mbPrefAble = true;
    setAutoTimeAble( true );

    mbToHereAble = true;

    mbSmartEditable = true;
    mSmartEditColumns.clear();
    mSmartEditColumns<<3<<4<<5;
    mRpcReq = RpcRequest::e_req_add_t4_key_point;
    mRpcType = RpcRequest::e_type_f_f_f;

    mCheckColumnList<<0<<7;
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

void SinanjuMotionGroup::setRpc( int row, RpcRequest &req)
{
    Q_ASSERT( row < mItems.size() );
    mItems[row]->mX = req.popFloat();
    mItems[row]->mY = req.popFloat();
    mItems[row]->mZ = req.popFloat();

    QModelIndex editIndex = index( row, 0 );

    emit dataChanged(editIndex, editIndex);
}

SinanjuMotionItem *SinanjuMotionGroup::operator[]( int index )
{
    Q_ASSERT( index >=0 && index < mItems.size() );

    return mItems[ index ];
}

int SinanjuMotionGroup::save( const QString &fileName )
{
    MDataSet dataSet;

    dataSet.setModel( mClassName );
    QStringList headers;
    headers<<"enable"<<"name"
           <<"t"<<"x"<<"y"<<"z"<<"terminal"
           <<"mode"
           <<"comment";
    dataSet.setHeaders( headers );

    MDataSection *pSec;
    pSec = dataSet.addSection();
    if ( NULL == pSec )
    { return -1; }

    //! section
    pSec->addAttribute( attr_timebase, MegaTableModel::toString( mtType ) );

    //! export data
    QString fmtStr = fmtString( headers );

    bool bRet;
    foreach( SinanjuMotionItem *pItem, mItems )
    {
        Q_ASSERT( NULL != pItem );

        bRet = pSec->addRow( QString("%1").arg( fmtStr )
                                               .arg( pItem->mbEnable )
                                               .arg( pItem->mName )
                                               .arg( pItem->mT )
                                               .arg( pItem->mX )
                                               .arg( pItem->mY )
                                               .arg( pItem->mZ )
                                               .arg( pItem->mH )
                                               .arg( MotionRow::encodeAttr( pItem->mIAttr ) )
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
//        <<"h"<<COL_SEP
//        <<"attr"<<COL_SEP
//        <<"comment"<<ROW_SEP;
//    foreach( SinanjuMotionItem *pItem, mItems )
//    {
//        if ( 0 != pItem->serialOut( text ) )
//        {
//            return ERR_FILE_WRITE_FAIL;
//        }
//    }

//    return 0;
}



int SinanjuMotionGroup::load( const QString &fileName )
{
    int ret;

    //! load
    MDataSet dataSet;
    ret = dataSet.load( fileName );
    if ( ret != 0 )
    { return ret; }

    if ( dataSet.verifyHeader("t", "x", "y", "z" ) )
    {}
    else
    { return -1; }

    MDataSection *pSec;
    pSec = dataSet.section( 0 );
    if ( NULL == pSec )
    { return -2; }

    MegaTableModel::toValue( pSec->getAttribute( attr_timebase), mtType );

    deparse_column_index( enable, "enable" );
    deparse_column_index( name, "name" );
    deparse_column_index( t, "t" );
    deparse_column_index( x, "x" );
    deparse_column_index( y, "y" );
    deparse_column_index( z, "z" );
    deparse_column_index( h, "terminal" );
    deparse_column_index( mode, "mode" );
    deparse_column_index( comment, "comment" );

//    headers<<"enable"<<"name"
//           <<"t"<<"x"<<"y"<<"z"<<"h"
//           <<"mode"
//           <<"comment";

//    //! get cols
//    int cT = dataSet.columnIndex( "t" );
//    int cX = dataSet.columnIndex( "p" );
//    int cV = dataSet.columnIndex( "v" );
//    int cEnable = dataSet.columnIndex( "enable" );
//    int cComment = dataSet.columnIndex( "comment" );

    removeRows( 0, mItems.count(), QModelIndex() );

    //! deload
    if ( dataSet.model().isEmpty() )
    {}
    else
    { mClassName = dataSet.model(); }

    //! data
    SinanjuMotionItem item;
    QString strMode;
    for ( int i = 0; i < pSec->rows(); i++ )
    {
        //! \note t, x, y, z is key
        if ( !pSec->cellValue( i, c_t, item.mT, 0 ) )
        { continue; }

        if ( !pSec->cellValue( i, c_x, item.mX, 0 ) )
        { continue; }

        if ( !pSec->cellValue( i, c_y, item.mY, 0 ) )
        { continue; }

        if ( !pSec->cellValue( i, c_z, item.mZ, 0 ) )
        { continue; }

        pSec->cellValue( i, c_h, item.mH, 0, false );

        pSec->cellValue( i, c_name, item.mName, "" );
        pSec->cellValue( i, c_enable, item.mbEnable, true );
        pSec->cellValue( i, c_comment, item.mComment, "" );
        pSec->cellValue( i, c_mode, strMode, "" );
        item.mIAttr = MotionRow::decodeAttr( strMode );

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
//            SinanjuMotionItem item;
//            lineStream.setString( &lineStr, QIODevice::ReadOnly );
//            if ( 0 != item.serialIn( lineStream ) )
//            {
//                //! next item
//            }
//            else
//            {
//                insertRow( mItems.size() );
//                *mItems[ mItems.size()- 1 ] = item;
//            }
//        }

//    }while( !text.atEnd() );

    emit dataChanged( index(0,0),
                      index(mItems.count(), SinanjuMotionItem::columns() - 1) );

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

void SinanjuMotionGroup::autoTime( double speed,
                                   double speedT,
                                   int align, int div )
{
    //! extract the enabled item
    QList< SinanjuMotionItem *> validItems;

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
    float t, dt, th;
    validItems[0]->mT = 0;
    t = 0;
    for ( int i = 1; i < validItems.size(); i++ )
    {
        //! body time
        dt = comAssist::eulcidenTime(
                                        validItems[i - 1]->mX, validItems[ i - 1]->mY, validItems[ i - 1]->mZ,
                                        validItems[i]->mX, validItems[ i ]->mY, validItems[ i ]->mZ,
                                        speed );
        //! hand time
        th = qAbs( validItems[i]->mH - validItems[i-1]->mH ) / speedT;

        //! max time
        dt = qMax( dt, th );

        //! timeline
        if ( mtType == time_abs )
        { t += aligndT( dt, align, div ); }
        else
        { t = aligndT( dt, align, div ); }

        validItems[ i ]->mT = t;
    }

    emit dataChanged( index(0,0),
                      index(mItems.count(), SinanjuMotionItem::columns() - 1) );
}

tpvType SinanjuMotionGroup::getAbsT( int index )
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

void SinanjuMotionGroup::switchTimeType( timeType pre, timeType nxt )
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
