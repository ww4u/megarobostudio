#include "../../include/mcstd.h"
#include "handactionmodel.h"

static QString _headers[]=
{
    QStringLiteral("Name"),
    QString("Value(" + QString( QChar(0x00B0)) + ")"),
    QStringLiteral("Comment"),
};

int handAction::columns()
{
    return sizeof_array( _headers );
}
QString handAction::header( int col )
{
    Q_ASSERT( col >= 0 && col <= sizeof_array(_headers) );

    return _headers[ col ];
}

//! action
handAction::handAction()
{ mValue = 0.0; }

handAction::~handAction()
{
}

void handAction::set( const QString &name,
                      double value,
                      const QString &comment )
{
    setName( name );
    setValue( value );
    setComment( comment );
}

void handAction::setName( const QString &name )
{ mName = name; }
QString handAction::name()
{ return mName; }

void handAction::setValue( double value )
{ mValue = value; }
double handAction::value()
{ return mValue; }

void handAction::setComment( const QString &comment )
{ mComment = comment; }
QString handAction::comment()
{ return mComment; }

//! model
handActionModel::handActionModel()
{

    //! debug
//    handAction *pAction;
//    pAction = new handAction();
//    pAction->set( "a", 1, "aa" );
//    mItems.append( pAction );

//    pAction = new handAction();
//    pAction->set( "b", 2, "bb" );
//    mItems.append( pAction );

//    pAction = new handAction();
//    pAction->set( "c", 350, "cc" );
//    mItems.append( pAction );
}

handActionModel::~handActionModel()
{
    delete_all( mItems );
}

int handActionModel::rowCount(const QModelIndex &parent) const
{ return mItems.count(); }
int handActionModel::columnCount(const QModelIndex &parent) const
{ return handAction::columns(); }

QVariant handActionModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    //! index
    int col = index.column();
    int row = index.row();

    if ( role != Qt::DisplayRole && role != Qt::EditRole )
    { return QVariant(); }

    if ( col == 0 )
    { return QVariant( mItems[ row ]->name() ); }
    else if ( col == 1 )
    { return QVariant( mItems[ row ]->value() ); }
    else if ( col == 2 )
    { return QVariant( mItems[ row ]->comment() ); }
    else
    { return QVariant(); }
}

bool handActionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int col = index.column();
        int row = index.row();
        if ( col == 0 )
        {
            mItems[ row ]->setName( value.toString() );
        }
        else if ( index.column() == 1 )
        {
            mItems[ row ]->setValue( value.toDouble() );
        }
        else if ( index.column() == 2 )
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
Qt::ItemFlags handActionModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool handActionModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    handAction *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new handAction();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool handActionModel::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant handActionModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( handAction::header(section)); }
    else
    { return QVariant(section);}
}

int handActionModel::save( const QString &fileName )
{
    QFile fileOut( fileName );

    if ( !fileOut.open( QIODevice::WriteOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamWriter writer( &fileOut );

    int ret;

    writer.writeStartDocument();

    writer.writeStartElement("hand_action");

    ret = serialOut( writer );

    writer.writeEndElement();

    writer.writeEndDocument();

    fileOut.close();

    return ret;

}

int handActionModel::load( const QString &fileName )
{
    //! check ver
    QFile fileIn(fileName);
    if ( !fileIn.open( QIODevice::ReadOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamReader reader( &fileIn );

    int ret;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "hand_action" )
        {
            ret = serialIn( reader );
        }
    }

    fileIn.close();

    return ret;
}

int handActionModel::serialOut( QXmlStreamWriter & writer )
{
    foreach( handAction *pAction, mItems )
    {
        Q_ASSERT( NULL != pAction );

        writer.writeStartElement( "action" );

        writer.writeTextElement( "name", pAction->name() );
        writer.writeTextElement( "value", QString::number( pAction->value() ) );
        writer.writeTextElement( "comment", pAction->comment() );

        writer.writeEndElement();
    }

    return 0;
}
int handActionModel::serialIn( QXmlStreamReader & reader )
{
    //! item
    handAction *pItem;
    QList< handAction * > localItems;

    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "action" )
        {
            pItem = new handAction();
            Q_ASSERT( NULL != pItem );

            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "name" )
                { pItem->setName( reader.readElementText() ); }
                else if ( reader.name() == "value" )
                { pItem->setValue( reader.readElementText().toDouble() ); }
                else if ( reader.name() == "comment" )
                { pItem->setComment( reader.readElementText() ) ; }
                else
                {}
            }

            localItems.append( pItem );
        }
    }

    //! assign
    delete_all( mItems );
    mItems = localItems;

    endResetModel();

    return 0;
}

QMap< QString, double > handActionModel::actionMap()
{
    QMap<QString, double > map;

    foreach ( handAction *pAction, mItems)
    {
        Q_ASSERT( NULL != pAction );

        if ( pAction->name().length() < 1 )
        { continue; }

        if ( map.contains( pAction->name()) )
        { continue; }

        map.insert( pAction->name(), pAction->value() );
    }

    return map;
}
