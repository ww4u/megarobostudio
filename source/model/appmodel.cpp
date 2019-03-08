#include "appmodel.h"

int AppItem::columns()
{ return 4; }
QString AppItem::header( int col )
{
    if ( col == 0 )
    { return QObject::tr("Auto start"); }
    else if( col == 1 )
    { return QObject::tr("program"); }
    else if( col == 2 )
    { return QObject::tr("argument"); }
    else
    { return QObject::tr("comment"); }
}

AppItem::AppItem()
{
    mbAutoStart = false;
}

AppItem::~AppItem()
{}

void AppItem::setAutoStart( bool b )
{ mbAutoStart = b; }
bool AppItem::autoStart()
{ return mbAutoStart; }

void AppItem::setProgram( const QString &prog)
{ mProgram = prog; }
QString AppItem::program( )
{ return mProgram; }

void AppItem::setArgument( const QString &args )
{ mArguments = args; }
QString AppItem::argument( )
{ return mArguments; }

void AppItem::setComment( const QString &comment )
{ mComment = comment; }
QString AppItem::comment( )
{ return mComment; }

AppModel::AppModel() : MegaTableModel( "","" )
{

}

AppModel::~AppModel()
{
    delete_all( mItems );
}

void AppModel::assign( const AppModel &model )
{
    beginResetModel();
    delete_all( mItems );
    AppItem *pItem;

        for ( int i = 0; i < model.mItems.size(); i++ )
        {
            pItem = model.mItems.at( i );
            addItem( pItem->autoStart(),
                            pItem->program(),
                            pItem->argument(),
                            pItem->comment() );
        }

    endResetModel();
}

int AppModel::rowCount(const QModelIndex &parent) const
{ return mItems.count(); }
int AppModel::columnCount(const QModelIndex &parent) const
{ return AppItem::columns(); }

QVariant AppModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    { return QVariant(); }

    //! index
    int col = index.column();
    int row = index.row();

    if ( role != Qt::DisplayRole && role != Qt::EditRole )
    { return QVariant(); }

    if ( col == 0 )
    { return QVariant( mItems[ row ]->autoStart() ); }
    else if ( col == 1 )
    { return QVariant( ( mItems[ row ]->program() ) ); }
    else if ( col == 2 )
    { return QVariant( ( mItems[ row ]->argument() ) ); }
    else if ( col == 3 )
    { return QVariant( ( mItems[ row ]->comment() ) ); }
    else
    { return QVariant(); }
}

bool AppModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int col = index.column();
        int row = index.row();
        if ( col == 0 )
        {
            mItems[ row ]->setAutoStart( value.toBool() );
        }
        else if ( index.column() == 1 )
        {
            mItems[ row ]->setProgram( value.toString() );
        }
        else if ( index.column() == 2 )
        {
            mItems[ row ]->setArgument( value.toString() );
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
Qt::ItemFlags AppModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool AppModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    AppItem *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new AppItem();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool AppModel::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant AppModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( AppItem::header(section)); }
    else
    { return QVariant(section);}
}

void AppModel::addItem( bool autoStart,
                        const QString &prog,
                        const QString &args,
                        const QString &comment )
{
    AppItem *pRel;

    pRel = new AppItem();
    Q_ASSERT( NULL != pRel );

    pRel->setAutoStart( autoStart );
    pRel->setProgram( prog );
    pRel->setArgument( args );
    pRel->setComment( comment );

    mItems.append( pRel );
}
