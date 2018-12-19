#include "scpiassist.h"

//! assist help
#include "../../device/_scpi_xxx_device.h"
#include "../../com/comassist.h"

scpi_result_t deload_ax_page( scpi_t * context,
                              int &ax, int &page )
{
    if ( SCPI_ParamInt32(context, &ax, true) != true )
    { return ( SCPI_RES_ERR ); }
    if ( SCPI_ParamInt32(context, &page, true) != true )
    { return( SCPI_RES_ERR ); }

    return SCPI_RES_OK;
}

scpi_result_t deload_ax_page_file( scpi_t * context,
                                   int &ax, int &page,
                                   QString &file )
{
    DEF_LOCAL_VAR();

    //! ax, page
    if ( deload_ax_page( context, ax, page ) != SCPI_RES_OK )
    { return( SCPI_RES_ERR ); }

    //! file
    if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
    { return( SCPI_RES_ERR ); }
    if (strLen < 1)
    { return( SCPI_RES_ERR ); }

    //! find the input file
    QByteArray byteName( pLocalStr, strLen );

    QString fileInName ( byteName.data() );

    if ( comAssist::ammendFileName( fileInName ) )
    { file = fileInName; }
    else
    { return( SCPI_RES_ERR ); }

    return SCPI_RES_OK;
}
