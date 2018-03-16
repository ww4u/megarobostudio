#include "robomsg.h"

#include "../../app/robonetthread.h"

bool RoboMsg::checkMsg( RoboMsg &msg, msg_type msgPatterns[], int n )
{
    for ( int i = 0; i < n; i++ )
    {
        if ( msgPatterns[i].msg == msg.getMsg() )
        {
            return msg.checkType( msgPatterns[i].types );
        }
    }

    return false;
}

RoboMsg::RoboMsg()
{
    mMsg = e_robo_none;
    mMs = QDateTime::currentMSecsSinceEpoch();
}

void RoboMsg::setMsg( eRoboMsg msg )
{ mMsg = msg; }
eRoboMsg RoboMsg::getMsg()
{ return mMsg; }

eRoboMsg RoboMsg::Msg()
{ return mMsg; }

qint64 RoboMsg::operator- ( const RoboMsg &rMsg ) const
{
    return mMs - rMsg.mMs;
}

#define check_tx( x ) if ( t##x != QMetaType::UnknownType && size() > x ) \
                        {\
                            if ( t##x != (QMetaType::Type)at( x ).type() )\
                            { return false; }\
                        }

bool RoboMsg::checkType( QMetaType::Type t0,
                QMetaType::Type t1,
                QMetaType::Type t2,
                QMetaType::Type t3,
                QMetaType::Type t4,
                QMetaType::Type t5 )
{
    check_tx( 0 );
    check_tx( 1 );
    check_tx( 2 );
    check_tx( 3 );

    check_tx( 4 );
    check_tx( 5 );

    return true;
}

bool RoboMsg::checkType( QMetaType::Type ts[6] )
{
    return checkType( ts[0], ts[1], ts[2], ts[3],
                      ts[4], ts[5] );
}


