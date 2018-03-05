#ifndef _SCPI_XXX_DEVICE_H_
#define _SCPI_XXX_DEVICE_H_


#ifndef NULL	
#define NULL 0
#endif

#define CHECK_CONTEXT()	\
                        if ( context->user_context == NULL )\
			{ return SCPI_RES_ERR; }

#define DEF_LOCAL_VAR()	int localIntVal=0;\
                                                const char *pLocalStr=0;\
												size_t strLen = 0;
			
#endif
