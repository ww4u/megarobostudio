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

void robotH2::setGap( double time,
                      double dist,
                      double zTime,
                      double zDist
                      )
{
    mGapTime = time;
    mGapDistance = dist;

    mGapZTime = zTime;
    mGapZDistance = zDist;
}
void robotH2::gap( double &time,
                   double &dist,
                   double &zTime,
                   double &zDist )
{
    time = mGapTime;
    dist = mGapDistance;

    zTime = mGapZTime;
    zDist = mGapZDistance;
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

void robotH2::setGantry( int mod, int gear, int pdir, int motion )
{
    mToothType = mod;
    mToothGear = gear;

    mPDirIndex = pdir;
    mMotionIndex = motion;
}
void robotH2::gantry( int &mod, int &gear, int &pdir, int &motion )
{
    mod = mToothType;
    gear = mToothGear;

    pdir = mPDirIndex;
    motion = mMotionIndex;
}

void robotH2::setZeroMovement( int zMoveMent )
{ mZeroMovement = (H2ZeroMovement)zMoveMent; }
int robotH2::zeroMovement()
{ return mZeroMovement; }

void robotH2::setZeroCorner( int corner )
{
    mZeroCorner = (H2ZeroCorner)corner;

    postConfigTransfer();
}
int robotH2::zeroCorner()
{ return mZeroCorner; }

//void robotH2::setTransferAble( bool b)
//{ mbTransferAble = b; }
//bool robotH2::transferAble()
//{ return mbTransferAble; }

void robotH2::setTransfer( /*bool b,*/
                           double t[2*2],
                           double s[2*1],
                           double invt[2*2] )
{
//    mbTransferAble = b;
    memcpy( mTransferR, t, sizeof(mTransferR) );
    memcpy( mTransferS, s, sizeof(mTransferS) );
    memcpy( mTransferRInv, invt, sizeof(mTransferRInv) );
}
void robotH2::transfer( /*bool &b,*/
                        double t[2*2],
                        double s[2*1],
                        double invt[2*2] )
{
//    b = mbTransferAble;
    memcpy( t, mTransferR, sizeof(mTransferR) );
    memcpy( s, mTransferS, sizeof(mTransferS) );
    memcpy( invt, mTransferRInv, sizeof(mTransferRInv) );
}
