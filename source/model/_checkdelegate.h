#ifndef CHECKDELEGATE_H
#define CHECKDELEGATE_H
#include <QStyledItemDelegate>
#include <QtWidgets>

class checkDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    checkDelegate( QObject *parent = 0 );

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class myInvertButton : public QPushButton
{
    Q_OBJECT
public:
    myInvertButton(QWidget *parent = Q_NULLPTR);

protected Q_SLOTS:
    void slot_toggled( bool bCheck );
};

#endif // CHECKDELEGATE_H
