#include "checkdelegate.h"
#include <QtWidgets>
#include "../../include/mcstd.h"
//#define CHECK_WIDGET    myInvertButton

#define CHECK_WIDGET    QCheckBox

checkDelegate::checkDelegate( checkShape shp,
                              QObject *parent ) : QStyledItemDelegate(parent)
{
    mShape = shp;

    mPrimSize.setWidth( 0 );
    mPrimSize.setHeight( 0 );

    if ( mShape == shape_check )
    {
        QCheckBox chk;
        mPrimSize = chk.sizeHint();
    }
    else if ( mShape == shape_radio )
    {
        QRadioButton rad;
        mPrimSize = rad.sizeHint();
    }
    else
    {}
}

QWidget *checkDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    if ( mShape == shape_check )
    {
        CHECK_WIDGET *pCheckbox = new CHECK_WIDGET( parent );

        pCheckbox->setCheckable( true );

        return pCheckbox;
    }
    else if ( mShape == shape_radio )
    {
        QRadioButton *pRadio = new QRadioButton( parent );

        return pRadio;
    }
    else
    {
        Q_ASSERT( false );
        return NULL;
    }
}

void checkDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    bool b = index.model()->data(index, Qt::EditRole).toBool();

    CHECK_WIDGET *pCheckBox = static_cast<CHECK_WIDGET*>(editor);

    pCheckBox->setChecked( b );
//    pCheckBox->setText( b ? tr("true"):tr("false") );
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
    QRect localRect;

    localRect = option.rect;
    localRect.setX( option.rect.left() + ( option.rect.width() - mPrimSize.width() ) / 2 );

    editor->setGeometry( localRect );
}

void checkDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.isValid() )
    {}
    else
    { return; }

    QVariant var = index.data( Qt::UserRole );
    if ( var.isValid() && var.type() == QVariant::Bool )
    {
        if ( var.toBool() )
        {}
        else
        { return; }
    }
    else
    {}

    QStyleOption opt=option;
    QStyleOptionButton optBtn;

    optBtn.rect = option.rect;

    //! value
    if ( index.data( Qt::DisplayRole ).toBool() )
    { optBtn.state = QStyle::State_On | QStyle::State_Enabled; }
    else
    { optBtn.state = QStyle::State_Off | QStyle::State_Enabled; }

    //! tune the rect
    optBtn.rect.setX( optBtn.rect.left() + ( option.rect.width() - mPrimSize.width())/2 );

    painter->save();

        if ( mShape == shape_check )
        {
            QApplication::style()->drawControl( QStyle::CE_CheckBox, &optBtn, painter );
        }
        else if ( mShape == shape_radio )
        {
            QApplication::style()->drawControl( QStyle::CE_RadioButton, &optBtn, painter );
        }
        else
        {}

    painter->restore();

//    logDbg();
}

void checkDelegate::setShape( checkShape shp )
{ mShape = shp; }
checkShape checkDelegate::shape()
{ return mShape; }

//myInvertButton::myInvertButton(QWidget *parent ) : QPushButton(parent)
//{
//    setCheckable( true );

//    connect( this, SIGNAL(toggled(bool)),
//             this, SLOT(slot_toggled(bool)));
//}

//void myInvertButton::slot_toggled( bool bCheck )
//{
//    setText( bCheck ? tr("true"):tr("false") );
//}
