#include "tpvgroup.h"
#include <QColor>
#include "../arith/pvt/pvt.h"
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

int tpvGroup::slopelize( QList<tpvGroup*> gps,
                      QList<int> planModes,
                      QList<float> rises,
                      QList<float> falls,
                      int *pErrGp, int *pErrRow )
{
    int ret = 0;
    *pErrGp = -1;

    //! check size
    Q_ASSERT( planModes.size() == gps.size() );
    Q_ASSERT( planModes.size() == rises.size() );
    Q_ASSERT( planModes.size() == rises.size() );

    for ( int i = 0; i < gps.size(); i++ )
    {
        ret = gps.at( i )->slopelize( planModes.at(i),
                                      rises.at(i),
                                      falls.at(i),
                                      pErrRow );
        if ( ret != 0 )
        {
            *pErrGp = i;
            return ret;
        }
    }

    return 0;
}

tpvGroup::tpvGroup() : MegaTableModel("joint","")
{
    mtType = time_abs;
    mSumT = -1;
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

    //! alignment
    //! enable
    if ( role == Qt::TextAlignmentRole && col == 0 )
    {
        return QVariant( Qt::AlignCenter );
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
    { return Qt::ItemIsEnabled; }

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
        Q_ASSERT( NULL != pItem );
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

QList< tpvItem *> * tpvGroup::getRows( QList<tpvRow> &rows )
{
    foreach (tpvItem *pItem, mItems)
    {
        Q_ASSERT( NULL != pItem );

        if ( pItem->mValid )
        {
            rows.append( tpvRow( pItem->getT(),
                                 pItem->getP(),
                                 pItem->getV(),
                                 pItem->getRise(),
                                 pItem->getFall() ) );
        }
    }

    return ( &mItems );
}

//! trim rows
//! remove the invalid rows
void tpvGroup::trimRows( tpvGroup &gp )
{
    //! timebase is same
    gp.setTimeType( mtType );

    foreach (tpvItem *pItem, mItems)
    {
        Q_ASSERT( NULL != pItem );

        if ( pItem->mValid )
        {
            gp.addItem( pItem->getT(), pItem->getP(), pItem->getV(),
                        pItem->getRise(), pItem->getFall() );
        }
    }
}

//! abs time
//! all are valid
void tpvGroup::abstimeRows( tpvGroup &gp )
{
    //! copy only
    if ( mtType == time_abs )
    {
        foreach (tpvItem *pItem, mItems)
        {
            Q_ASSERT( NULL != pItem );

            gp.addItem( pItem->getT(), pItem->getP(), pItem->getV() );
        }
    }
    //! rel
    else
    {
        for ( int i = 0; i < mItems.size(); i++ )
        {
            gp.addItem( getAbsT( i ), mItems[i]->getP(), mItems[i]->getV() );
        }
    }

    //! abs
    gp.setTimeType( time_abs );
}

//! 0 -- cubic
//! 1 -- trapzoid
//! 2 -- s
int tpvGroup::slopelize( int planMode,
                         float rise, float fall,
                         int *errRow )
{
    int errIndex;
    int ret;
    double slopes[2]={ rise,fall };

    *errRow = -1;
    for ( int i = 0; i < mItems.size() - 1 ; i++ )
    {
        ret = pvtSlope( mItems.at(i)->mP, mItems.at(i)->mV, mItems.at(i)->mT,
                        mItems.at(i+1)->mP, mItems.at(i+1)->mV, mItems.at(i+1)->mT,
                        planMode,
                        slopes,
                        &errIndex
                  );
        if ( ret != 0 )
        {
            *errRow = i;
            return ret;
        }

        //! save to the next
        mItems.at(i+1)->setSlope( slopes[0], slopes[1] );
    }

    return 0;
}

int tpvGroup::addItem( tpvType t, tpvType p, tpvType v, tpvType rise, tpvType fall  )
{
    tpvItem *pItem;

//    pItem = findItem( t );

//    if ( NULL != pItem )
//    { logDbg()<<t; return -1; }

    pItem = new tpvItem( t, p, v, rise, fall  );
    if ( NULL == pItem )
    { logDbg(); return -2; }

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
    delete_all( mItems );

    return 0;
}

void tpvGroup::rstAbsT()
{ mSumT = -1; }

tpvType tpvGroup::getAbsT( int index )
{
    Q_ASSERT( index >= 0 && index < mItems.size() );

    if ( mtType == time_rel )
    {
        if ( mSumT < 0 )
        { mSumT = mItems.at(index)->getT(); }
        else
        { mSumT += mItems.at(index)->getT(); }

        return mSumT;
    }
    else
    {
        return mItems.at(index)->getT();
    }
}

int tpvGroup::save( const QString &fileName )
{
    MDataSet dataSet;

    dataSet.setModel( mClassName );
    QStringList headers;
    headers<<"enable"<<"t"<<"p"<<"v"<<"comment";
    dataSet.setHeaders( headers );

    MDataSection *pSec;
    pSec = dataSet.addSection();
    if ( NULL == pSec )
    { return -1; }

    //! attr
    pSec->addAttribute(attr_timebase, tpvGroup::toString( mtType ) );

    //! export data
    QString fmtStr = fmtString( headers );

    bool bRet;
    foreach( tpvItem *pItem, mItems )
    {
        Q_ASSERT( NULL != pItem );

        bRet = pSec->addRow( QString("%1").arg( fmtStr )
                                               .arg( pItem->mValid )
                                               .arg( pItem->mT )
                                               .arg( pItem->mP )
                                               .arg( pItem->mV )
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

//    foreach( tpvItem *pItem, mItems )
//    {
//        if ( 0 != pItem->serialOut( text ) )
//        {
//            return ERR_FILE_WRITE_FAIL;
//        }
//    }

//    return 0;
}
int tpvGroup::load( const QString &fileName, pvtType flt )
{
    int ret;

    //! load
    MDataSet dataSet;
    ret = dataSet.load( fileName );
    if ( ret != 0 )
    { return ret; }

//    if ( dataSet.isEmpty() )
//    { return -1; }

    if ( dataSet.verifyHeader("t", "p") )
    {}
    else
    { return -1; }

    MDataSection *pSec;
    pSec = dataSet.section( 0 );
    if ( NULL == pSec )
    { return -2; }

    //! get cols
    int cT = dataSet.columnIndex( "t" );
    int cP = dataSet.columnIndex( "p" );
    int cV = dataSet.columnIndex( "v" );
    int cEnable = dataSet.columnIndex( "enable" );
    int cComment = dataSet.columnIndex( "comment" );

    removeRows( 0, mItems.count(), QModelIndex() );

    //! deload
    if ( dataSet.model().isEmpty() )
    {}
    else
    { mClassName = dataSet.model(); }

    //! attribute
    MegaTableModel::toValue( pSec->getAttribute( attr_timebase ), mtType );

    //! data
    tpvItem item;
    for ( int i = 0; i < pSec->rows(); i++ )
    {
        //! \note t, p is key
        if ( !pSec->cellValue( i, cT, item.mT, 0 ) )
        { continue; }

        if ( !pSec->cellValue( i, cP, item.mP, 0 ) )
        { continue; }

        pSec->cellValue( i, cV, item.mV, 0 );

        pSec->cellValue( i, cEnable, item.mValid, 1 );

        pSec->cellValue( i, cComment, item.mComment, "" );

        //! append the item
        insertRow( mItems.size() );
        *mItems[ mItems.size()- 1 ] = item;
    }

//    return 0;

//    QFile file( fileName );

//    if ( !file.open( QFile::ReadOnly ) )
//    { return ERR_FILE_OPEN_FAIL; }

//    //! remove all
//    removeRows( 0, mItems.count(), QModelIndex() );

//    ImcStream text( &file );
//    ImcStream lineStream;

//    QString lineStr;

//    int ret;
//    do
//    {
//        lineStr = text.readLine();
//        lineStr = lineStr.trimmed();

//        //! comment
//        if ( lineStr.startsWith("#") || lineStr.startsWith("//") || lineStr.contains("[") || lineStr.length() < 5 )
//        {
//        }
//        else
//        {
//            tpvItem item;
//            lineStream.setString( &lineStr, QIODevice::ReadOnly );
//            switch( flt )
//            {
//            case pvt_pvt:
//                ret = item.serialIn_pvt( lineStream );
//                break;
//            case pvt_tpv:
//                ret = item.serialIn_tpv( lineStream );
//                break;
//            default:
//                return ERR_FILE_DO_NOT_SUPPORT;
//            }

//            if ( 0 != ret )
//            {
//                return ERR_FILE_READ_FAIL;
//            }

//            insertRow( mItems.size() );
//            *mItems[ mItems.size()- 1 ] = item;
//        }

//    }while( !text.atEnd() );

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

void tpvGroup::switchTimeType( timeType pre, timeType nxt )
{
    if ( pre == nxt )
    { return; }

    //! rel-> abs
    if ( nxt == time_abs )
    {
        for ( int i = 1; i < mItems.size(); i++ )
        {
            mItems[i]->setT( mItems[i]->getT() + mItems[i-1]->getT() );
        }
    }
    //! rel->abs
    else if ( nxt == time_rel )
    {
        for ( int i = mItems.size()-1; i > 0; i-- )
        {
            mItems[i]->setT( mItems[i]->getT() - mItems[i-1]->getT() );
        }
    }
    else
    {}

    //! changed
    emit dataChanged( index(0,0),
                      index(mItems.count(), tpvItem::columns() - 1) );
}

