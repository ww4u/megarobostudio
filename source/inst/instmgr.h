#ifndef _DEV_MGR_H_
#define _DEV_MGR_H_

#include <QtCore>



#include "../bus/canbus.h"
#include "../bus/rs232bus.h"
#include "../bus/receivecache.h"

#include "../instServer/instserver.h"

//! many robots
#include "../../robot/robotfact.h"

#include "./vrobolist.h"

class mcModel;

typedef QList< VRoboList * > DeviceTree;

namespace MegaDevice 
{

class INTRThread : public QThread
{
    Q_OBJECT

public:
    INTRThread( QObject *parent = 0 );
    virtual ~INTRThread( );

Q_SIGNALS:
    void sig_event( eventId id, frameData );
protected Q_SLOTS:
    void slot_event( eventId id, frameData dat );
};

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
    void setMainShell( scpiShell *pShell );

    int probeBus();
    int probeCanBus();
    int probeRs232Bus();

    int emergencyStop();
    int hardReset();
    int requestStates();

    //! device tree
    DeviceTree getDeviceTree();

    void appendFileDeviceTree( DeviceTree &devTree );
    void clearFileDeviceTree();

    //! robo list
    DeviceTree roboTree( const DeviceTree &devTree );
    DeviceTree roboTree( );
    QStringList roboResources();

    deviceMRQ *getDevice( int id );

    INTRThread *getInterruptSource();

    //! find robot
    VRobot * findRobot( const QString &name, int axesId );
    VRobot * findRobot( const QString &name, int *pAx );    //! chx@name

    VRobot * findRobot( const QString &name, const QString &bus );
    //! name@bus
    VRobot * findRobot( const QString &fullname );

    //! only name
    VRobot * findAbbRobot( const QString &abbName );

    VRoboList *findBus( const QString &busName );

    VRobot * findRobotBySendId( int sendId, int devId, int axesId = 0 );
    VRobot * findRobotByRecvId( int recvId, int devId, int axesId = 0 );

    MegaDevice::deviceMRQ  *findDevice( const QString &name, int axesId=0 );
    MegaDevice::deviceMRQ  *findDevice( const QString &name, int *pAx );     //! chx@name

    QString sendIdToName( int devId, int sendId );

    QStringList getResources();
    QMap<QString,int>  getDeviceMap();
    QStringList getChans();     //! chx@devicename

    QStringList robots();

    QStringList resources();

    void setTPVBase( float t=1.0f, float p=1.0f, float v=1.0f );

    int openBus();
    int closeBus();

protected:
    void preProbeBus();
    void postProbeBus();

    int probeCANBus( IBus *pBus,
                     int id,
                     int seqId,
                     const QString &devRsrc,
                     VRoboList &roboList );

    void gc();

    void gcPhyBus();
    void gcFileBus();
public:
    scpiShell *findShell( const QString &name );

    bool matchSignature( quint32 sig, QString &alias );

public:
    mcModel *m_pMainModel;
    scpiShell *m_pMainShell;

                                //! can buses
//    QList< CANBus *> mCanBuses;
    QList< IBus *> mBuses;

    //! \todo other bus

    VRoboList mDevices;

    //! real bus
    DeviceTree mDeviceTree;
    DeviceTree mFileDeviceTree;

    QList<IBus *> mFileBusList;

    INTRThread *m_pINTR;
    QMap<quint32, QString> mDeviceMap;
};
	
}

#endif
