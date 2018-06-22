#ifndef DATAUPLOADER_H
#define DATAUPLOADER_H

#include <QtCore>

//! delcare
namespace MegaDevice {
class deviceMRQ;
}

class receiveCache;

//! download the tpv to pvts
class DataUploader : public QThread
{
    Q_OBJECT
public:
    DataUploader( QObject *pObj=0 );
    virtual ~DataUploader();

public:
    void kickoff( Priority pri = InheritPriority );
    int state();
    int callRet();

Q_SIGNALS:
    void sig_progress( int cur, int from, int to );

protected:
    virtual void run();

public:
    void requestLoad(
                      receiveCache *pReceive,
                      MegaDevice::deviceMRQ *pDev,
                      int chId,
                      const QString &fileName );

protected:
    int uploadProc();

protected:
    receiveCache *m_pCache;

    MegaDevice::deviceMRQ *m_pDev;
    int mChId;
    QString mFileOutName;

    int mTmoms;
    int mTickms;

    int mState;
    int mCallRet;
};

#endif // DATAUPLOADER_H
