
#include "robotfact.h"

VRobot *robotFact::createRobot( const QString &str, bool bPost )
{
    VRobot *pRobo;
    do
    {logDbg()<<str;
        if ( str_is_2( str, "megatron", "mrx-as") )
        {
            pRobo = new robotMegatron();
            break;
        }

        if ( str_is_2( str, "sinanju", "mrx-t4") )
        {
            pRobo = new robotSinanju();
            break;
        }

        if ( str_is_2( str, "delta", "mrx-dt") )
        {
            pRobo = new robotDelta();
            break;
        }

        if ( str_is_2( str, "h2", "mrx-h2") )
        {
            pRobo = new robotH2();
            break;
        }

        if ( str_is( str, "mrx-h2m") )
        {
            pRobo = new robotH2M();
            break;
        }

//        if ( str_is_2( str, "mrx-h2-z", "mrx-h2z") )
//        {
//            pRobo = new robotH2Z();
//            break;
//        }

        if ( str_is( str, "mrx-inj2") )
        {
            pRobo = new robotInject();
            break;
        }

        if ( str_is( str, "mrx-motor") )
        {
            pRobo = new robotMotor();
            break;
        }

        if ( str_is( str, "mrx-motors") )
        {
            pRobo = new robotMotors();
            break;
        }

        //! igus
        if ( str_is( str, "igus-drylin") )
        {
            pRobo = new robotIgusDelta();
            break;
        }

        //! 1.0
        if ( str_is_2(str, "quebeley", "MRQ-C-23-D") )
        {
            pRobo = new roboQuebeley( VRobot::robot_qubeley_d );
            break;
        }

        //! 1.1
        if ( str_is(str, "MRQ-C-23-S") )
        {
            pRobo = new roboQuebeley( VRobot::robot_qubeley_s );
            break;
        }

        if ( str_is_2(str, "geogoog", "MRQ-M-23-04") )
        {
            pRobo = new roboGeogoog();
            break;
        }

        if ( str_is_4(str, "geogoog10", "MRQ-M1710", "MRQ_MC_1710", "geo10") )
        {
            pRobo = new roboGeo10();
            break;
        }

        if ( str_is_2(str, "geogoog5_1", "MRQ-MV") )
        {
            pRobo = new roboGeo51();
            break;
        }

        if ( str_is(str, "MRQ-M2302") )
        {logDbg();
            pRobo = new roboGeo2();
            break;
        }

        //! gouf
        if ( str_is(str, "MRV-M3604") )
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

    if ( bPost )
    {
        Q_ASSERT( NULL != pRobo );
        pRobo->postCtor();
    }
    return pRobo;
}



