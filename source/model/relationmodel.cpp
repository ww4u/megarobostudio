#include "relationmodel.h"

int Relation::columns()
{ return 2; }
QString Relation::header( int col )
{
    if ( col == 0 )
    { return QObject::tr("Name"); }
    else
    { return QObject::tr("Value"); }
}

Relation::Relation()
{}
Relation::~Relation()
{
}

void Relation::setName( const QString &name )
{ mName = name; }
QString Relation::name()
{ return mName; }

void Relation::setValue( const QString &val )
{ mValue = val; }
QString Relation::value()
{ return mValue; }

//! model
RelationModel::RelationModel() : MegaTableModel("","")
{

}

RelationModel::~RelationModel()
{
    delete_all( mItems );
}

int RelationModel::rowCount(const QModelIndex &parent) const
{ return mItems.count(); }
int RelationModel::columnCount(const QModelIndex &parent) const
{ return Relation::columns(); }

QVariant RelationModel::data(const QModelIndex &index, int role) const
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
    { return QVariant( ( mItems[ row ]->value() ) ); }
    else
    { return QVariant(); }
}

bool RelationModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
            mItems[ row ]->setValue( value.toString() );
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
Qt::ItemFlags RelationModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool RelationModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    if ( position < 0 || rows < 0 )
    { return false; }

    beginInsertRows(QModelIndex(), position, position+rows-1);

    Relation *pItem;
    for (int row = 0; row < rows; ++row)
    {
        pItem = new Relation();
        mItems.insert( position+row, pItem );
    }

    endInsertRows();
    return true;
}
bool RelationModel::removeRows(int position, int rows, const QModelIndex &parent)
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

QVariant RelationModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
    { return QVariant(); }

    if ( orientation == Qt::Horizontal )
    { return QVariant( Relation::header(section)); }
    else
    { return QVariant(section);}
}

void RelationModel::addItem( const QString &name, const QString &val )
{
    Relation *pRel;

    pRel = new Relation();
    Q_ASSERT( NULL != pRel );

    pRel->setName( name );
    pRel->setValue( val );
    mItems.append( pRel );
}
