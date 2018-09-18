#ifndef _MRQ_MODEL_H_
#define _MRQ_MODEL_H_
#include "../../../include/mcstd.h"
#include "../phydevice.h"

#include "../../intf/iserial.h"

#include "_MRQ_model.h"

namespace MegaDevice
{

struct PwmInfo
{
    byte mDuty;
    quint32 mFreq;
};

class MRQ_model : public PhyDevice, public _MRQ_model
{
public:
    MRQ_model();

public:
//    virtual int save( const QString &str );
//    virtual int load( const QString &str );

//    virtual int _serialOut( QXmlStreamWriter &writer );
    virtual int serialOut( QXmlStreamWriter &writer );
    virtual int serialIn( QXmlStreamReader &reader );

protected:
    int saveLink( QXmlStreamWriter &writer );
    int loadLink( QXmlStreamReader &reader );

    int saveSystem( QXmlStreamWriter &writer );
    int loadSystem( QXmlStreamReader &reader );

    int saveRs232( QXmlStreamWriter &writer );
    int loadRs232( QXmlStreamReader &reader );

    int saveCan( QXmlStreamWriter &writer );
    int loadCan( QXmlStreamReader &reader );

    int saveClock( QXmlStreamWriter &writer );
    int loadClock( QXmlStreamReader &reader );

    int saveMotion( QXmlStreamWriter &writer );
    int loadMotion( QXmlStreamReader &reader );

    int saveSubMotion( int ax, QXmlStreamWriter &writer );
    int loadSubMotion( int ax, QXmlStreamReader &reader );

    int saveIdentity( QXmlStreamWriter &writer );
    int loadIdentity( QXmlStreamReader &reader );

    int saveSubIdentity( int sub, QXmlStreamWriter &writer );
    int loadSubIdentity( int sub, QXmlStreamReader &reader );

    int saveMotor( QXmlStreamWriter &writer );
    int loadMotor( QXmlStreamReader &reader );

    int saveSubMotor( int id, QXmlStreamWriter &writer );
    int loadSubMotor( int id, QXmlStreamReader &reader );

    int saveEncoder( QXmlStreamWriter &writer );
    int loadEncoder( QXmlStreamReader &reader );

    int saveSubEncoder( int sub, QXmlStreamWriter &writer );
    int loadSubEncoder( int sub, QXmlStreamReader &reader );

    int saveMotionPlan( QXmlStreamWriter &writer );
    int loadMotionPlan( QXmlStreamReader &reader );

    int saveMotionSlew( int ax, QXmlStreamWriter &writer );
    int loadMotionSlew( int ax, QXmlStreamReader &reader );

    int saveSubMotionPlan( int ax, QXmlStreamWriter &writer );
    int loadSubMotionPlan( int ax, QXmlStreamReader &reader );

    int saveSubMotionPlanPage( int ax, int page, QXmlStreamWriter &writer );
    int loadSubMotionPlanPage( int ax, int page, QXmlStreamReader &reader );

    int saveReport( QXmlStreamWriter &writer );
    int loadReport( QXmlStreamReader &reader );

    int saveSubReport( int ax, QXmlStreamWriter &writer );
    int loadSubReport( int ax, QXmlStreamReader &reader );

    int saveTrigger( QXmlStreamWriter &writer );
    int loadTrigger( QXmlStreamReader &reader );

    int saveSubTrigger( int ax, QXmlStreamWriter &writer );
    int loadSubTrigger( int ax, QXmlStreamReader &reader );

    int saveSubTriggerChannel( int ax, int ch, QXmlStreamWriter &writer );
    int loadSubTriggerChannel( int ax, int ch, QXmlStreamReader &reader );

    int saveDriver( QXmlStreamWriter &writer );
    int loadDriver( QXmlStreamReader &reader );

    int saveSubDriver( int sub, QXmlStreamWriter &writer );
    int loadSubDriver( int sub, QXmlStreamReader &reader );

    int saveSubDriver820( int sub, QXmlStreamWriter &writer );
    int loadSubDriver820( int sub, QXmlStreamReader &reader );

    int saveDo( QXmlStreamWriter &writer );
    int loadDo( QXmlStreamReader &reader );

    int saveSubDo( int sub, QXmlStreamWriter &writer );
    int loadSubDo( int sub, QXmlStreamReader &reader );

    int saveISOo( QXmlStreamWriter &writer );
    int loadISOo( QXmlStreamReader &reader );

    int saveSubISOo( int sub, QXmlStreamWriter &writer );
    int loadSubISOo( int sub, QXmlStreamReader &reader );

    int saveSensorUART( QXmlStreamWriter &writer );
    int loadSensorUART( QXmlStreamReader &reader );

    int saveSubUART( int sub, QXmlStreamWriter &writer );
    int loadSubUART( int sub, QXmlStreamReader &reader );

    int saveSubUARTSensor( int sub, int subSens, QXmlStreamWriter &writer );
    int loadSubUARTSensor( int sub, int subSens, QXmlStreamReader &reader );

    int saveTunning( QXmlStreamWriter &writer );
    int loadTunning( QXmlStreamReader &reader );

    int saveSubTunning( int ax, QXmlStreamWriter &writer );
    int loadSubTunning( int ax, QXmlStreamReader &reader );

    int saveISOi( QXmlStreamWriter &writer );
    int loadISOi( QXmlStreamReader &reader );

    int saveAi( QXmlStreamWriter &writer );
    int loadAi( QXmlStreamReader &reader );

    int saveAngleAlarm( QXmlStreamWriter &writer );
    int loadAngleAlarm( QXmlStreamReader &reader );

    int saveSubAngleAlarm( int sub, QXmlStreamWriter &writer );
    int loadSubAngleAlarm( int sub, QXmlStreamReader &reader );

    int saveDistanceAlarm( QXmlStreamWriter &writer );
    int loadDistanceAlarm( QXmlStreamReader &reader );

    int saveSubDistanceAlarm( int sub, QXmlStreamWriter &writer );
    int loadSubDistanceAlarm( int sub, QXmlStreamReader &reader );

    int saveOtp( QXmlStreamWriter &writer );
    int loadOtp( QXmlStreamReader &reader );

public:
    void setSignature( quint32 hash );
    virtual quint32 getSignature();

    //! desc + alias
    virtual QString getFullDesc( int axesId = -1 );

    QString getDesc();
//    QString getSN();
    QString getSwVer();
    QString getHwVer();
    QString getFwVer();
    QString getBtVer();

    QString getSeqVer();

    int setAccScale( int id, int scale );
    int getAccScale( int id );

    int setDecScale( int id, int scale );
    int getDecScale( int id );

protected:
    QString mDesc;

    QString mSwVer;
    QString mHwVer;
    QString mFwVer;
    QString mBtVer;
    QString mSeqVer;    //! from ca 02

    quint32 mSignature;

    PwmInfo mFanInfo;
    PwmInfo mLedInfo[4];
public:
    QList<int16> mAccList, mDecList;   //! 1/1000
};

}

Q_DECLARE_METATYPE( MegaDevice::MRQ_model )
Q_DECLARE_METATYPE( MegaDevice::MRQ_model *)

#endif
