#include "scriptmodel.h"
#include <QIcon>

#include "../com/comassist.h"

scriptModel::scriptModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    //! root is a group
    m_pRootNode = new scriptGroup();
    Q_ASSERT( NULL != m_pRootNode );

}
scriptModel::~scriptModel()
{
    delete m_pRootNode;
}
QVariant scriptModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QModelIndex scriptModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
    {
        return QModelIndex();
    }

    //! top item
    if ( !parent.isValid() )
    {
        return createIndex( 0, 0, m_pRootNode );
    }
    //! valid parent
    else
    {

    }

    //! parent node
    scriptNode *parentItem = getItem(parent);
    Q_ASSERT( NULL != parentItem );

    scriptNode *childItem = parentItem->child(row);
    if (childItem)
    {
        //! attach the pointer
        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex scriptModel::parent(const QModelIndex &index) const
{
//    return QModelIndex();
    if (index.isValid())
    {  }
    else
    { return QModelIndex(); }

    //! current item
    scriptNode *pNode = getItem( index );
    Q_ASSERT( NULL != pNode );

    //! parent item
    scriptNode *pParent = pNode->getParent();
    if ( NULL == pParent  )
    {return QModelIndex();}

    //! root
    Q_ASSERT( NULL != m_pRootNode );
    if ( pParent == m_pRootNode )
    {
        return createIndex( 0, 0, m_pRootNode );
    }

    //! get parent from child
    return createIndex( pParent->childNumber(), 0, pParent );
}

int scriptModel::rowCount(const QModelIndex &parent) const
{
    //! top item
    if (parent.isValid())
    {
    }
    else
    {
        if ( m_pRootNode->getName().length() > 0 )
        { return 1; }
        else
        { return 0; }
    }

    scriptNode *pNode;

    //! parent node
    pNode = getItem( parent );
    if ( NULL==pNode)
    { return 0; }

    if ( pNode->getNodeType() == scriptNode::node_file )
    { return 1; }
    else
    {
        return pNode->childCount();
    }
}

int scriptModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

bool scriptModel::hasChildren(const QModelIndex &parent) const
{
    //! parent node
    scriptNode *pNode = getItem( parent );
    if ( NULL==pNode)
    { return 0; }

    if ( pNode->getNodeType() == scriptNode::node_group )
    {
        return true;
    }
    else
    {
        return false;
    }
}

QVariant scriptModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ( role == Qt::DisplayRole || role == Qt::EditRole )
    {
        scriptNode *pNode = getItem( index );
        return QVariant( pNode->getName() );
    }
    else if ( role == Qt::DecorationRole )
    {
        scriptNode *pNode = getItem( index );
        if ( pNode->getNodeType() == scriptNode::node_file )
        { return (QVariant)QIcon(":/res/image/icon/activity.png"); }
        else
        { return (QVariant)QIcon(":/res/image/icon/workbench.png"); }
    }
    else
    {}

    return QVariant();
}

bool scriptModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( !index.isValid() )
    { return false; }

    if (data(index, role) != value) {
        // FIXME: Implement me!

        scriptNode *pNode = getItem( index );
        QString str = value.toString();
        //! check
        if ( str.length() < 1 )
        { return false; }
        pNode->setName( value.toString() );

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags scriptModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled | QAbstractItemModel::flags(index);
}

bool scriptModel::insertRows(int row, int count, const QModelIndex &parent)
{
//    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
//    endInsertRows();
    return false;
}

//bool scriptModel::insertColumns(int column, int count, const QModelIndex &parent)
//{
//    beginInsertColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endInsertColumns();
//    return false;
//}

bool scriptModel::removeRows(int row, int count, const QModelIndex &parent)
{
//    beginRemoveRows(parent, row, row + count - 1);
//    // FIXME: Implement me!

//    scriptNode *pNode = getItem( parent )->child( row );
//    logDbg()<<pNode->getName();
//    pNode->remove();
//    delete pNode;

//    endRemoveRows();
    return false;
}

Qt::DropActions scriptModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

QStringList scriptModel::mimeTypes() const
{
    QStringList types;
    types << "application/robo_script";
    return types;
}

//bool scriptModel::removeColumns(int column, int count, const QModelIndex &parent)
//{
//    beginRemoveColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endRemoveColumns();
//    return false;
//}

QMimeData *scriptModel::mimeData(const QModelIndexList &indexes) const
{
    if ( indexes.size() > 1 || indexes.size() < 1 )
    { return NULL; }

    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    foreach (const QModelIndex &index, indexes)
    {
         if (index.isValid())
         {
             scriptNode *pNode = getItem( index );
             encodedData = QByteArray::number( (quint32)pNode, 16 );
             logDbg()<<pNode->getName();
         }
    }

    mimeData->setData("application/robo_script", encodedData);
    return mimeData;
}

bool scriptModel::canDropMimeData(const QMimeData *data,
     Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(parent);

    if (!data->hasFormat("application/robo_script"))
     return false;

    if (column > 0)
     return false;
logDbg();

    scriptNode *pNode = getItem(parent);
    if ( pNode->getNodeType() != scriptNode::node_group )
    { return false; }

    return true;
}

bool scriptModel::dropMimeData(const QMimeData *data,
     Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (!canDropMimeData(data, action, row, column, parent))
    { return false; }
logDbg();
    if (action == Qt::IgnoreAction)
     return true;
logDbg();
    int beginRow;

    if (row != -1)
    { beginRow = row; }
    else if (parent.isValid())
    { beginRow = parent.row(); }
    else
    { beginRow = rowCount(QModelIndex()); }

    QByteArray encodedData = data->data("application/robo_script");
    if ( encodedData.size() != 8 )
    { return false; }
logDbg();
    quint32 ptrData = encodedData.toUInt(NULL,16);
    scriptNode *pNode = (scriptNode*)ptrData;
    if ( NULL == pNode )
    { return false; }
logDbg()<<beginRow;
    insertRows( beginRow, 1 );

    scriptNode *pParent = getItem( parent );
//    if ( row >= pParent->childCount() )
//    { return false; }

//    pNode->remove();
//    pParent->insertNode( beginRow, pNode );

    logDbg()<<pParent->getName()<<pParent->childCount();
//    beginInsertRows( parent, 0, pParent->childCount()-1  );

    //! \todo warnnings
    pNode->remove();
    pParent->insertNode( beginRow, pNode );
logDbg()<<pParent->childCount();
//    beginInsertRows( parent, 0, pParent->childCount()-1  );
    beginInsertRows( parent, beginRow, beginRow  );
    endInsertRows();

//    beginInsertRows( QAbstractItemModel::parent(parent),
//                     0,
//                     pParent->getParent()->childCount()-1  );
//    endInsertRows();

    return true;
}

int scriptModel::save( const QString &fileName )
{
    Q_ASSERT( NULL != m_pRootNode );

    int ret = m_pRootNode->save( fileName );

    return ret;
}
int scriptModel::load( const QString &fileName )
{
    Q_ASSERT( NULL != m_pRootNode );

    beginResetModel();

    m_pRootNode->clear();

    int ret = m_pRootNode->load( fileName );

    endResetModel();

    return ret;
}

void scriptModel::reNew()
{
    beginResetModel();

    //! \todo close all editor
    m_pRootNode->clear();

    endResetModel();
}

int scriptModel::saveAll()
{
    dynamic_cast<scriptGroup*>(m_pRootNode)->saveAll( m_pRootNode->getPath() );

    return 0;
}

void scriptModel::appendNode( const QModelIndex &index,
                 scriptNode *pNode )
{
    scriptNode *pCurNode = getItem( index );

    beginInsertRows( parent(index), rowCount( parent(index)), rowCount( parent(index))  );

    if ( pCurNode == NULL )
    {
        m_pRootNode->appendNode( pNode );
    }
    else if ( pCurNode->getNodeType() == scriptNode::node_group )
    {
        pCurNode->appendNode( pNode );
    }
    else
    {
        pCurNode->getParent()->appendNode( pNode );
    }

    endInsertRows();
}

void scriptModel::removeNode( const QModelIndex &index )
{
    if ( !index.isValid() )
    { return; }

    scriptNode *pNode = getItem( index );
    if ( pNode->getParent() == m_pRootNode && pNode->getNodeType() == scriptNode::node_group )
    { return; }

    beginResetModel();

    pNode->remove();
    delete pNode;

    endResetModel();
}

scriptNode *scriptModel::getItem( const QModelIndex &index ) const
{
    if (index.isValid())
    {
        scriptNode *item = static_cast<scriptNode *>(index.internalPointer());
        if (item)
        { return item; }
    }

    //! root node must be valid
    Q_ASSERT( NULL !=m_pRootNode );
    return m_pRootNode;

}

void scriptModel::setName( const QString &name )
{
    m_pRootNode->setName( name );
}
QString scriptModel::getName()
{
    return m_pRootNode->getName();
}

void scriptModel::setPath( const QString &path )
{
    m_pRootNode->setPath( path );
}
QString scriptModel::getPath()
{
    return m_pRootNode->getPath();
}

scriptNode *scriptModel::rootNode()
{
    return m_pRootNode;
}
