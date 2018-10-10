#include "checkdelegate.h"
#include <QtWidgets>

#define CHECK_WIDGET    myInvertButton

checkDelegate::checkDelegate( QObject *parent ) : QStyledItemDelegate(parent)
{

}

QWidget *checkDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    CHECK_WIDGET *pCheckbox = new CHECK_WIDGET( parent );

    pCheckbox->setCheckable( true );

    return pCheckbox;
}

void checkDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    bool b = index.model()->data(index, Qt::EditRole).toBool();

    CHECK_WIDGET *pCheckBox = static_cast<CHECK_WIDGET*>(editor);

    pCheckBox->setChecked( b );
    pCheckBox->setText( b ? tr("true"):tr("false") );
}
void checkDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    CHECK_WIDGET *pCheckBox = static_cast<CHECK_WIDGET*>(editor);

    model->setData( index, pCheckBox->isChecked(), Qt::EditRole );
}

void checkDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

myInvertButton::myInvertButton(QWidget *parent ) : QPushButton(parent)
{
    setCheckable( true );

    connect( this, SIGNAL(toggled(bool)),
             this, SLOT(slot_toggled(bool)));
}

void myInvertButton::slot_toggled( bool bCheck )
{
    setText( bCheck ? tr("true"):tr("false") );
}
