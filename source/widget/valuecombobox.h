#ifndef VALUECOMBOBOX_H
#define VALUECOMBOBOX_H

#include <QtWidgets>

class ValueComboBox : public QComboBox
{
    Q_OBJECT
public:
    ValueComboBox( QWidget *parent = Q_NULLPTR );

public:
    void addItem( const QString &str, int value );
    void addItemInt( const QString &str );

    void setValue( int val );
    int value();

};

#endif // VALUECOMBOBOX_H
