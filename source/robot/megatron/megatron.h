
#ifndef _ROBOT_MEGATRON_H_
#define _ROBOT_MEGATRON_H_

#include "../rawrobo/rawrobo.h"

#include "../../device/mrq/deviceMRQ.h"

#include "../../model/handactionmodel.h"

class robotMegatron : public RawRobo
{
public:
    robotMegatron();

public:
    virtual int serialIn( QXmlStreamReader &reader );
    virtual int serialOut( QXmlStreamWriter &writer );

public:
    virtual void onMsg( int subAxes, RoboMsg &msg );

    virtual int download( tpvGroup *pGroup, int axes = 0 );
    virtual int download( motionGroup *pGroup, int axes = 0 );

    virtual int download( QList<tpvGroup*> &groups,
                          QList<int> &joints );      //! joint tab id

    virtual int download( VRobot *pSetup );

    virtual int run( int axes );
    virtual int stop( int axes );

    virtual int run( );
    virtual int stop( );

    virtual void onLine();
    virtual void offLine();

public:
    virtual QAbstractTableModel *handActions();

protected:
    handActionModel mHandActionModel;

};

#endif
