#ifndef SCPIASSIST_H
#define SCPIASSIST_H

#include "scpi/scpi.h"
#include <QtCore>
scpi_result_t deload_ax_page( scpi_t * context,
                              int &ax, int &page );

scpi_result_t deload_ax_page_file( scpi_t * context,
                                   int &ax, int &page,
                                   QString &file );

#endif // SCPIASSIST_H
