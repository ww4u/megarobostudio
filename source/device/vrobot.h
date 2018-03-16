#ifndef VROBOT_H
#define VROBOT_H

#include <QtCore>
#include <QImage>

#include "../../../include/mccfg.h"
#include "../../sys/sysapi.h"

#include "vdevice.h"
#include "roboworker.h"
#include "robostate.h"

#include "../../bus/receivecache.h"

#include "../../model/tpvgroup.h"
#include "../../model/motiongroup.h"


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

        robot_complex = 1024,
        robot_motor = 1024,
        robot_slide1,
        robot_slide2,
        robot_delta,
        robot_megatron,

        robot_user = 65536,
    };

public:
    VRobot();
    virtual ~VRobot();
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
    virtual int download( tpvGroup *pGroup, int axes = 0 );
    virtual int download( motionGroup *pGroup, int axes = 0 );

    virtual int download( QList<tpvGroup*> &groups,
                          QList<int> &joints );
//    virtual int download(  QList< double *> &tList, QList<int> &skipTList,
//                           QList< double *> &pList, QList<int> &skipPList,
//                           QList< double *> &vList, QList<int> &skipVList,
//                           int count );

    virtual int download( VRobot *pSetup );

    virtual int run( int axes  );
    virtual int stop( int axes  );

    virtual int run( );
    virtual int stop( );

    virtual int setLoop( int n );
    virtual int loopNow();

    virtual void startTimer( void *pContext, int id=0, int tmous=1000 );
    virtual void killTimer( void *pContext, int id=0 );
    virtual void onTimer( void *pContext, int id );

    //! condition
    //! sub ax
    virtual void attachCondition( int subAxes,
                                  MegaDevice::RoboCondition *pCond );
    virtual bool waitCondition( int subAxes,
                                MegaDevice::RoboCondition *pCond,
                                int tmoms=-1 );

    //! entity
    virtual void attachCondition(
                                  MegaDevice::RoboCondition *pCond );
    virtual bool waitCondition(
                                MegaDevice::RoboCondition *pCond,
                                int tmoms=-1 );

public:
    //! prop
    QString& getClass();
    robotEnum getId();
    robotEnum robotId();

    int setAxes(int n);
    int getAxes();
    int axes();

    QImage & getImage();

    RoboWorker *lpc( int ax );      //! local post call
    RoboWorker *lpc();              //! robo lpc
public:
    virtual QAbstractTableModel *handActions();

    //! MOTION_STAUS
    virtual void setStatus( int stat, int ch = 0 );
    int getStatus( int ch = 0 );
    int status( int ch = 0 );

    void setInstMgr( MegaDevice::InstMgr *pMgr );
    MegaDevice::InstMgr *getInstMgr();
    MegaDevice::InstMgr *instMgr();

    void setSubGroup( int sub );
    int getSubGroup();
    int subGroup();

    virtual void setSubGroupId( int id );
    int getSubGroupId();
    int subGroupId();

    void setTPVUnit( float t=1.0f, float p=1.0f, float v=1.0f );

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

private:
    int mAxes;
public:
    QString mClass;                     //! robot class
    robotEnum mId;
    QImage mImage;                      //! device image

    MegaDevice::InstMgr *m_pInstMgr;

    double mTBase, mPBase, mVBase;      //! tune info

    QMap<int, int> mRobotStatus;        //! status

                                        //! groupId
    int mSubGroup;
    int mSubGroupId;

    QStringList mAxesConnectionName;    //! connected to device
    QStringList mJointName;             //! by config

    QList <double> mRefAngles;          //! ref angles for each joint by joint id
    QList <double> mRotateAngles;       //! rotate angle for the coordinate
    QList <double> mArmLengths;         //! arm lengths from base:[0]

    double mBaseCompensation;
    double mLengthUnit;                 //! mm

    RoboWorker* m_pAxesWorkers;         //! worker
    RoboWorker* m_pRoboWoker;
};

Q_DECLARE_METATYPE( VRobot )
Q_DECLARE_METATYPE( VRobot * )

#endif // VROBOT_H
