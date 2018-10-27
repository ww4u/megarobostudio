
#ifndef _ROBOT_H2_H_
#define _ROBOT_H2_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

#include "../../model/handactionmodel.h"

//! arith
#include "../../arith/pathplan/pathplan.h"
//#include "../../arith/kinematic/kinematic.h"

struct H2ZeroArg : public RoboTaskArgument
{
    int mAx;                        //! 0 -- x
    float mZeroTime;
    float mZeroDist,mZeroEndV;
    float mZeroGapDist, mZeroGapTime;

    //! x,y dist
    float mZeroXDist,mZeroXEndV, mZeroXGapDist;
    float mZeroYDist,mZeroYEndV, mZeroYGapDist;

    float mZeroZDist, mZeroZTime, mZeroZEndV;
    float mZeroGapZDist, mZeroGapZTime;

    H2ZeroArg();
};

enum H2ZeroCorner
{
    corner_lb,
    corner_lt,
    corner_rt,
    corner_rb
};

enum H2ZeroMovement
{
    movement_x_y,
    movement_y_x,
};

class H2Task : public RoboTask
{
    Q_OBJECT
public:
    H2Task( QObject *pParent = NULL );

protected:
    virtual void run();
};

class robotH2 : public RawRobo
{
public:
    typedef int (*apiH2Request)( void *pArg );

public:
    robotH2();
    ~robotH2();
public:
    DECLARE_SCPI()

public:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );

    virtual void postload();
protected:
    void postConfigTransfer();

public:
    virtual int download( VRobot *pSetup );

public:
    virtual int build( MegaTableModel *pModel,
                       xxxGroup<tracePoint> &tracePlan,
                       xxxGroup<jointsTrace> &jointsPlan,
                       QList< tpvGroup *> &tpvGroups,
                       QList< int> &sectionList );

public:
    int program( QList<H2KeyPoint> &curve,
                 const tpvRegion &region );
    int move( QList<H2KeyPoint> &curve,
              const tpvRegion &region );
    int move( float x,  float y,
              float dstx, float dsty, float dt, float endVx, float endVy,
              const tpvRegion &region );

    int preMove( QList<H2KeyPoint> &curve,
              const tpvRegion &region );

    //! jId -- 0: x
    virtual int goZero( const tpvRegion &region,
                        int jointId, bool bCcw );
    virtual int goZero( const tpvRegion &region=0 );      //! 1.x, 2.y
    virtual int goZero( const tpvRegion &region,
                        const QList<int> &jointList,
                        const QList<bool> &ccwList );

    virtual int getPOSE( float pos[] );

    int zeroAxesTask( void *pArg );

    int zeroX( H2ZeroArg *pZArg );
    int zeroY( H2ZeroArg *pZArg );

    int gapX( H2ZeroArg *pZArg );
    int gapY( H2ZeroArg *pZArg );

    int clrAngle();

    int angle( int jId, float &fAng );
    int pose( float &x, float &y );

    void coordRotate( QList<H2KeyPoint> &curve );
    void coordRotate( H2KeyPoint &pt, double rot[2*2], double shift[2*1] );

    void coordIRotate( H2KeyPoint &pt );
    void coordIRotate( H2KeyPoint &pt, double rot[2*2], double shift[2*1] );

    int moveTest1( const tpvRegion &region=0 );
    int moveTest2( const tpvRegion &region=0 );

public:
    void setZeroAttr( double zeroTime,
                      double zeroDist );
    void zeroAttr( double &zeroTime, double &zeroDist );

    void setGap( double time, double dist,
                 double zTime, double zDist );
    void gap( double &time, double &dist,
              double &zTime, double &zDist );

    void setCenter( float x, float y );
    void center( float &x, float &y );

    void setGantry( int mod, int gear, int pdir, int motion );
    void gantry( int &mod, int &gear, int &pdir, int &motion );

    void setZeroMovement( int zMoveMent );
    int zeroMovement();

    void setZeroCorner( int corner );
    int zeroCorner();

    //! transfer
//    void setTransferAble( bool b);
//    bool transferAble();

    void setTransfer( /*bool b,*/
                      double t[2*2],
                      double s[2*1],
                      double invt[2*2] );
    void transfer( /*bool &b,*/
                   double t[2*2],
                   double s[2*1],
                   double invt[2*2] );

    int serialOutZero( QXmlStreamWriter &writer);
    int serialInZero( QXmlStreamReader &reader );

    int serialOutGantry( QXmlStreamWriter &writer);
    int serialInGantry( QXmlStreamReader &reader);

    int serialOutTransfer( QXmlStreamWriter &writer);
    int serialInTransfer( QXmlStreamReader &reader);

    int serialInTransferR( QXmlStreamReader &reader);
    int serialInTransferS( QXmlStreamReader &reader);
    int serialInTransferRInv( QXmlStreamReader &reader);

protected:
    int verifyTrace( QList<H2KeyPoint> &curve );
    int buildTrace( QList<H2KeyPoint> &curve,
                    QList< tpvGroup *> &jointsGroup,
                    QList< int > &sectionList );
    int downloadTrace( const tpvRegion &region,
                       QList< tpvGroup *> &jointsGroup );

protected:
    double mZeroTime, mZeroDistance;
    double mGapTime, mGapDistance;
    double mGapZTime, mGapZDistance;
    double mZeroX, mZeroY;

    QList <int> mAxesDirs;      //! realX = dir * x + zeroX

    QList<int> mEncoderDirs;    //! 1 or -1
    int mLines;

                                //! transfer matrix
//    bool mbTransferAble;
    double mTransferR[2*2], mTransferS[2*1];

    double mTransferRInv[2*2];  //! deduced by mT

    H2ZeroCorner mZeroCorner;
    H2ZeroMovement mZeroMovement;

    int mToothType, mToothGear;
    int mPDirIndex;     //! 0: cw, 1:ccw
    int mMotionIndex;   //! 0: rot, 1: line
};

#endif
