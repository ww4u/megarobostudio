
#ifndef _ROBOT_MOTORS_H_
#define _ROBOT_MOTORS_H_

#include "../rawrobo/rawrobo.h"
#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

#include "../../model/relationmodel.h"

struct MotorsArg : public RoboTaskArgument
{
    int mAx;                        //! 0 -- x
    float mZeroDist,mZeroTime, mZeroEndV;
    float mZeroGapDist, mZeroGapTime;

    MotorsArg();
};

class MotorsTask : public RoboTask
{
    Q_OBJECT
public:
    MotorsTask( QObject *pParent = NULL );

protected:
    virtual void run();
};

class robotMotors : public RawRobo
{
public:
    robotMotors();

public:
    DECLARE_SCPI()

public:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );

    QAbstractTableModel *getModel();
protected:
    int serialOutJoints( QXmlStreamWriter &writer);
    int serialInJoints( QXmlStreamReader &reader );

    int serialOutSubJoint( QXmlStreamWriter &writer, Relation *pRel );
    int serialInSubJoint( QXmlStreamReader &reader, Relation *pRel );

public:
//    int program( QList<D1Point> &curve,
//                 const tpvRegion &region );
//    int move( QList<D1Point> &curve,
//              const tpvRegion &region );

//    int preMove( QList<D1Point> &curve,
//              const tpvRegion &region );

//    int move( float dd,
//              float dt,
//              float endV,
//              const tpvRegion &region );

public:
//    virtual int goZero( const tpvRegion &region,
//                        int jointId, bool bCcw );
//    virtual int goZero( const tpvRegion &region=0 );
//    virtual int goZero( const tpvRegion &region,
//                        const QList<int> &jointList,
//                        const QList<bool> &ccwList );

//    int zeroAxesTask( void *pArg );

//    void setZeroAttr( double zeroTime,
//                      double zeroDist,
//                      int posIndex,
//                      bool bPosEn );
//    void zeroAttr( double &zeroTime,
//                   double &zeroDist,
//                   int &posIndex,
//                   bool &bPosEn );

//    void setGap( double time,
//                 double dist,
//                 bool bPosEn );
//    void gap( double &time,
//              double &dist,
//              bool &bPosEn );

protected:
//    int buildTrace( QList<D1Point> &curve );
//    int downloadTrace( const tpvRegion &region );

protected:
//    double mZeroTime, mZeroDistance;
//    double mGapTime, mGapDistance;

//    bool mbZeroPosEn, mbGapPosEn;

//    int mPosIndex;

    RelationModel mModel;

};

#endif
