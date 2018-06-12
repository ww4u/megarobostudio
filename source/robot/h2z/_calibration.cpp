#include "../h2z.h"

void robotH2Z::setZeroAttr( double zeroTime, double zeroDist )
{
    mZeroTime = zeroTime;
    mZeroDistance = zeroDist;
}
void robotH2Z::zeroAttr( double &zeroTime, double &zeroDist )
{
    zeroTime = mZeroTime;
    zeroDist = mZeroDistance;
}

void robotH2Z::setGap( double time, double dist )
{
    mGapTime = time;
    mGapDistance = dist;
}
void robotH2Z::gap( double &time, double &dist )
{
    time = mGapTime;
    dist = mGapDistance;
}

void robotH2Z::setCenter( float x, float y )
{
    mZeroX = x;
    mZeroY = y;
}
void robotH2Z::center( float &x, float &y )
{
    x = mZeroX;
    y = mZeroY;
}
