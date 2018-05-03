#include "comboxdelegate.h"
#include <QtWidgets>

comboxDelegate::comboxDelegate( QObject *parent ) : QStyledItemDelegate(parent)
{

}

QWidget *comboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    QComboBox *pCombox = new QComboBox( parent );
    Q_ASSERT( NULL != pCombox );

    pCombox->setFrame( false );
    pCombox->setInsertPolicy( QComboBox::NoInsert );
    pCombox->setEditable( false );

    QMapIterator<QString, int > iter(mTables);
    while( iter.hasNext() )
    {
        iter.next();

        pCombox->addItem( iter.key() );
    }

    return pCombox;
}

void comboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if ( index.isValid() )
    {}
    else
    { return; }

    QString value = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *pCombox = static_cast<QComboBox*>(editor);

    //! not empty
    if ( value.length() > 0 )
    {}
    else
    {
        pCombox->setCurrentIndex( 0 );
        return;
    }

    if ( mTables.find(value) != mTables.end() )
    {
        pCombox->setCurrentIndex( mTables.find(value).value() );
    }
    else
    {
        pCombox->setCurrentIndex( 0 );
    }
}

void comboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    QComboBox *pCombox = static_cast<QComboBox*>(editor);
    QString value = pCombox->currentText();

    model->setData(index, value, Qt::EditRole);
}

void comboxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void comboxDelegate::setItems( const QStringList &strList,
               const QList<int> &vals )
{
    Q_ASSERT( vals.size() == strList.size() );

    for( int i = 0; i < strList.size(); i++ )
    {
        mTables.insert( strList[i], vals[i] );
    }
}
void comboxDelegate::setItems( const QStringList &strList,
               const int base )
{
    for( int i = 0; i < strList.size(); i++ )
    {
        mTables.insert( strList[i], base + i );
    }
}

int comboxDelegate::value( const QString &name,
                           int *pVal )
{
    foreach( QString str, mTables.keys() )
    {
        if ( name.compare( str, Qt::CaseInsensitive) == 0 )
        {
            *pVal = mTables[str];
            return 0;
        }
    }

    return 0;

}

QString comboxDelegate::toString( int id )
{
    QMapIterator< QString, int > iter( mTables );

    while (iter.hasNext())
    {
          iter.next();

          if ( iter.value() == id )
          { return iter.key(); }
    }

    return mTables.firstKey();
}

