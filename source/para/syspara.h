#ifndef SYSPARA_H
#define SYSPARA_H

#include <QtCore>

class SysPara
{
public:
    enum enumPort
    {
        port_mega_can,
        port_usb_can,
        port_rs232,
    };
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
    QString mEventLogFile;

    bool mbSearchOnOpen;
    bool mbAutoLoadSetup;
    bool mbMaximizeStartup;
    bool mbAutoLoadPrj;
    bool mbAffirmZero;
    bool mbAutoStatusView;


                            //! misa
    bool mMisaEn;
    quint32 mMisaSocket;

    bool mComEn;
    QString mComName;

    QString mRemoteDirPath;

                                    //! motion
    double mGeometryResolution;     //! 5mm
    double mAngleResolution;        //! degree

                            //! user
    int mStyleIndex, mLangIndex;
    QString mLatestPrjPath, mLatestPrjName;

                            //! system alias
    QStringList mAliases, mSNList;
};

#endif // SYSPARA_H
