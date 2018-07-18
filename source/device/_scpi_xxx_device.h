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

#define DECLARE_SCPI()      protected:  \
                                virtual const void* loadScpiCmd();

#define CMD_ITEM( cmd, api )    { cmd, api, 0 }

#define COM_ITEMs()          CMD_ITEM( "SYSTem:ERRor?", SCPI_SystemErrorNextQ ),\
                            CMD_ITEM( "SYSTem:ERRor:NEXT?", SCPI_SystemErrorNextQ ),\
                            CMD_ITEM( "SYSTem:ERRor:COUNt?", SCPI_SystemErrorCountQ )

#define LOCAL_OBJ( type )      ((type*)context->user_context)

#endif
