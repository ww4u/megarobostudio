#include "sinanju.h"

int robotSinanju::convertTrace( QList<TraceKeyPoint> &curve,
                                xxxGroup<jointsTrace> &jointsPlan,
                                QList< tpvGroup *> &groups,
                                QList< int > &sectionList
                                )
{
    //! 0.check
    if( jointsPlan.size() > 0 )
    {}
    else
    {
        sysError( QObject::tr("no plan") );
        return ERR_NO_TPV_DATA;
    }

    int ret;

    //! 0~3
    ret = buildTpvGroup( jointsPlan, groups );
    if ( ret != 0 )
    {
        sysError( QObject::tr("split joint") );
        return ret;
    }
    sectionList<<0<<groups.size();

    //! tail
    //! for hand
    tpvGroup *pGroup = new tpvGroup();
    if ( NULL == pGroup )
    { return ERR_ALLOC_FAIL; }

    for ( int i = 0; i < curve.size(); i++ )
    {
        ret = pGroup->addItem( curve.at(i).t, curve.at(i).hand, 0 );
        if ( ret != 0 )
        {
            delete pGroup;
            sysError( QObject::tr("new group") );
            return ERR_FAIL_ADD_TPV;
        }
    }
    groups.append( pGroup );
    sectionList<<4<<1;

    return 0;
}

//! only for 4 joints
int robotSinanju::downloadTrace( const tpvRegion &region,
                                 QList< tpvGroup *> &groups )
{
    int ret;
    if ( groups.size() != axes() )
    {
        sysError(QObject::tr("not enough axes") );
        return ERR_INVALID_DATA;
    }

    onLine();

    fsm( region )->setState( MegaDevice::mrq_state_program );

    QList<int> jointsTabList;
    jointsTabList<<0<<1<<2<<3<<4;
    ret = download( groups, jointsTabList, region );
    if ( ret != 0 )
    { return ret; }

    return 0;
}

int robotSinanju::buildTpvGroup( xxxGroup<jointsTrace> &jointsPlan,
                                  QList< tpvGroup *> &jointsGroup )
{
    //! create each joints group
    delete_all( jointsGroup );

    int ret;
    //! for each axis
    int axisDir;

    for ( int i = 0; i < 4; i++ )
    {
        tpvGroup *pGroup = new tpvGroup();
        Q_ASSERT( NULL != pGroup );

        axisDir = 1;

        //! for each data
        for ( int j = 0; j < jointsPlan.size(); j++ )
        {
            ret = pGroup->addItem(
                             jointsPlan.data()[j].t,
                             jointsPlan.data()[j].p[i],
                             jointsPlan.data()[j].v[i]
                             );
            if ( ret != 0 )
            {
                delete_all( jointsGroup );
                return ret;
            }
        }

        jointsGroup.append( pGroup );
    }

    return 0;
}
