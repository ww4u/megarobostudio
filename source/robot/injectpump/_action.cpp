#include "injectpump.h"
#include "../../device/mrq/deviceMRQ.h"

int robotInject::inj_move( const tpvRegion &region,
                           float dh, float dt )
{
    return move( 0, dh, dt,
                 0, 0,
                 region );
}

int robotInject::valve_move( const tpvRegion &region,
                           float dv, float dt )
{
    return move( dv, 0, dt,
                 0, 0,
                 region );
}

