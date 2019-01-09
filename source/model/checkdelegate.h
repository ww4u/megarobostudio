#ifndef CHECKDELEGATE_H
#define CHECKDELEGATE_H
#include <QStyledItemDelegate>
#include <QtWidgets>

enum checkShape
{
    shape_check,
    shape_radio,
};

class checkDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    checkDelegate( checkShape shp = shape_check,
                   QObject *parent = 0 );

public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    virtual void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

public:
    void setShape( checkShape shp );
    checkShape shape();

protected:
    checkShape mShape;

    QSize mPrimSize;
};

//class myInvertButton : public QPushButton
//{
//    Q_OBJECT
//public:
//    myInvertButton(QWidget *parent = Q_NULLPTR);

//protected Q_SLOTS:
//    void slot_toggled( bool bCheck );
//};

#endif // CHECKDELEGATE_H
