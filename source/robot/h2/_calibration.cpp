#include "../h2.h"

void robotH2::setZeroAttr( double zeroTime, double zeroAngle, double zeroSpeed )
{
    mZeroTime = zeroTime;
    mZeroAngle = zeroAngle;
    mZeroSpeed = zeroSpeed;
}
void robotH2::zeroAttr( double &zeroTime, double &zeroAngle, double &zeroSpeed )
{
    zeroTime = mZeroTime;
    zeroAngle = mZeroAngle;
    zeroSpeed = mZeroSpeed;
}
