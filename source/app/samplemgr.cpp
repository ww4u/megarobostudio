#include "samplemgr.h"

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "samplethread.h"

sampleMgr::sampleMgr()
{
    m_pDbMeta = NULL;
    m_pInstMgr = NULL;
}

sampleMgr::~sampleMgr()
{
    clear( );
}

void sampleMgr::attachProxy( sampleProxy *pProxy )
{
    Q_ASSERT( NULL != pProxy );
    sampleProxy *pItem = find( *pProxy );

    //! do not find
    if ( NULL == pItem )
    {
        pItem = new sampleProxy( pProxy );

        Q_ASSERT( NULL != pItem );
        mSamples.append( pItem );
    }
    //! find
    else
    {
        *pItem = *pProxy;   //! copy value
    }
}

void sampleMgr::attachProxy( sampleProxy &proxy )
{
    attachProxy( &proxy );
}

void sampleMgr::attachDbMeta( dbMeta *pMeta )
{
    Q_ASSERT( NULL != pMeta );
    m_pDbMeta = pMeta;
}
void sampleMgr::attachInstMgr( MegaDevice::InstMgr *pInstMgr )
{
    Q_ASSERT( NULL != pInstMgr );
    m_pInstMgr = pInstMgr;
}
int  sampleMgr::getCount()
{
    return mSamples.size();
}

void sampleMgr::clear( sampleThread *pCallThread )
{
//    foreach( sampleProxy *pProxy, mSamples )
//    {
//        Q_ASSERT( NULL != pProxy );
//        if ( NULL != pCallThread )
//        {
//            pCallThread->sigSpyUpdated( pProxy->chName(),
//                                        pProxy->mItem,
//                                        0 );
//          }
//    }

    delete_all( mSamples );
}

void sampleMgr::stop()
{
    foreach( sampleProxy *pProxy, mSamples )
    {
        Q_ASSERT( NULL != pProxy );
        pProxy->stop();
    }
}

void sampleMgr::sampleProc( int tickus,
                            sampleThread *pCallThread )
{
    Q_ASSERT( NULL != pCallThread );

    spyItem sampleItem;
    bool bSample;

    QList< sampleProxy *> commitList;
    QList< spyItem > valList;
    //! sample
    foreach (sampleProxy *pProxy, mSamples)
    {
        Q_ASSERT( NULL != pProxy );
        //! sample
        bSample = proxySample( pProxy, tickus, &sampleItem );
        //! success
        if ( bSample )
        {
            //! log in
            int cacheSize = pProxy->logInCache( &sampleItem );

            //! update size
            pCallThread->sigSpyUpdated( pProxy->chName(),
                                        pProxy->mItem,
                                        cacheSize );

            //! \todo commit the item
            logDbg()<<sampleItem.mVal.toHex();

            commitList.append( pProxy );
            valList.append( sampleItem );
        }
    }

    //! commit to db
    commitProc( commitList, valList );
}

bool sampleMgr::proxySample( sampleProxy *pProxy, int tickus, spyItem *pItem )
{
    Q_ASSERT( NULL != pProxy );

    //! find device
    Q_ASSERT( NULL != m_pInstMgr );
    MegaDevice::deviceMRQ *pMrq;
    int ax;

    pMrq = m_pInstMgr->findDevice( pProxy->mChName, &ax );
    if ( NULL == pMrq )
    { return false; }

    //! sample
    return pProxy->sample( tickus, pItem, pMrq, ax );
}

void sampleMgr::commitProc( QList< sampleProxy *> &commitList,
                            QList< spyItem > &valList )
{
    //! no item
    if ( commitList.size() > 0 || commitList.size() != valList.size() )
    {}
    else
    { return; }

    //! \note set removeDatabase() doc
    {
        //! db
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        if ( !db.isValid() || m_pDbMeta == NULL || m_pDbMeta->mbUpload != true )
        {
            logDbg()<<db.lastError().text();
            return;
        }

        db.setHostName( m_pDbMeta->mHostName );
        db.setDatabaseName( m_pDbMeta->mDbName );
        db.setUserName( m_pDbMeta->mUserName );
        db.setPassword( m_pDbMeta->mPassword );
        bool ok = db.open();
        if ( !ok )
        {
            logDbg();
            return;
        }

        //! upload
        QSqlQuery query(db);
        QString str;
        sampleProxy *pItem;
        spyItem *pValItem;

        for ( int i = 0; i < commitList.size(); i++ )
        {
            pItem = commitList[i];
            pValItem = &valList[i];

            Q_ASSERT( NULL != pItem && NULL != pValItem );

            str = QString( "INSERT INTO test (hash_code, axes_id, item, epoch_ms, value ) "
                           "VALUES (%1, %2, %3, %4, \'%5\')"
                           ).arg( pItem->mSignature )
                            .arg( pItem->mAx )
                            .arg( pItem->mItem )
                            .arg( pValItem->mMs )
                            .arg( QString(pValItem->mVal.toHex()) );
            logDbg()<<str;
            ok = query.exec( str );
            if ( !ok )
            {
                logDbg()<<query.lastError().text();
                break;
            }
        }

        db.close();
    }

    //! \note see removeDatabase() doc
    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
}

sampleProxy *sampleMgr::find( sampleProxy & proxy )
{
    foreach( sampleProxy *pItem, mSamples )
    {
        if ( pItem->isLike(proxy) )
        { return pItem; }
    }

    return NULL;
}
