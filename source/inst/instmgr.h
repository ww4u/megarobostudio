#ifndef _DEV_MGR_H_
#define _DEV_MGR_H_

#include <QtCore>



#include "../bus/canbus.h"
#include "../bus/receivecache.h"

#include "../instServer/instserver.h"

//! many robots
#include "../../robot/robotfact.h"

#include "./vrobolist.h"

class mcModel;

//typedef QList< VRobot *> VRoboList;
typedef QList< VRoboList * > DeviceTree;

namespace MegaDevice 
{

class InstMgr : public instServer
{
    Q_OBJECT
protected:
    static InstMgr *_mpMgr;
protected:
    InstMgr( QObject *parent = NULL );
public:
    ~InstMgr();

public:
    static InstMgr *proxy();
    static void free();

public:
    //! to some device
    virtual void dataIn( QTcpSocket *socket,
                           const QString &name,
                           QByteArray &ary );

public:
    void setMainModel( mcModel *pModel );

    int probeBus();
    int probeCanBus();

    int emergencyStop();
    int hardReset();

    //! device tree
    DeviceTree getDeviceTree();

    void appendFileDeviceTree( DeviceTree &devTree );
    void clearFileDeviceTree();

    //! robo list
    DeviceTree roboTree( const DeviceTree &devTree );
    DeviceTree roboTree( );
    QStringList roboResources();

    deviceMRQ *getDevice( int id );

    receiveCache *getInterruptSource();

    //! find robot
    VRobot * findRobot( const QString &name, int axesId );

    VRobot * findRobot( const QString &name, const QString &bus );
    //! name@bus
    VRobot * findRobot( const QString &fullname );

    VRoboList *findBus( const QString &busName );

    VRobot * findRobotBySendId( int sendId, int axesId = 0 );
    VRobot * findRobotByRecvId( int recvId, int axesId = 0 );

    deviceMRQ *findDevice( const QString &name, int axesId=0 );
    deviceMRQ *findDevice( const QString &name, int *pAx );     //! chx@name

    QString sendIdToName( int sendId );

    QStringList getResources();
    QStringList getChans();     //! chx@devicename

    void setTPVBase( float t=1.0f, float p=1.0f, float v=1.0f );

protected:
    void preProbeBus();
    void postProbeBus();

    void gc();

    void gcPhyBus();
    void gcFileBus();

    scpiShell *findShell( const QString &name );

public:
    mcModel *m_pMainModel;

    //! can bus
    CANBus mCanBus;

    //! \todo other bus

    VRoboList mDevices;

    //! real bus
    DeviceTree mDeviceTree;
    DeviceTree mFileDeviceTree;

    QList<IBus *> mFileBusList;

    receiveCache *m_pReceiveCache;
};
	
}

#endif
