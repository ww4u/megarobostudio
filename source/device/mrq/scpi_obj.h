#ifndef _SCPI_OBJ_H_
#define _SCPI_OBJ_H_

//! context to obj 
#define SET_OBJ( context )	((MegaDevice::deviceMRQ*)context->user_context)
//#define GET_OBJ( context )	SET_OBJ(context)->getModel()
#define GET_OBJ( context )      ((MegaDevice::deviceMRQ*)context->user_context)

#endif
