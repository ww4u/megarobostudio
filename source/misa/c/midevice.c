
#include "midevice.h"
#include "stdio.h"

ViSession miOpen_( const char *pName,
            const char *addr,
            unsigned int port )
{
    ViStatus viSta;
    ViSession viDef, viDev;
    viSta = viOpenDefaultRM( &viDef );
    if ( viSta != VI_SUCCESS )
    { return 0; }

    //! cat name
    char rsrc[64];
    sprintf( rsrc, "TCPIP::%s::%d::SOCKET", addr, port );

    viSta = viOpen( viDef, rsrc, 0, 2000, &viDev );
    if ( viSta != VI_SUCCESS )
    { return 0; }

    //! set attribute
    viSetAttribute( viDev, VI_ATTR_TERMCHAR, 0X0A );
    viSetAttribute( viDev, VI_ATTR_TERMCHAR_EN, VI_TRUE );

    //! set the name
    viPrintf( viDev, "%s\n", pName );
    viFlush( viDev, VI_WRITE_BUF );

    return viDev;
}

ViSession miOpen( const char *pName )
{
    return miOpen_( pName, "127.0.0.1", 1234 );
}

void miClose( ViSession vi )
{
	viClose( vi );
}
