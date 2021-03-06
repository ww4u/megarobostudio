#ifndef SYSPARA_H
#define SYSPARA_H

#include <QtCore>
#include "../model/relationmodel.h"
#include "../model/appmodel.h"
enum SysMode
{
    sys_normal,
    sys_admin,
};

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
    int mSpeed;             //! speed index
    int mTimeout;
    int mRecvTmo;
    int mInterval;          //! for write interval
    int mTpvInterval;
    int mFailTryCnt;
    bool mbAutoAssignId;
    int mDeviceId;
    int mDeviceCount;

    QString mVisaLanAddr;   //! lan
    QStringList mVisaLanList;

                            //! usb
    QString mVisaUsbAddr;
    QStringList mVisaUsbList;
                            //! 232
    QString mVisa232Addr;
    QStringList mVisa232List;
                            //! -e
    QString mVisaEAddr;
    QStringList mVisaEList;
                            //! uart baud
    int mBaudIndex;
    int mDataWidthIndex;
    int mParityIndex;
    int mStopIndex;
    int mFlowControlIndex;

    int mVisaTmo;           //! common to each

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
    bool mbStopOnSearch;
    bool mbAutoLoadSetup;
    bool mbMaximizeStartup;
    bool mbAutoLoadPrj;
    bool mbAffirmZero;
    bool mbAutoStatusView;
    bool mbShowNotice;
    bool mbShowHelp;
    bool mbStateInterrupt;
    bool mbNewAutoAdd;

                            //! misa
    bool mMisaEn;
    quint32 mMisaSocket;
    int mMisaPortCnt;

    bool mComEn;
    QString mComName;

    QString mRemoteDirPath;

                                    //! motion
    double mGeometryResolution;     //! 5mm
    double mAngleResolution;        //! degree
    bool   mOmitEn;
    double mOmitThreshold;

    double mMaxSpeed, mMaxTerminalSpeed;

                                    //! font
    QString mFontFamily;
    int     mPointSize;

                            //! user
    int mStyleIndex, mLangIndex;
    QString mLatestPrjPath, mLatestPrjName;

                            //! system alias
    bool mAliasEn;
    RelationModel mAlias;

                            //! app model
    AppModel mAppModel;
    int mAppStartDelay;

    SysMode mSysMode;       //! sysmode
};

#endif // SYSPARA_H
