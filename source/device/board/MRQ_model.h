#ifndef _MRQ_MODEL_H_
#define _MRQ_MODEL_H_
#include "../../../include/mcstd.h"
#include "../vrobot.h"

#include "../../intf/iserial.h"

#include "_MRQ_model.h"

namespace MegaDevice
{
class MRQ_model : public VRobot, public _MRQ_model
{
public:
    MRQ_model();

public:
//    virtual int save( const QString &str );
//    virtual int load( const QString &str );

    virtual int serialOut( QXmlStreamWriter &writer );
    virtual int serialIn( QXmlStreamReader &reader );

protected:
    int save_motor( int id, QXmlStreamWriter &writer );
    int load_motor( int id, QXmlStreamReader &reader );

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

protected:
    QString mDesc;

    QString mSn;
    QString mSwVer;
    QString mHwVer;
    QString mFwVer;
    QString mBtVer;

//    int mChCnt;

    quint32 mSignature;
};

}

Q_DECLARE_METATYPE( MegaDevice::MRQ_model )
Q_DECLARE_METATYPE( MegaDevice::MRQ_model *)

#endif
