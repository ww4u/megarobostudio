#ifndef VROBOT_H
#define VROBOT_H

#include <QtCore>
#include <QImage>

#include "../../include/mccfg.h"
#include "../sys/sysapi.h"

#include "vdevice.h"
#include "roboworker.h"
#include "robostate.h"

#include "../bus/receivecache.h"

#include "../model/tpvgroup.h"
#include "../model/motiongroup.h"


//! \todo setup

namespace MegaDevice{
//! manager
class InstMgr;

//! devices
class deviceMRQ;

}

#define robot_is_mrq( id )          ( ( (id) >= VRobot::robot_raw ) && ( (id) < VRobot::robot_complex ) )
#define robot_is_robot( id )        ( ( (id) >= VRobot::robot_complex ) && ( (id) < VRobot::robot_user ) )

class VRobot : public MegaDevice::VDevice, public ISerial
{
public:
    enum robotEnum
    {
        robot_unk = 0,

        robot_raw = 256,
        robot_qubeley = 256,
        robot_geogoog,
        robot_geogoog_8,
        robot_geogoog_10,
        robot_geogoog_5_1,

        robot_complex = 1024,
        robot_motor = 1024,
        robot_slide,
        robot_delta,
        robot_megatron,
        robot_sinanju,
        robot_h2,

        robot_user = 65536,
    };

protected:
    static double _mTBase, _mPBase, _mVBase;      //! tune info
public:
    static void setTpvBase( double tBase,
                    double pBase,
                    double vBase );

public:
    VRobot();
    virtual ~VRobot();

    virtual void postCtor();    //! post ctor after the sub ctor completed

protected:
    void gcWorker();

public:
    //! serial
    virtual int load( const QString &name );
    virtual int save( const QString &name );

    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );

public:
    //! msg
    virtual void onMsg(  int subAxes, RoboMsg &msg );

    virtual void interruptHandler( eventId eId,
                                   frameData &data );

    virtual void interruptHandler( eventId eId,
                                   frameData &data,
                                   VRobot *pRobot
                                   );

    virtual void onLine();
    virtual void offLine();

protected:
    virtual void event_motionStatus( frameData &data );
    virtual void event_motionStatus( frameData &data, VRobot *pRobot );

public:
    //! action
    virtual int transform( int axes = 0 );

    //! for robot -- axes is joint
    virtual int download( tpvGroup *pGroup, const tpvRegion &region );
    virtual int download( motionGroup *pGroup, const tpvRegion &region );

    virtual int download( QList<tpvGroup*> &groups,
                          QList<int> &joints,
                          const tpvRegion &region );

    virtual int download( VRobot *pSetup );

    virtual int run( const tpvRegion &region=0  );
    virtual int stop( const tpvRegion &region=0  );
    virtual int goZero( );
    virtual int goZero( int jointTabId );
    virtual int goZero( int jointTabId, bool bCcw );
    virtual int goZero( const QList<int> &jointList,
                        const QList<bool> &ccwList );

    virtual int setLoop( int n, const tpvRegion &region=0 );
    virtual int loopNow();

    virtual void startTimer( void *pContext, int id=0, int tmous=1000 );
    virtual void killTimer( void *pContext, int id=0 );
    virtual void onTimer( void *pContext, int id );

    //! condition
    virtual void attachCondition( const tpvRegion &region,
                                  MegaDevice::RoboCondition *pCond );
    virtual bool waitCondition( const tpvRegion &region,
                                MegaDevice::RoboCondition *pCond,
                                int tmoms=-1 );

    //! entity
//    virtual void attachCondition(
//                                  MegaDevice::RoboCondition *pCond );
//    virtual bool waitCondition(
//                                MegaDevice::RoboCondition *pCond,
//                                int tmoms=-1 );

public:
    //! prop
    QString& getClass();
    QString& getDetail();

    robotEnum getId();
    robotEnum robotId();


    //! configs
    int setAxes(int n);
    int axes();

    void setRegions( int page );
    int regions();

    void setDcAxes( int n );
    int dcAxes();

    void setDOs( int n );
    int dos();

    void setDIs( int n );
    int dis();

    void setIsos( int n );
    int isos();

    void setIsis( int n );
    int isis();

    void setAins( int n );
    int ains();

    void setMosos( int n );
    int mosos();

    void setEncoders( int n );
    int encoders();

    void setTemperatures( int n);
    int temperatures();

    void setUarts( int n );
    int uarts();

    void setUartSensors( int n );
    int uartSensors();

    void setZeroSpeed( double spd );
    double zeroSpeed();

    QImage & getImage();

    RoboWorker *lpc( int ax );      //! local post call
    RoboWorker *lpc();              //! robo lpc
public:
    virtual QAbstractTableModel *handActions();

    //! MOTION_STAUS
    virtual void setStatus( int stat, const tpvRegion &region );
    int status( const tpvRegion &region );

    void setInstMgr( MegaDevice::InstMgr *pMgr );
    MegaDevice::InstMgr *getInstMgr();
    MegaDevice::InstMgr *instMgr();

    void setSubGroup( int sub );
    int getSubGroup();
    int subGroup();

    virtual void setSubGroupId( int id );
    int getSubGroupId();
    int subGroupId();

    //! name@CH1
    MegaDevice::deviceMRQ *findDevice( const QString &name,
                                       int *pAxes );

    MegaDevice::deviceMRQ *jointDevice( int jointId,
                                        int *pAxes );

public:
    void setAxesConnectionName( const QStringList &names );
    QStringList axesConnectionName();

    QStringList axesDeviceName();

protected:
    void setAxesDefName( int axesCnt ); //! for ui label view
    void setJointName( int axesCnt );

    //! assist
    VRobot * subRobot( int index, int *pAxes );

protected:
    int mAxes, mRegions;                //! configs
    int mDCAxes;

    int mDOs, mDIs, mISOs, mISIs, mAINs, mMosos;
    int mEncoders, mTemperatures, mUARTs, mUART_Sensors;

public:
    QString mClass;                     //! robot class
    QString mDetail;
    robotEnum mId;
    QImage mImage;                      //! device image

    MegaDevice::InstMgr *m_pInstMgr;

    QMap<tpvRegion, int> mRobotStatus;  //! status

                                        //! groupId
    int mSubGroup;
    int mSubGroupId;
    double mZeroSpeed;

    QStringList mAxesConnectionName;    //! connected to device
    QStringList mJointName;             //! by config

    QList<bool> mJointAngleMask;        //! angles for each joint

    QList<bool> mAngleDir;              //! true: +

    QList <double> mRefAngles;          //! 'ref' angles for each joint by joint id
    QList <double> mRotateAngles;       //! rotate angle for the coordinate
    QList <double> mArchAngles;         //! arch angles

    QList <double> mInitAngles;         //! init angle
    QList <double> mInitPos;            //! sinanju:x,y,z


    QList <double> mArmLengths;         //! arm lengths from base:[0]
    double mBaseCompensation;
    double mLengthUnit;                 //! mm

    RoboWorker* m_pAxesWorkers;         //! worker
    RoboWorker* m_pRoboWoker;
};

Q_DECLARE_METATYPE( VRobot )
Q_DECLARE_METATYPE( VRobot * )

#endif // VROBOT_H
