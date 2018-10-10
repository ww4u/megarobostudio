#include "valuecombobox.h"

ValueComboBox::ValueComboBox( QWidget *parent ) : QComboBox( parent )
{

}

void ValueComboBox::addItem( const QString &str, int value )
{
    QComboBox::addItem( str, QVariant(value) );
}

void ValueComboBox::addItemInt( const QString &str )
{
    int value = str.toInt();
    QComboBox::addItem( str, QVariant(value) );
}

void ValueComboBox::setValue( int val )
{
    for ( int i = 0; i < count(); i++ )
    {
        if ( itemData(i).toInt() == val )
        {
            setCurrentIndex( i );
            return;
        }
    }
}
int ValueComboBox::value()
{
    return currentData().toInt();
}
