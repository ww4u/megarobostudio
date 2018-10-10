#include "spycfgmodel.h"
#include <QColor>
static const QString _headers[]=
{
    QStringLiteral("Name"),
    QStringLiteral("On/Off"),
    QStringLiteral("Source"),
    QStringLiteral("Type"),

    QStringLiteral("Interval(s)"),
    QStringLiteral("CacheSize"),
    QStringLiteral("Comment"),
};

int spyCfgItem::columns()
{
    return sizeof_array( _headers );
}

QString spyCfgItem::header( int col )
{
    Q_ASSERT( col >= 0 && col < spyCfgItem::columns() );

    return _headers[ col ];
}

spyCfgItem::spyCfgItem( const QString &name ) : mName(name)
{
    mbOnOff = false;
    mInterval = 1;
    mCacheSize = 1024;

    mCacheLen = 0;
}
spyCfgItem::~spyCfgItem()
{}

void spyCfgItem::setCacheLen( int len )
{ mCacheLen = len; }
int spyCfgItem::cacheLen()
{ return mCacheLen; }

//! model
spyCfgModel::spyCfgModel()
{

}

spyCfgModel::~spyCfgModel()
{
    removeRows( 0, mItems.size(), QModelIndex() );
}

int spyCfgModel::rowCount(const QModelIndex &parent) const
{ return mItems.size(); }
int spyCfgModel::columnCount(const QModelIndex &parent) const
{ return spyCfgItem::columns(); }

QVariant spyCfgModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    int col = index.column();
    int row = index.row();

    //! color
    if ( role == Qt::BackgroundColorRole )
    {
        if ( !mItems[row]->mbOnOff )
        {
            return QVariant( QColor( Qt::lightGray ) );
        }
    }

    //! disp + edit
    if ( role == Qt::DisplayRole || role == Qt::EditRole )
    {}
    else if ( role == Qt::UserRole )
    {
        QVariant var;
        var.setValue( mItems[row] );
        return var;
    }
    else
    { return QVariant(); }

    if ( col == 0 )
    { return QVariant( mItems[row]->mName ); }
    if ( col == 1 )
    { return QVariant( mItems[row]->mbOnOff ); }
    if ( col == 2 )
    { return QVariant( mItems[row]->mSource ); }

    if ( col == 3 )
    { return QVariant( mItems[row]->mType ); }
    if ( col == 4 )
    { return QVariant( mItems[row]->mInterval ); }
    if ( col == 5 )
    {
        if ( role == Qt::DisplayRole )
        { return QVariant( QString("%1/%2").arg( mItems[row]->mCacheLen ).arg( mItems[row]->mCacheSize ) ); }
        else
        { return QVariant( mItems[row]->mCacheSize );  }
    }
    if ( col == 6 )
    { return QVariant( mItems[row]->mComment ); }

    return QVariant();
}

bool spyCfgModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( index.isValid() && role == Qt::EditRole )
    {}
    else
    { return false; }

    int col = index.column();
    int row = index.row();
    if ( col == 0 )
    {
        mItems[ row ]->mName = ( value.toString() );
    }
    else if ( index.column() == 1 )
    {
        mItems[ row ]->mbOnOff = ( value.toBool() );
    }
    else if ( index.column() == 2 )
    {
        mItems[ row ]->mSource = ( value.toString() );
    }
    else if ( index.column() == 3 )
    {
        mItems[ row ]->mType = ( value.toString() );
    }
    else if ( index.column() == 4 )
    {
        mItems[ row ]->mInterval = ( value.toDouble() );
    }
    else if ( index.column() == 5 )
    {
        mItems[ row ]->mCacheSize = ( value.toInt() );
    }
    else if ( index.column() == 6 )
    {
        mItems[ row ]->mComment = ( value.toString() );
    }
    else
    {}

    emit dataChanged(index, index);

    return true;
}
Qt::ItemFlags spyCfgModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool spyCfgModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    spyCfgItem *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new spyCfgItem();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool spyCfgModel::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant spyCfgModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( spyCfgItem::header(section)); }
    else
    { return QVariant(section);}
}

QList< spyCfgItem *> *spyCfgModel::items()
{
    return &mItems;
}

spyCfgItem *spyCfgModel::findItem( const QString &src,
                                   const QString &itemName,
                                   int *pRow )
{
    spyCfgItem *pItem;
    for ( int i = 0; i < mItems.size(); i++ )
    {
        pItem = mItems.at( i );
        Q_ASSERT( NULL != pItem );

        if ( pItem->mSource == src && pItem->mType == itemName )
        {
            *pRow = i;
            return pItem;
        }
    }

    return NULL;
}

void spyCfgModel::setCacheLen( const QString &name, const QString &item, int len )
{
    int curRow;
    spyCfgItem *pSpyItem = findItem( name, item, &curRow );
    if ( NULL == pSpyItem )
    {}
    else
    {
        pSpyItem->setCacheLen( len );

        QModelIndex changeCell = createIndex( curRow, 5 );

        emit dataChanged( changeCell, changeCell );
    }
}
int spyCfgModel::cacheLen( const QString &name, const QString &item )
{
    int curRow;
    spyCfgItem *pSpyItem = findItem( name, item, &curRow );
    if ( NULL == pSpyItem )
    { return 0; }
    else
    { return pSpyItem->mCacheLen; }
}

int spyCfgModel::save( const QString &fileName )
{
    QFile fileOut( fileName );

    if ( !fileOut.open( QIODevice::WriteOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamWriter writer( &fileOut );

    //! write
    writer.writeStartDocument();

    writer.writeStartElement("spy_config");

    foreach( spyCfgItem *pItem, mItems )
    {
        Q_ASSERT( NULL != pItem );

        writer.writeStartElement("item");

        writer.writeTextElement( "name", ( pItem->mName) );
        writer.writeTextElement( "on", QString::number( pItem->mbOnOff) );
        writer.writeTextElement( "source", ( pItem->mSource) );
        writer.writeTextElement( "type", ( pItem->mType) );

        writer.writeTextElement( "interval", QString::number( pItem->mInterval) );
        writer.writeTextElement( "cache", QString::number( pItem->mCacheSize) );
        writer.writeTextElement( "comment", ( pItem->mComment) );

        writer.writeEndElement();
    }

    writer.writeEndElement();

    writer.writeEndDocument();

    fileOut.close();

    return 0;
}
int spyCfgModel::load( const QString &fileName )
{
    //! check ver
    QFile fileIn(fileName);
    if ( !fileIn.open( QIODevice::ReadOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamReader reader( &fileIn );

    QList< spyCfgItem * > localItems;
    spyCfgItem *pItem;

    //! start element
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "spy_config" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "item" )
                {
                    pItem = new spyCfgItem();
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "name" )
                        { pItem->mName = reader.readElementText(); }
                        else if ( reader.name() == "on" )
                        { pItem->mbOnOff = reader.readElementText().toInt() > 0; }
                        else if ( reader.name() == "source" )
                        { pItem->mSource = reader.readElementText(); }
                        else if ( reader.name() == "type" )
                        { pItem->mType = reader.readElementText(); }
                        else if ( reader.name() == "interval" )
                        { pItem->mInterval = reader.readElementText().toDouble(); }
                        else if ( reader.name() == "cache" )
                        { pItem->mCacheSize = reader.readElementText().toInt(); }
                        else if ( reader.name() == "comment" )
                        { pItem->mComment = reader.readElementText(); }
                        else
                        { reader.skipCurrentElement(); }
                    }

                    localItems.append( pItem );
                }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    fileIn.close();

    //! assign
    delete_all( mItems );
    mItems = localItems;

    endResetModel();

    return 0;
}

void spyCfgModel::fillDbgData()
{
    for ( int i = 0; i < 10; i++ )
    {
        mItems.append( new spyCfgItem( QString::number(i) ) );
    }
}
