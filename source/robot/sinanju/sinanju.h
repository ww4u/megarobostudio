
#ifndef _ROBOT_SINANJU_H_
#define _ROBOT_SINANJU_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../device/_scpi_xxx_device.h"

//! arith
#include "../../arith/pathplan/pathplan.h"
#include "../../arith/sinanju_split/sinanju_split.h"

#include "../../model/sinanjumotiongroup.h"

class roboSinanjuTaskArgument : public RoboTaskArgument
{
public:
    QList<double> mAimAngles;
    QList<double> mSepAngles;

public:
    roboSinanjuTaskArgument()
    {}
};

class robotSinanju : public RawRobo
{
public:
    robotSinanju();
    ~robotSinanju();
public:
    DECLARE_SCPI()

public:
    virtual void onMsg( int subAxes, RoboMsg &msg );

public:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );

    virtual int download( QList<tpvGroup*> &groups,
                          QList<int> &joints,       //! joint tab id
                          const tpvRegion &region );

    virtual int download( VRobot *pSetup );

//    virtual int run( const tpvRegion &region=0  );  //! just run
//    virtual int stop( const tpvRegion &region=0 );  //! stop

    virtual int goFactory( const tpvRegion &region=0 );

    virtual int goZero( const tpvRegion &region=0 );
    virtual int goZero( const tpvRegion &region,
                        int jointId, bool bCcw );
    virtual int goZero( const tpvRegion &region,
                        const QList<int> &jointList,
                        const QList<bool> &ccwList );

    virtual bool checkZeroValid();
    virtual float getZero( int jointTabId );
    virtual int  setZero( int jointTabId, float zero );

    virtual int align( const tpvRegion &region );

    virtual int getPOSE( float pos[] );
    virtual float toDeltaAngle(int jointId, float angle);

protected:
    float deduceAngle( float fNow, float fAim, float fSep, bool dir );

public:
    int zeroAxesTask( void *pArg );

protected:
    int toAimSession( const tpvRegion &region,
                      const QList<double> &aimAngles,
                      const QList<double> &sepAngles );

    int goX( const tpvRegion &region,
             const QList<double> &aimAngles,
             const QList<double> &sepAngles,
             float handT, float handP, float handV );
    int toAim( const tpvRegion &region,
               const QList<double> &aimAngles,
               const QList<double> &sepAngles );
    int toAimd( const tpvRegion &region,
                const QList<double> &aimAngles,
                const QList<double> &sepAngles );

public:
    virtual int loopNow();

    virtual void toState(int stat);

public:
    virtual int build( MegaTableModel *pModel,
                       xxxGroup<tracePoint> &tracePlan,
                       xxxGroup<jointsTrace> &jointsPlan,
                       QList< tpvGroup *> &tpvGroups,
                       QList< int> &sectionList );

public:
    int call( int n, const tpvRegion &region=0 );  //! load + run

    int program( const QString &fileName,
                 const tpvRegion &region );
    int loadProgram( const QString &fileName );

    int program( QList<TraceKeyPoint> &curve,
                 const tpvRegion &region );


    int move( QList<TraceKeyPoint> &curve,
              const tpvRegion &region );

    int moveTest1( const tpvRegion &region=0 );
    int moveTest2( const tpvRegion &region=0 );

    int nowPose( TraceKeyPoint &pos );
    int nowAngle( float angles[] );
    //! ref to the joint angle
    int nowJointAngle( float angles[4] );
    int jointMove( int jointId, int subPage, float angle, float t );

    int angleToPos( float angles[4],
                     TraceKeyPoint &pos );
    void diffAngle( float angles[4],
                    float anglesOut[4] );
    void diffAngle( float angles[4],
                    double anglesOut[4] );

    int nowDist( QList<float> &dists );

protected:
    int buildTrace( QList<TraceKeyPoint> &curve,
                    xxxGroup<tracePoint> &tracePlan,
                    xxxGroup<jointsTrace> &jointsPlan
                     );

    int verifyTrace( QList<TraceKeyPoint> &curve );

    void interpTune( QList<TraceKeyPoint> &curve );

    //! rotate
    void coordRotate( QList<TraceKeyPoint> &curve );
    void coordIRotate( TraceKeyPoint &pt );

    //! ass
    void coordRotate( TraceKeyPoint &pt, double rot[3*3], double shift[3*1] );
    void coordIRotate( TraceKeyPoint &pt, double rot[3*3], double shift[3*1] );

    //! body

    //! user
    //! body->user
    void toUser( TraceKeyPoint &pt );
    //! user->body
    void fromUser( TraceKeyPoint &pt );

    //! tcp
    //! body->tcp
    void toTcp( TraceKeyPoint &pt );
    //! tcp->body
    void fromTcp( TraceKeyPoint &pt );

    int planTrace( QList<TraceKeyPoint> &curve,
                   xxxGroup<tracePoint> &tracePoints );

    int splitTrace( xxxGroup<tracePoint> &tracePoints,
                    xxxGroup<jointsTrace> &traceJoints );

    int convertTrace(   QList<TraceKeyPoint> &curve,
                        xxxGroup<jointsTrace> &jointsPlan,
                        QList< tpvGroup *> &groups,
                        QList< int> &sectionList );

    int downloadTrace( const tpvRegion &region,
                       QList< tpvGroup *> &groups );
    int buildTpvGroup( xxxGroup<jointsTrace> &jointsPlan,
                       QList< tpvGroup *> &gp );

protected:
    int serialInAngle( QXmlStreamReader &reader );
    int serialOutAngle( QXmlStreamWriter &writer );

    int serialInHandZero( QXmlStreamReader &reader );
    int serialOutHandZero( QXmlStreamWriter &writer );

    int serialInArm( QXmlStreamReader &reader );
    int serialOutArm( QXmlStreamWriter &writer );

    int serialInInitPos( QXmlStreamReader &reader );
    int serialOutInitPos( QXmlStreamWriter &writer );

    int serialInJointCcw( QXmlStreamReader &reader );
    int serialOutJointCcw( QXmlStreamWriter &writer );

    int serialInJointFactory( QXmlStreamReader &reader );
    int serialOutJointFactory( QXmlStreamWriter &writer );

    int serialInTransfer( QXmlStreamReader &reader );
    int serialOutTransfer( QXmlStreamWriter &writer );

    int serialInTransferR( QXmlStreamReader &reader );
    int serialInTransferRInv( QXmlStreamReader &reader );
    int serialInTransferS( QXmlStreamReader &reader );

    int serialInTcp( QXmlStreamReader &reader );
    int serialOutTcp(QXmlStreamWriter &writer );

protected:
    void exportPlan( const QString &fileName, xxxGroup<tracePoint> &tracePlan );
    void exportJoints( const QString &fileName, xxxGroup<jointsTrace> &jointsPlan );

public:
    void setHandZeroAttr( double zeroTime, double zeroAngle );
    void handZeroAttr( double &zeroTime, double &zeroAngle );

    void setGapAttr( double gapTime, double gapAngle );
    void gapAttr( double &gapTime, double &gapAngle );

    void setHandAble( bool b );
    bool handAble();

    void setJointFactoryAngle( int id, double angle );
    double jointFactoryAngle( int id );

    void setTransfer(
                      double rot[3*3],
                      double shift[3*1],
                      double rotInv[3*3] );
    void transfer(
                   double rot[3*3],
                   double shift[3*1],
                   double rotInv[3*3] );

    void setTcp( double p[3], double r[3] );
    void getTcp( double p[3], double r[3] );

protected:
    double mHandZeroTime, mHandZeroAngle;
    double mGapTime, mGapAngle;

    bool mbHandAble;
    QList< double > mJointFactoryList;
    QList< double > mJointFactorySeperateList;

//    bool mbTransferable;
    double mTransferR[3*3], mTransferRInv[3*3];
    double mTransferS[3*1];

    double mTcpP[3], mTcpR[3];
};

#endif
