#ifndef VROBOT_H
#define VROBOT_H

#include <QtCore>
#include <QImage>
#include <QSemaphore>
#include "../../include/mccfg.h"
#include "../sys/sysapi.h"
#include "../../source/para/syspara.h"

#include "vdevice.h"
#include "roboworker.h"
#include "robostate.h"

#include "../bus/receivecache.h"

#include "../model/tpvgroup.h"
#include "../model/sinanjumotiongroup.h"

#include "../arith/pathplan/pathplan.h" //! joints trace
#include "../arith/sinanju_split/sinanju_split.h"

//! \todo setup

namespace MegaDevice{
//! manager
class InstMgr;

//! devices
class deviceMRQ;

}

class RoboTask;
class RoboService;      //! msg service

#define robot_is_mrq( id )          ( ( (id) >= VRobot::robot_mrq ) && ( (id) < VRobot::robot_mrq_max ) )
#define robot_is_mrv( id )          ( ( (id) >= VRobot::robot_mrv ) && ( (id) < VRobot::robot_mrv_max ) )
#define robot_is_robot( id )        ( ( (id) >= VRobot::robot_complex ) && ( (id) < VRobot::robot_user ) )

#define scale_t( t )    ( (t)*(VRobot::_mTBase) )
#define scale_p( p )    ( (p)*(VRobot::_mPBase) )
#define scale_v( v )    ( (v)*(VRobot::_mVBase) )

enum eRoboPlanMode
{
    plan_linear = 0,
    plan_3rd,
    plan_5rd,
};

enum eRoboAngle
{
    robo_angle_none,
    robo_angle_abs,
    robo_angle_inc,
};

enum eRoboCoord
{
    robo_coord_body,
    robo_coord_tool,
    robo_coord_user,
};

struct PlanAttr
{
    eRoboPlanMode mMode;
    float mStep;

    PlanAttr( eRoboPlanMode mode = plan_linear,
              float step = 5.0f )
    {
        mMode = mode;
        mStep = step;
    }
};

class VRobot : public MegaDevice::VDevice, public ISerial
{
public:
    enum robotEnum
    {
        robot_unk = 0,

        robot_raw = 256,
        robot_mrq = 256,
        robot_qubeley_d = 256,
        robot_qubeley_s,
        robot_geogoog,
        robot_geogoog_8,
        robot_geogoog_10,
        robot_geogoog_5_1,

        robot_mrq_max,

        robot_mrv = 320,
        robot_gouf,

        robot_mrv_max,

        robot_complex = 1024,
        robot_sinanju = 1024,
        robot_delta,
        robot_megatron,
        robot_h2,
        robot_h2_m,
        robot_h2z,

        robot_ip,

        robot_motor,
        robot_motors,
        robot_slide,

        robot_igus_delta,

        robot_user = 65536,
    };

    enum EMotorDriverType
    {
        motor_driver_262,
        motor_driver_820,
    };

public:
    typedef int (VRobot::*apiTaskRequest)( void *pArg );

protected:
    static double _mTBase, _mPBase, _mVBase;      //! tune info
    static QString _tempPath;

    static SysPara *_mSysPara;

public:
    static void setTpvBase( double tBase,
                    double pBase,
                    double vBase );
    static void setTempPath( const QString &tempPath );
    static QString tempPath();

    static void attachSysPara( SysPara *pSysPara );
    static bool motionPredict( float t, float angle );

public:
    VRobot();
    virtual ~VRobot();
protected:
    void init();

public:
    virtual void postCtor();    //! post ctor after the sub ctor completed
    virtual void setReset();
protected:
    void gcWorker();

public:
    virtual int tryLoad();

    //! serial
    virtual int load( const QString &name );
    virtual int save( const QString &name );

    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );

    virtual void postload();

    virtual int uploadSetting();
    virtual int applySetting();

    virtual quint32 getSignature();

    QString getSN();
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
    virtual bool checkLink( int p1=0, int p2=0 );

protected:
    virtual void event_motionStatus( frameData &data );
    virtual void event_motionStatus( frameData &data, VRobot *pRobot );

public:
    virtual int build( MegaTableModel *pModel,

                       xxxGroup< tracePoint > &tracePlan,
                       xxxGroup< jointsTrace > &jointsPlan,
                       QList< tpvGroup *> &gp,
                       QList< int > &sectionList );     //! section
                                                        //! from,len,from,len
    //! action
    virtual int transform( int axes = 0 );

    //! for robot -- axes is joint
    virtual int download( tpvGroup *pGroup, const tpvRegion &region );
    virtual int download( QList<tpvGroup*> &groups,
                          QList<int> &joints,
                          const tpvRegion &region );

    virtual int download( VRobot *pSetup );

    virtual int run( const tpvRegion &region=0  );
    virtual int stop( const tpvRegion &region=0  );
    virtual int call( const tpvRegion &region=0  );

    virtual int goFactory( const tpvRegion &region=0 );

    virtual int goZero( const tpvRegion &region=0 );
    virtual int goZero( const tpvRegion &region,
                        int jointTabId );
    virtual int goZero( const tpvRegion &region,
                        int jointTabId, bool bCcw );
    virtual int goZero( const tpvRegion &region,
                        const QList<int> &jointList,
                        const QList<bool> &ccwList );


    virtual bool checkZeroValid();
    virtual float getZero( int jointTabId );
    virtual int  setZero( int jointTabId, float val );

    virtual int getMechanicalVersion();

    virtual int align( const tpvRegion &region );

    virtual int getPOSE( float pos[] ); //! mPOSITION
    virtual float toDeltaAngle( int jointId, float angle );

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
public:
    //! prop
    QString& getClass();
    QString& getDetail();

    robotEnum getId();
    robotEnum robotId();

    virtual QList<int> deviceIds();     //! receive/send/group
    virtual QList<int> subIDs( int ch );
    virtual QString deviceFullDesc();

    virtual void setPower( bool b );
    bool power();

    void setCoord( eRoboCoord cord );
    eRoboCoord getCoord( );

    //! configs
    int setAxes(int n);
    int axes();

    void setRegions( int page );
    int regions();

    void setDcAxes( int n );
    int dcAxes();

    void setOutputs( int n );
    int outputs();

    void setInputs( int n );
    int inputs();

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

    void setEncoderAble( bool );
    bool encoderAble();

    void setRunWaveAble( bool b );
    bool runWaveAble();

    void setTunningAble( bool b );
    bool tunningAble();

    void setCoordAble( bool b );
    bool coordAble();

    void setJointPanelAble( bool b );
    bool jointPanelAble();

    void setRoboPanelAble( bool b );
    bool roboPanelAble();

    void setMrpAble( bool b );
    bool mrpAble();

    bool axesPOSAble( int ax );

    void setDriverId( int id );
    int driverId();

    void setTemperatures( int n);
    int temperatures();

    void setUarts( int n );
    int uarts();

    void setUartSensors( int n );
    int uartSensors();

    QStringList uartSensorList();
    QStringList uartNameList();

    void setAbsEncAlarms( int n );
    int absEncAlarms();

    void setDistanceAlarms( int n);
    int distanceAlarms();

    void setAlarms( int n);
    int alarms();

    void setTrigSrcs( int srcs );
    int trigSrcs();

    virtual QString trigSrcAlias( int ax, int iTrig );

    QList<int> trigSrcIdList();

    virtual void setJointZeroCcw( int jId, bool b );
    QList<bool> jointZeroCcwList();

    void setJointZeroCcwVisible( int jId, bool b );
    QList<bool> jointZeroCcwVisibleList();

    void setPoseCount( int pos );
    int poseCount();

    QStringList & poseTitles();

    void setAngleType( eRoboAngle angMode );
    eRoboAngle angleType();

    bool interpAble();

    bool zeroAttrInComPref();

    void microStepAttr( QStringList &stepList, int &base );

    void setZeroPref( double spd, int tmo, int tick );
    void zeroPref( double &spd, int &tmo, int &tick );

    QString toZeroUnit();

    QImage & getImage();

    void setPlanAttr( const PlanAttr &attr );
    PlanAttr planAttr();

    RoboWorker *lpc( int ax );      //! local post call
    RoboWorker *lpc();              //! robo lpc
public:
    virtual QAbstractTableModel *handActions();

    //! MOTION_STAUS
    virtual void setStatus( int stat, const tpvRegion &region, frameData &data );

    void assignStatus( int stat, const tpvRegion &region );

    int status( const tpvRegion &region );
    virtual int state( const tpvRegion &region, int inTask = 0 );

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

    QList<int> jointPlanMode();
    QList<float> jointAcc();
    QList<float> jointDec();
public:
    void setAxesConnectionName( const QStringList &names );
    QStringList axesConnectionName();

    QStringList axesDeviceName();

protected:
    void setAxesDefName( int axesCnt ); //! for ui label view
    void setJointName( int axesCnt );

    //! assist
    VRobot * subRobot( int index, int *pAxes );

    int checkRoboTask();

protected:

    int mAxes, mRegions;                //! configs
    int mDCAxes;

    QList<bool> mPOSAble;

    int mDOs, mDIs, mISOs, mISIs, mAINs, mMosos;
    int mOutputs, mInputs;

    int mTemperatures, mUARTs, mUART_Sensors;
    int mAbsEncoderAlarms, mDistanceAlarms;
    int mAlarms;

    QStringList mSensorNameList;
    QStringList mUartNameList;

    int mTrigSrcs;                      //! 5,2
    QList<int> mTrigIdsList;

    int mPoseCount;                     //! 0,3..
    QStringList mPoseTitles;

    eRoboAngle mAngleType;

    QString mToZeroUnit;

    bool mbInterpAble;
    bool mbZeroAttrInComPref;
    bool mbEncoderAble;
    bool mbRunWaveAble;
    bool mbTunningAble;
    bool mbCoordAble;

    //! ui attri
    bool mbJointPanelAble, mbRoboPanelAble;
    bool mbMrpAble;

    int  mDriverId;

    QStringList mMicrostepsList;
    int mMicrostepBase;

    PlanAttr mPlanAttr;

public:
    QString mClass;                     //! robot class
    QString mDetail;
    QString mSN;
    robotEnum mId;
    QImage mImage;                      //! device image

    MegaDevice::InstMgr *m_pInstMgr;

    QMap<tpvRegion, int> mRobotStatus;  //! status

    bool mbPwr;
                                        //! groupId
    int mSubGroup;
    int mSubGroupId;
    double mZeroSpeed;
    int mZeroTmo, mZeroTick;

    eRoboCoord mCoord;
    QStringList mAxesConnectionName;    //! connected to device
    QStringList mJointName;             //! by config

    QList<bool> mJointAngleMask;        //! angles for each joint
    QList<bool> mJointDeltaAngleMask;

    QList<bool> mJointCcwMask;          //! ccw for each angle
    QList<bool> mJointZeroCcw;          //! zero ccw for each joint
    QList<bool> mJointAlignAble;

    QList<bool> mAngleDir;              //! true: +

    QList<bool> mAngleStickDir;
    QList<bool> mAngleStickAble;

    QList<bool> mLvtAble;

    QList <double> mRefAngles;          //! 'ref' angles for each joint by joint id
    QList <double> mRotateAngles;       //! rotate angle for the coordinate
    QList <double> mArchAngles;         //! arch angles

    QList <double> mInitAngles;         //! init angle
    QList <double> mSeperateAngles;
    QList <double> mInitPos;            //! sinanju:x,y,z

    QList <double> mArmLengths;         //! arm lengths from base:[0]
    double mBaseCompensation;
    double mLengthUnit;                 //! mm

    RoboWorker* m_pAxesWorkers;         //! worker
    RoboWorker* m_pRoboWoker;
                                        //! temp task
    RoboTask *m_pRoboTask;

    QSemaphore *m_pXEventSema;

    RoboService *m_pRoboMsgService;
    RoboService *m_pRoboProcService;
};

Q_DECLARE_METATYPE( VRobot )
Q_DECLARE_METATYPE( VRobot* )

class RoboTaskArgument
{
public:
    int mTmo;       //! us
    int mTick;      //! us

    tpvRegion mRegion;
public:
    RoboTaskArgument();
    virtual ~RoboTaskArgument();
};

//! request
class RoboTaskRequest
{
public:
    VRobot *m_pRobo;
    VRobot::apiTaskRequest m_pApi;
    RoboTaskArgument *m_pArg;       //! need to be deleted

public:
    RoboTaskRequest();
    virtual ~RoboTaskRequest();

public:
    void request( VRobot *pRobo,
                  VRobot::apiTaskRequest api,
                  RoboTaskArgument *pArg );
};

class RoboTask : public QThread
{
    Q_OBJECT

protected:
    static QList<RoboTask*> _roboTasks;
public:
    static void killAll();

public:
    RoboTask( QObject *pObj = NULL );
    ~RoboTask();

protected:
    virtual void run();

public:
    void setRequest( RoboTaskRequest * pReq );

protected:
    int checkRequest( const RoboTaskRequest *pReq );
    void gc();


protected:
    RoboTaskRequest *m_pReq;
};

class RoboService : public QThread
{
    Q_OBJECT

public:
    RoboService( QObject *pObj = nullptr );

protected:
    virtual void run();
};

#endif // VROBOT_H
