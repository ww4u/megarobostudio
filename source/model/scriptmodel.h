#ifndef SCRIPTMODEL_H
#define SCRIPTMODEL_H

#include <QAbstractItemModel>

#include "scriptgroup.h"

class scriptModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit scriptModel(QObject *parent = nullptr);
    ~scriptModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

//    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

//    bool canFetchMore(const QModelIndex &parent) const override;
//    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

//    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    Qt::DropActions supportedDropActions() const override;

    QStringList mimeTypes() const override;

    QMimeData *mimeData(const QModelIndexList &indexes) const;

    bool canDropMimeData(const QMimeData *data,
         Qt::DropAction action, int row, int column, const QModelIndex &parent);

    bool dropMimeData(const QMimeData *data,
       Qt::DropAction action, int row, int column, const QModelIndex &parent);

public:
    int save( const QString &fileName );
    int load( const QString &fileName );
    void reNew();

    int saveAll();

    void appendNode( const QModelIndex &index,
                     scriptNode *pNode );
    void removeNode( const QModelIndex &index );

    scriptNode *getItem( const QModelIndex &index ) const;

public:
    void setName( const QString &name );
    QString getName();

    void setPath( const QString &path="" );
    QString getPath();

    scriptNode *rootNode();
private:
    scriptNode *m_pRootNode;        //! root is a group
};

#endif // SCRIPTMODEL_H
