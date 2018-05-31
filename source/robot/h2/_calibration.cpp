#include "../h2.h"

void robotH2::setZeroAttr( double zeroTime, double zeroDist )
{
    mZeroTime = zeroTime;
    mZeroDistance = zeroDist;
}
void robotH2::zeroAttr( double &zeroTime, double &zeroDist )
{
    zeroTime = mZeroTime;
    zeroDist = mZeroDistance;
}

void robotH2::setGap( double time, double dist )
{
    mGapTime = time;
    mGapDistance = dist;
}
void robotH2::gap( double &time, double &dist )
{
    time = mGapTime;
    dist = mGapDistance;
}

void robotH2::setCenter( float x, float y )
{
    mZeroX = x;
    mZeroY = y;
}
void robotH2::center( float &x, float &y )
{
    x = mZeroX;
    y = mZeroY;
}
