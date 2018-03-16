#ifndef MODELSYSPREF_H
#define MODELSYSPREF_H

#include "../../intf/iserial.h"
#include "dbmeta.h"

class modelSysPref : public ISerial
{
public:
    enum enumPort
    {
        port_mega_can,
        port_usb_can,
        port_rs232,
    };

public:
    modelSysPref();
    void rst();
public:
    virtual int save( const QString &str );
    virtual int load( const QString &str );

protected:
    int loadFromTo( QXmlStreamReader &reader, int &from, int &to );
    int loadDatabase( QXmlStreamReader &reader );

public:
    int mPort;
    int mSpeed;     //! speed index
    int mTimeout;
    int mInterval;
    int mFailTryCnt;

    int mEnumerateTimeout;
    int mSampleTick;


    int mSendIdFrom, mSendIdTo;
    int mRecvIdFrom, mRecvIdTo;
    int mGroupIdFrom, mGroupIdTo;

    double mTimeUnit;
    double mPosUnit;
    double mVelUnit;

    bool mAutoExpand;
    QString mDumpPath;      //! absolute path

    bool mbSearchOnOpen;
    bool mbAutoLoadSetup;

    dbMeta mDbMeta;
                            //! motion
    int mSpaceResolution;   //! 5mm

};

#endif // MODELSYSPREF_H
