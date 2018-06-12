#include "injectpump.h"

void robotInject::setZeroAttr( double zeroTime, double zeroDist )
{
    mZeroTime = zeroTime;
    mZeroDistance = zeroDist;
}
void robotInject::zeroAttr( double &zeroTime, double &zeroDist )
{
    zeroTime = mZeroTime;
    zeroDist = mZeroDistance;
}

void robotInject::setGap( double vtime, double vdist,
                          double itime, double idist )
{
    mVGapTime = vtime;
    mVGapDistance = vdist;

    mIGapTime = itime;
    mIGapDistance = idist;
}
void robotInject::gap( double &vtime, double &vdist,
                       double &itime, double &idist )
{
    vtime = mVGapTime;
    vdist = mVGapDistance;

    itime = mIGapTime;
    idist = mIGapDistance;
}

void robotInject::setInjAttr( InjProp &prop )
{ mProp = prop; }
void robotInject::injAttr( InjProp &prop )
{ prop = mProp; }
