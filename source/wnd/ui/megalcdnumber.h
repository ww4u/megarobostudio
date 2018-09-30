#ifndef MEGALCD_H
#define MEGALCD_H

#include <QLCDNumber>


class MegaLcdnumber : public QLCDNumber
{
    Q_OBJECT

public:
    explicit MegaLcdnumber(QWidget *parent = 0);
    ~MegaLcdnumber();

public:
    void display( float v );
    void display( double v );
    void display( int n );
    void display( const QString &v );
};

#endif // MEGALCD_H
