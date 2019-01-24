#include "rsrcsearchthread.h"

#include "../../include/mcstd.h"
#include "../../bus/canapi.h"
#include <QApplication>

#ifdef NI_VISA
#include "visa.h"
#endif

RsrcSearchThread::RsrcSearchThread( QObject *parent ) : QThread( parent )
{

}

void RsrcSearchThread::run()
{
    mSearchList.clear();

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        search();

    QApplication::restoreOverrideCursor();
}

void RsrcSearchThread::setOption( int type, const QString & patt )
{
    mPortType = type;
    mRsrcPattern = patt;
}

QStringList RsrcSearchThread::searchList()
{ return mSearchList; }

void RsrcSearchThread::search()
{
#ifdef NI_VISA
    if ( mPortType != 0 )
    {
        char descs[1024]={0};
        int ret;

        MegaDevice::CANApi api;

        if ( api.load( "megacandevice.dll", mPortType ) )
        { logDbg()<<mPortType; }
        else
        {logDbg()<<mPortType;
            return;
        }

        ret = api.find( mPortType, descs, sizeof(descs)-1 );

        api.unload();

        if ( ret != 0 )
        {
            QString rawStr = QString( descs );
            logDbg()<<rawStr;
            mSearchList = rawStr.split( ';', QString::SkipEmptyParts );
        }
    }
    else
    {
        ViStatus viStat;
        ViSession viDef;
        viStat = viOpenDefaultRM( &viDef );
        if ( viStat != VI_SUCCESS )
        { return; }

        ViFindList viList;
        ViChar rsrc[64];
        ViUInt32 retCount;
        viStat = viFindRsrc( viDef, mRsrcPattern.toLatin1().data(), &viList, &retCount, rsrc  );
        while( viStat == VI_SUCCESS )
        {
            mSearchList.append( rsrc );
            viStat = viFindNext( viList, rsrc );
        }

        viClose( viDef );
    }

#endif
}
