#ifndef SCPIASSIST_H
#define SCPIASSIST_H

#include "scpi/scpi.h"
#include <QtCore>
scpi_result_t deload_ax_page( scpi_t * context,
                              int &ax, int &page );

scpi_result_t deload_ax_page_file( scpi_t * context,
                                   int &ax, int &page,
                                   QString &file );


#define DECLARE_ROBO( type )  type *pRobo;\
                                pRobo = ((type*)context->user_context);\
                                Q_ASSERT( NULL != pRobo );

#define ROBO()          pRobo

#define LOCAL_ROBO()    pRobo

#define CHECK_LINK()    if ( pRobo->checkLink() ) \
                        {}\
                        else\
                        { scpi_ret( SCPI_RES_ERR ); } \

#define check_ax_page()     if ( page < 0 || page >= pRobo->regions() )\
                            { \
                                sysError( QObject::tr("Invalid page") ); \
                                scpi_ret( SCPI_RES_ERR ); \
                            }

#define CHECK_LINK_AX_PAGE()    CHECK_LINK();\
                                check_ax_page();

#endif // SCPIASSIST_H
