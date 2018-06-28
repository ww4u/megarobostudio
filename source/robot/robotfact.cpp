
#include "robotfact.h"

VRobot *robotFact::createRobot( const QString &str )
{
    VRobot *pRobo;
    do
    {
        if ( QString::compare(str,"megatron",Qt::CaseInsensitive) == 0
             || QString::compare(str,"mrx-as",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new robotMegatron();
            break;
        }

        if ( QString::compare(str,"sinanju",Qt::CaseInsensitive) == 0
             || QString::compare(str,"mrx-t4",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new robotSinanju();
            break;
        }
        if ( QString::compare(str,"delta",Qt::CaseInsensitive) == 0
             || QString::compare(str,"mrx-dt",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new robotDelta();
            break;
        }
        if ( QString::compare(str,"h2",Qt::CaseInsensitive) == 0
             || QString::compare(str,"mrx-h2",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new robotH2();
            break;
        }

        if ( QString::compare(str,"mrx-h2z",Qt::CaseInsensitive) == 0
             || QString::compare(str,"mrx-h2-z",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new robotH2Z();
            break;
        }

        if ( QString::compare(str,"mrx-inj2",Qt::CaseInsensitive) == 0
              )
        {
            pRobo = new robotInject();
            break;
        }

        if ( QString::compare(str,"mrx-motor",Qt::CaseInsensitive) == 0
              )
        {
            pRobo = new robotMotor();
            break;
        }

//        if ( QString::compare(str,"motor",Qt::CaseInsensitive) == 0 )
//        {
//            pRobo = new robotMotor();
//            break;
//        }

//        if ( QString::compare(str,"slide",Qt::CaseInsensitive) == 0 )
//        {
//            pRobo = new robotSlide();
//            break;
//        }
    //    if ( QString::compare(str,"slide2",Qt::CaseInsensitive) == 0 )
    //    {
    //        return new robotSlide2();
    //    }

        if ( QString::compare(str,"quebeley",Qt::CaseInsensitive) == 0
             || QString::compare(str,"MRQ-C-23-D",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new roboQuebeley( VRobot::robot_qubeley_d );
            break;
        }

        if ( QString::compare(str,"MRQ-C-23-S",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new roboQuebeley( VRobot::robot_qubeley_s );
            break;
        }
        if ( QString::compare(str,"geogoog",Qt::CaseInsensitive) == 0
             || QString::compare(str,"MRQ-M-23-04",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new roboGeogoog();
            break;
        }

        if ( QString::compare(str,"geogoog10",Qt::CaseInsensitive) == 0
             || QString::compare(str,"MRQ-M1710",Qt::CaseInsensitive) == 0
             || QString::compare(str,"MRQ_MC_1710",Qt::CaseInsensitive) == 0
             || QString::compare(str,"geo10",Qt::CaseInsensitive)==0 )
        {
            pRobo = new roboGeo10();
            break;
        }

        if ( QString::compare(str,"geogoog5_1",Qt::CaseInsensitive) == 0
             || QString::compare(str,"MRQ-MV",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new roboGeo51();
            break;
        }

        //! gouf
        if ( QString::compare(str,"MRV-M3604",Qt::CaseInsensitive) == 0 )
        {
            pRobo = new roboGouf();
            break;
        }

        else
        {
            logDbg()<<str;
            Q_ASSERT(false);
            return NULL;
        }

    }while(0);

    Q_ASSERT( NULL != pRobo );
    pRobo->postCtor();
    return pRobo;
}



