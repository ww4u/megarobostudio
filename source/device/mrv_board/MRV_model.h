#ifndef _MRV_MODEL_H_
#define _MRV_MODEL_H_
#include "../../../include/mcstd.h"
#include "../phydevice.h"

#include "../../intf/iserial.h"

#include "_MRV_model.h"

namespace MegaDevice
{


class MRV_model : public PhyDevice, public _MRV_model
{
public:
    MRV_model();

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

    int saveMotion( QXmlStreamWriter &writer );
    int loadMotion( QXmlStreamReader &reader );

    int saveSubMotion( int iSub, QXmlStreamWriter &writer );
    int loadSubMotion( int iSub, QXmlStreamReader &reader );

    int saveGlobal( QXmlStreamWriter &writer );
    int loadGlobal( QXmlStreamReader &reader );

    int saveSubGlobal( int iSub, QXmlStreamWriter &writer );
    int loadSubGlobal( int iSub, QXmlStreamReader &reader );

    int saveValve( QXmlStreamWriter &writer );
    int loadValve( QXmlStreamReader &reader );

    int saveSubValve( int iSub, QXmlStreamWriter &writer );
    int loadSubValve( int iSub, QXmlStreamReader &reader );

    int saveThreshold( QXmlStreamWriter &writer );
    int loadThreshold( QXmlStreamReader &reader );

    int saveSubThreshold( int iSub, QXmlStreamWriter &writer );
    int loadSubThreshold( int iSub, QXmlStreamReader &reader );

    int saveCalibration( QXmlStreamWriter &writer );
    int loadCalibration( QXmlStreamReader &reader );

    int saveSubCalibration( int iSub, QXmlStreamWriter &writer );
    int loadSubCalibration( int iSub, QXmlStreamReader &reader );

    int saveIOConfig( QXmlStreamWriter &writer );
    int loadIOConfig( QXmlStreamReader &reader );

    int saveSubIOConfig( int iSub, QXmlStreamWriter &writer );
    int loadSubIOConfig( int iSub, QXmlStreamReader &reader );

public:
    void setSignature( quint32 hash );
    quint32 getSignature();

    //! desc + alias
    QString getFullDesc( int axesId = -1 );

    QString getDesc();
    QString getSN();
    QString getSwVer();
    QString getHwVer();
    QString getFwVer();
    QString getBtVer();

    QString getSeqVer();

protected:
    QString mDesc;

    QString mSn;
    QString mSwVer;
    QString mHwVer;
    QString mFwVer;
    QString mBtVer;
    QString mSeqVer;

    quint32 mSignature;
};

}

Q_DECLARE_METATYPE( MegaDevice::MRV_model )
Q_DECLARE_METATYPE( MegaDevice::MRV_model *)

#endif
