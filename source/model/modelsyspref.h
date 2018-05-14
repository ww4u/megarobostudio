#ifndef MODELSYSPREF_H
#define MODELSYSPREF_H

#include "../intf/iserial.h"
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
    void setLatestPrj( const QString &path,
                       const QString &name);
    QString& latestPrjPath();
    QString& latestPrjName();

public:
    int mPort;
    int mSpeed;     //! speed index
    int mTimeout;
    int mRecvTmo;
    int mInterval;  //! for write interval
    int mTpvInterval;
    int mFailTryCnt;
    bool mbAutoAssignId;
    int mDeviceId;
    int mDeviceCount;

    QString mVisaAddr;
    int mVisaTmo;
    QStringList mVisaList;

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
    bool mbMaximizeStartup;
    bool mbAutoLoadPrj;
    bool mbAffirmZero;
    bool mbAutoStatusView;

    dbMeta mDbMeta;
                            //! misa
    bool mMisaEn;
    quint32 mMisaSocket;
    QString mRemoteDirPath;

                            //! motion
    int mSpaceResolution;   //! 5mm

                            //! user
    int mStyleIndex, mLangIndex;
    QString mLatestPrjPath, mLatestPrjName;
};

#endif // MODELSYSPREF_H
