
#include "robotfact.h"

VRobot *robotFact::createRobot( const QString &str )
{
    if ( QString::compare(str,"megatron",Qt::CaseInsensitive) == 0 )
    {
        return new robotMegatron();
    }
    if ( QString::compare(str,"motor",Qt::CaseInsensitive) == 0 )
    {
        return new robotMotor();
    }
    if ( QString::compare(str,"delta",Qt::CaseInsensitive) == 0 )
    {
        return new robotDelta();
    }
    if ( QString::compare(str,"slide1",Qt::CaseInsensitive) == 0 )
    {
        return new robotSlide1();
    }
    if ( QString::compare(str,"slide2",Qt::CaseInsensitive) == 0 )
    {
        return new robotSlide2();
    }

    if ( QString::compare(str,"quebeley",Qt::CaseInsensitive) == 0 )
    {
        return new roboQuebeley();
    }
    if ( QString::compare(str,"geogoog",Qt::CaseInsensitive) == 0 )
    {
        return new roboGeogoog();
    }

    else
    { return NULL; }
}



