#include "bspCan.h"

#include "string.h"

int main( int argc, char **argv )
{
    int ret;

    //! init + open
    ret = bspCanInit();
    if ( ret != 0 )
    { return ret; }

    //! send
    CAN_OBJ obj;

    memset( &obj, 0, sizeof(obj) );
    obj.ID = 0x01;
    obj.ExternFlag = 0x01;
    obj.DataLen = 0x02;

    obj.Data[0] = 0x01;
    obj.Data[1] = 0x01;

    ret = bspCanSend( &obj );
    if ( ret != 0 )
    { bspCanClose(); }

    bspCanClose();

    return 0;
}
