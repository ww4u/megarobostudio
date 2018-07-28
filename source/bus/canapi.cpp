#include "canapi.h"
#include "../../include/mcstd.h"

namespace MegaDevice {

CANApi::CANApi()
{
    open = NULL;
    close = NULL;

    init = NULL;
    getSize = NULL;

    clear = NULL;

    start = NULL;

    transmit = NULL;
    receive = NULL;

    write = NULL;
    read = NULL;

    mPortId = 0;
}

CANApi::~CANApi()
{
    unload();
}

#ifdef ARCH_32

#include "canapi_win.cpp"

#endif

#ifdef ARCH_RASPBERRY
#include "canapi_raspberry.cpp"
#endif

#ifdef ARCH_LINUX
#include "canapi_linux.cpp"
#endif

}
