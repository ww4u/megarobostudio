
#include "devicemrv.h"

namespace MegaDevice
{
int deviceMRV::uploadSetting()
{
    int ret;

    receiveCache::cli();

    ret = _uploadSetting();
    if ( ret != 0 )
    { MRV_PROGRESS_HIDE(); }
    else
    { setFilled(true); }

    receiveCache::sti();

    return ret;
}
int deviceMRV::applySetting()
{
    int ret;

    preSet();

    ret = _applySetting();
    if ( ret != 0 )
    { MRV_PROGRESS_HIDE(); }

    postSet();

    return 0;
}

}
