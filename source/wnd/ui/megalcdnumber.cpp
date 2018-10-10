#include "megalcdnumber.h"
#include <float.h>

MegaLcdnumber::MegaLcdnumber(QWidget *parent) :
    QLCDNumber(parent)

{
}

MegaLcdnumber::~MegaLcdnumber()
{
}

void MegaLcdnumber::display( float v )
{
    if ( checkOverflow( v ) )
    { QLCDNumber::display( FLT_MAX );}
    else
    { QLCDNumber::display( v ); }
}
void MegaLcdnumber::display( double v )
{
    if ( checkOverflow( v ) )
    { QLCDNumber::display( FLT_MAX );}
    else
    { QLCDNumber::display( v ); }
}
void MegaLcdnumber::display( int v )
{
    if ( checkOverflow( v ) )
    { QLCDNumber::display( INT_MAX );}
    else
    { QLCDNumber::display( v ); }
}
void MegaLcdnumber::display( const QString &v )
{
    QLCDNumber::display( v );
}
