#include "deviceMRQ.h"

namespace MegaDevice {

static float _stepAngles[]=
{
    1.8f,
    0.9f,
    15.0f,
    7.5f
};

static float _microSteps[]=
{
    256.0f,
    128.0f,
    64.0f,
    32.0f,
    16.0f,
    8.0f,
    4.0f,
    2.0f,
    1.0f
};

float deviceMRQ::slowRatio( int ax )
{
    Q_ASSERT( ax >= 0 && ax < axes() );

    //! rot
    if ( mMOTOR_TYPE[ax] == MRQ_MOTOR_TYPE_ROTARY )
    {
        if ( mMOTOR_GEARRATIODEN[ax] > 0 )
        {}
        else
        {
            sysError( QObject::tr("Invalid slow ratio") );
            return 1.0;
        }

        //! degree or rad
        if ( mMOTOR_POSITIONUNIT[ax] == MRQ_MOTOR_POSITIONUNIT_ANGLE )
        { return (float)mMOTOR_GEARRATIONUM[ax] / mMOTOR_GEARRATIODEN[ax]; }
        else
        { return 180.0f / MATH_PI * (float)mMOTOR_GEARRATIONUM[ax] / mMOTOR_GEARRATIODEN[ax]; }
    }
    //! lead
    else
    {
        if ( mMOTOR_LEAD[ax] > 0 )
        {}
        else
        {
            sysError( QObject::tr("Invalid slow ratio") );
            return 1.0;
        }

        return  360.0f / mMOTOR_LEAD[ax];
    }
}
float deviceMRQ::stepAngle( int ax )
{
    Q_ASSERT( ax >= 0 && ax < axes() );
    Q_ASSERT( mMOTOR_STEPANGLE[ ax ] >= 0
              && mMOTOR_STEPANGLE[ ax ] < sizeof_array(_stepAngles) );

    return _stepAngles[ mMOTOR_STEPANGLE[ ax ] ];
}
float deviceMRQ::microStep( int ax )
{
    Q_ASSERT( ax >= 0 && ax < axes() );
    Q_ASSERT( mDRIVER_MICROSTEPS[ ax ] >= 0
              && mDRIVER_MICROSTEPS[ ax ] < sizeof_array(_microSteps) );

    return _microSteps[ mDRIVER_MICROSTEPS[ ax ] ];
}

}

