#include "../../include/mcstd.h"
#include "eventactionmodel.h"

static QString _headers[]=
{
    QStringLiteral("Enable"),
    QStringLiteral("Event"),
    QStringLiteral("Action"),
    QStringLiteral("Comment"),
};

int EventAction::columns()
{
    return sizeof_array( _headers );
}
QString EventAction::header( int col )
{
    Q_ASSERT( col >= 0 && col <= sizeof_array(_headers) );

    return _headers[ col ];
}

EventAction::EventAction()
{
    mEnable = false;
}
EventAction::~EventAction()
{}

void EventAction::setEnable( bool b )
{ mEnable = b; }
bool EventAction::enable()
{ return mEnable; }

void EventAction::setEvent( const QString& evt )
{ mEvent = evt; }
QString EventAction::event()
{ return mEvent; }

void EventAction::setAction( const QString& act )
{ mAction = act; }
QString EventAction::action()
{ return mAction; }

void EventAction::setComment( const QString &cmt )
{ mComment = cmt; }
QString EventAction::comment()
{ return mComment; }

EventActionModel::EventActionModel()
{

}

EventActionModel::~EventActionModel()
{
    delete_all( mItems );
}

int EventActionModel::rowCount(const QModelIndex &parent) const
{ return mItems.count(); }
int EventActionModel::columnCount(const QModelIndex &parent) const
{ return EventAction::columns(); }

QVariant EventActionModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    //! index
    int col = index.column();
    int row = index.row();

    if ( role != Qt::DisplayRole && role != Qt::EditRole )
    { return QVariant(); }

    if ( col == 0 )
    { return QVariant( mItems[ row ]->enable() ); }
    else if ( col == 1 )
    { return QVariant( ( mItems[ row ]->event() ) ); }
    else if ( col == 2 )
    { return QVariant( ( mItems[ row ]->action() ) ); }
    else if ( col == 3 )
    { return QVariant( mItems[ row ]->comment() ); }
    else
    { return QVariant(); }
}

bool EventActionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int col = index.column();
        int row = index.row();
        if ( col == 0 )
        {
            mItems[ row ]->setEnable( value.toBool() );
        }
        else if ( index.column() == 1 )
        {
            mItems[ row ]->setEvent( value.toString() );
        }
        else if ( index.column() == 2 )
        {
            mItems[ row ]->setAction( value.toString() );
        }
        else if ( index.column() == 3 )
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
Qt::ItemFlags EventActionModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool EventActionModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    EventAction *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new EventAction();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool EventActionModel::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant EventActionModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( EventAction::header(section)); }
    else
    { return QVariant(section);}
}

QList< EventAction *> *EventActionModel::items()
{
    return &mItems;
}

int EventActionModel::save( const QString &fileName )
{
    QFile fileOut( fileName );

    if ( !fileOut.open( QIODevice::WriteOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamWriter writer( &fileOut );

    int ret;

    writer.writeStartDocument();

    writer.writeStartElement("event_action");

    ret = serialOut( writer );

    writer.writeEndElement();

    writer.writeEndDocument();

    fileOut.close();

    return ret;

}

int EventActionModel::load( const QString &fileName )
{
    //! check ver
    QFile fileIn(fileName);
    if ( !fileIn.open( QIODevice::ReadOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamReader reader( &fileIn );

    int ret;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "event_action" )
        {
            ret = serialIn( reader );
        }
    }

    fileIn.close();

    return ret;
}

int EventActionModel::serialOut( QXmlStreamWriter & writer )
{
    foreach( EventAction *pAction, mItems )
    {
        Q_ASSERT( NULL != pAction );

        writer.writeStartElement( "event_action" );

        writer.writeTextElement( "enable", QString::number( pAction->enable() ) );
        writer.writeTextElement( "event", ( pAction->event() ) );
        writer.writeTextElement( "action", ( pAction->action() ) );
        writer.writeTextElement( "comment", pAction->comment() );

        writer.writeEndElement();
    }

    return 0;

}
int EventActionModel::serialIn( QXmlStreamReader & reader )
{
    //! item
    EventAction *pItem;
    QList< EventAction * > localItems;

    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "event_action" )
        {
            pItem = new EventAction();
            Q_ASSERT( NULL != pItem );

            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "enable" )
                { pItem->setEnable( reader.readElementText().toInt() > 0 ); }
                else if ( reader.name() == "event" )
                { pItem->setEvent( reader.readElementText() ); }
                else if ( reader.name() == "action" )
                { pItem->setAction( reader.readElementText() ); }
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

