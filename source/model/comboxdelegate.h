#ifndef COMBOXDELEGATE_H
#define COMBOXDELEGATE_H
#include <QStyledItemDelegate>
#include <QMap>

class comboxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    comboxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setItems( const QStringList &strList,
                   const QList<int> &vals );
    void setItems( const QStringList &strList,
                   const int base = 0 );

    int value( const QString &name, int *pVal );
    QString toString( int id );


protected:
    QMap<QString, int> mTables;
//    QComboBox *m_pCombobox;
};

#endif // COMBOXDELEGATE_H
