
#include "robotfact.h"

VRobot *robotFact::createRobot( const QString &str )
{
    VRobot *pRobo;
    do
    {
        if ( QString::compare(str,"megatron",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new robotMegatron();
            break;
        }

        if ( QString::compare(str,"sinanju",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new robotSinanju();
            break;
        }
        if ( QString::compare(str,"delta",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new robotDelta();
            break;
        }
        if ( QString::compare(str,"h2",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new robotH2();
            break;
        }

    //    if ( QString::compare(str,"motor",Qt::CaseInsensitive) == 0 )
    //    {
    //        return new robotMotor();
    //    }

    //    if ( QString::compare(str,"slide1",Qt::CaseInsensitive) == 0 )
    //    {
    //        return new robotSlide1();
    //    }
    //    if ( QString::compare(str,"slide2",Qt::CaseInsensitive) == 0 )
    //    {
    //        return new robotSlide2();
    //    }

        if ( QString::compare(str,"quebeley",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new roboQuebeley();
            break;
        }
        if ( QString::compare(str,"geogoog",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new roboGeogoog();
            break;
        }
        if ( QString::compare(str,"geogoog8",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new roboGeo8();
            break;
        }
        if ( QString::compare(str,"geogoog10",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new roboGeo10();
            break;
        }
        if ( QString::compare(str,"geogoog5_1",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new roboGeo51();
            break;
        }

        else
        { return NULL; }

    }while(0);

    Q_ASSERT( NULL != pRobo );
    pRobo->postCtor();
    return pRobo;
}



