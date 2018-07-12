static scpi_result_t _scpi_setLINK_INTFC( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRV_LINK_INTFC val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_LINK_INTFC_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setLINK_INTFC( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getLINK_INTFC( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_LINK_INTFC oval0;

	(GET_OBJ(context))->getLINK_INTFC( &oval0 );
	pLocalStr = MRV_LINK_INTFC_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getLINK_DEVICEINFO( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_LINK_DEVICEINFO oval0;

	MRV_LINK_DEVICEINFO_1 oval1;

	uint32 oval2;

	(GET_OBJ(context))->getLINK_DEVICEINFO( &oval0, &oval1, &oval2 );
	pLocalStr = MRV_LINK_DEVICEINFO_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	pLocalStr = MRV_LINK_DEVICEINFO_1_toString( oval1);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	SCPI_ResultInt32(context, oval2);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSYSTEM_WORKMODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRV_SYSTEM_WORKMODE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_SYSTEM_WORKMODE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSYSTEM_WORKMODE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSYSTEM_WORKMODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_SYSTEM_WORKMODE oval0;

	(GET_OBJ(context))->getSYSTEM_WORKMODE( &oval0 );
	pLocalStr = MRV_SYSTEM_WORKMODE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSYSTEM_POWERON( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRV_SYSTEM_POWERON val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_SYSTEM_POWERON_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSYSTEM_POWERON( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSYSTEM_POWERON( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_SYSTEM_POWERON oval0;

	(GET_OBJ(context))->getSYSTEM_POWERON( &oval0 );
	pLocalStr = MRV_SYSTEM_POWERON_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSYSTEM_SN( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte oval0;

	char oval1;

	char oval2;

	char oval3;

	char oval4;

	char oval5;

	(GET_OBJ(context))->getSYSTEM_SN( &oval0, &oval1, &oval2, &oval3, &oval4, &oval5 );
	SCPI_ResultInt32(context, oval0);

	SCPI_ResultInt32(context, oval1);

	SCPI_ResultInt32(context, oval2);

	SCPI_ResultInt32(context, oval3);

	SCPI_ResultInt32(context, oval4);

	SCPI_ResultInt32(context, oval5);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSYSTEM_TYPE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_SYSTEM_TYPE oval0;

	MRV_LINK_DEVICEINFO_1 oval1;

	(GET_OBJ(context))->getSYSTEM_TYPE( &oval0, &oval1 );
	pLocalStr = MRV_SYSTEM_TYPE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	pLocalStr = MRV_LINK_DEVICEINFO_1_toString( oval1);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSYSTEM_SOFTVER( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	char oval0;

	char oval1;

	char oval2;

	(GET_OBJ(context))->getSYSTEM_SOFTVER( &oval0, &oval1, &oval2 );
	SCPI_ResultInt32(context, oval0);

	SCPI_ResultInt32(context, oval1);

	SCPI_ResultInt32(context, oval2);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSYSTEM_HARDVER( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	char oval0;

	char oval1;

	(GET_OBJ(context))->getSYSTEM_HARDVER( &oval0, &oval1 );
	SCPI_ResultInt32(context, oval0);

	SCPI_ResultInt32(context, oval1);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSYSTEM_BOOTVER( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	char oval0;

	char oval1;

	(GET_OBJ(context))->getSYSTEM_BOOTVER( &oval0, &oval1 );
	SCPI_ResultInt32(context, oval0);

	SCPI_ResultInt32(context, oval1);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSYSTEM_ERROR( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	char oval0;

	char oval1;

	char oval2;

	char oval3;

	char oval4;

	char oval5;

	(GET_OBJ(context))->getSYSTEM_ERROR( &oval0, &oval1, &oval2, &oval3, &oval4, &oval5 );
	SCPI_ResultInt32(context, oval0);

	SCPI_ResultInt32(context, oval1);

	SCPI_ResultInt32(context, oval2);

	SCPI_ResultInt32(context, oval3);

	SCPI_ResultInt32(context, oval4);

	SCPI_ResultInt32(context, oval5);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSYSTEM_WARNCLEAR( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setSYSTEM_WARNCLEAR( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setRS232_BAUD( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRV_RS232_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_RS232_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setRS232_BAUD( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getRS232_BAUD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_RS232_BAUD oval0;

	(GET_OBJ(context))->getRS232_BAUD( &oval0 );
	pLocalStr = MRV_RS232_BAUD_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setRS232_WORDLEN( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRV_RS232_WORDLEN val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_RS232_WORDLEN_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setRS232_WORDLEN( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getRS232_WORDLEN( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_RS232_WORDLEN oval0;

	(GET_OBJ(context))->getRS232_WORDLEN( &oval0 );
	pLocalStr = MRV_RS232_WORDLEN_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setRS232_FLOWCTL( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRV_RS232_FLOWCTL val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_RS232_FLOWCTL_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setRS232_FLOWCTL( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getRS232_FLOWCTL( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_RS232_FLOWCTL oval0;

	(GET_OBJ(context))->getRS232_FLOWCTL( &oval0 );
	pLocalStr = MRV_RS232_FLOWCTL_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setRS232_PARITY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRV_RS232_PARITY val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_RS232_PARITY_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setRS232_PARITY( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getRS232_PARITY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_RS232_PARITY oval0;

	(GET_OBJ(context))->getRS232_PARITY( &oval0 );
	pLocalStr = MRV_RS232_PARITY_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setRS232_STOPBIT( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRV_RS232_STOPBIT val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_RS232_STOPBIT_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setRS232_STOPBIT( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getRS232_STOPBIT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_RS232_STOPBIT oval0;

	(GET_OBJ(context))->getRS232_STOPBIT( &oval0 );
	pLocalStr = MRV_RS232_STOPBIT_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setRS232_APPLYPARA( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	(SET_OBJ(context))->setRS232_APPLYPARA(  );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_TYPE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRV_CAN_TYPE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_CAN_TYPE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCAN_TYPE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_TYPE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_CAN_TYPE oval0;

	(GET_OBJ(context))->getCAN_TYPE( &oval0 );
	pLocalStr = MRV_CAN_TYPE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_BAUD( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRV_CAN_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_CAN_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCAN_BAUD( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_BAUD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_CAN_BAUD oval0;

	(GET_OBJ(context))->getCAN_BAUD( &oval0 );
	pLocalStr = MRV_CAN_BAUD_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_GROUP( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setCAN_GROUP( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_GROUP( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte oval0;

	(GET_OBJ(context))->getCAN_GROUP( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_SENDID( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint32 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint32)localIntVal;

	(SET_OBJ(context))->setCAN_SENDID( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_SENDID( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint32 oval0;

	(GET_OBJ(context))->getCAN_SENDID( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_RECEIVEID( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint32 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint32)localIntVal;

	(SET_OBJ(context))->setCAN_RECEIVEID( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_RECEIVEID( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint32 oval0;

	(GET_OBJ(context))->getCAN_RECEIVEID( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_GROUPID1( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint32 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint32)localIntVal;

	(SET_OBJ(context))->setCAN_GROUPID1( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_GROUPID1( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint32 oval0;

	(GET_OBJ(context))->getCAN_GROUPID1( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_GROUPID2( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint32 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint32)localIntVal;

	(SET_OBJ(context))->setCAN_GROUPID2( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_GROUPID2( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint32 oval0;

	(GET_OBJ(context))->getCAN_GROUPID2( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_BROADCASTID( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint32 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint32)localIntVal;

	(SET_OBJ(context))->setCAN_BROADCASTID( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_BROADCASTID( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint32 oval0;

	(GET_OBJ(context))->getCAN_BROADCASTID( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_APPLYPARA( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	(SET_OBJ(context))->setCAN_APPLYPARA(  );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_NETMANAGELED( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRV_CAN_NETMANAGELED val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_CAN_NETMANAGELED_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCAN_NETMANAGELED( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_NETMANAGESTATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRV_CAN_NETMANAGESTATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_CAN_NETMANAGESTATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCAN_NETMANAGESTATE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_NETMANAGESTATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRV_CAN_NETMANAGESTATE oval0;

	(GET_OBJ(context))->getCAN_NETMANAGESTATE( &oval0 );
	pLocalStr = MRV_CAN_NETMANAGESTATE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_NETMANAGEID( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint32 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint32)localIntVal;

	(SET_OBJ(context))->setCAN_NETMANAGEID( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_NETMANAGEID( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint32 oval0;

	(GET_OBJ(context))->getCAN_NETMANAGEID( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_NETMANAGEHASH( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint32 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint32)localIntVal;

	(SET_OBJ(context))->setCAN_NETMANAGEHASH( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_NETMANAGEHASH( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint32 oval0;

	(GET_OBJ(context))->getCAN_NETMANAGEHASH( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_NETMANAGESIGNATURE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint32 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint32)localIntVal;

	(SET_OBJ(context))->setCAN_NETMANAGESIGNATURE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_NETMANAGESIGNATURE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint32 oval0;

	(GET_OBJ(context))->getCAN_NETMANAGESIGNATURE( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_SWITCH( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_MOTION_SWITCH val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_MOTION_SWITCH_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRV_MOTION_SWITCH_1 val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_MOTION_SWITCH_1_toValue( pLocalStr, &val2) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_SWITCH( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_STATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_MOTION_SWITCH_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_MOTION_SWITCH_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRV_MOTION_STATE_1 oval0;

	(GET_OBJ(context))->getMOTION_STATE( val0, val1, &oval0 );
	pLocalStr = MRV_MOTION_STATE_1_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_STATEREPORT( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_MOTION_STATEREPORT val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_MOTION_STATEREPORT_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_STATEREPORT( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_STATEREPORT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_MOTION_STATEREPORT oval0;

	(GET_OBJ(context))->getMOTION_STATEREPORT( val0, &oval0 );
	pLocalStr = MRV_MOTION_STATEREPORT_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setGLOBAL_GROUP( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_GLOBAL_GROUP val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_GLOBAL_GROUP_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	byte val2;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val2 = (byte)localIntVal;

	(SET_OBJ(context))->setGLOBAL_GROUP( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getGLOBAL_GROUP( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_GLOBAL_GROUP val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_GLOBAL_GROUP_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	byte oval0;

	(GET_OBJ(context))->getGLOBAL_GROUP( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setGLOBAL_DISTINGUISH( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_CAN_NETMANAGELED val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_CAN_NETMANAGELED_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setGLOBAL_DISTINGUISH( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getGLOBAL_DISTINGUISH( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_CAN_NETMANAGELED oval0;

	(GET_OBJ(context))->getGLOBAL_DISTINGUISH( val0, &oval0 );
	pLocalStr = MRV_CAN_NETMANAGELED_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_DEVICE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_VALVECTRL_DEVICE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_VALVECTRL_DEVICE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setVALVECTRL_DEVICE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_DEVICE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_VALVECTRL_DEVICE oval0;

	(GET_OBJ(context))->getVALVECTRL_DEVICE( val0, &oval0 );
	pLocalStr = MRV_VALVECTRL_DEVICE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_PWMFREQ( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint32 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint32)localIntVal;

	(SET_OBJ(context))->setVALVECTRL_PWMFREQ( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_PWMFREQ( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint32 oval0;

	(GET_OBJ(context))->getVALVECTRL_PWMFREQ( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_RTDUTY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint32)localIntVal;

	(SET_OBJ(context))->setVALVECTRL_RTDUTY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_RTDUTY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getVALVECTRL_RTDUTY( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_ACTION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_VALVECTRL_ACTION val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_VALVECTRL_ACTION_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setVALVECTRL_ACTION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_ACTION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_VALVECTRL_ACTION oval0;

	(GET_OBJ(context))->getVALVECTRL_ACTION( val0, &oval0 );
	pLocalStr = MRV_VALVECTRL_ACTION_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_IDLEDUTY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint32)localIntVal;

	(SET_OBJ(context))->setVALVECTRL_IDLEDUTY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_IDLEDUTY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getVALVECTRL_IDLEDUTY( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_HOLDDUTY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint32)localIntVal;

	(SET_OBJ(context))->setVALVECTRL_HOLDDUTY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_HOLDDUTY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getVALVECTRL_HOLDDUTY( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_OPENDUTY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint32)localIntVal;

	(SET_OBJ(context))->setVALVECTRL_OPENDUTY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_OPENDUTY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getVALVECTRL_OPENDUTY( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_OPENDLYTIME( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	(SET_OBJ(context))->setVALVECTRL_OPENDLYTIME( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_OPENDLYTIME( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getVALVECTRL_OPENDLYTIME( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_OPENTIME( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	(SET_OBJ(context))->setVALVECTRL_OPENTIME( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_OPENTIME( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getVALVECTRL_OPENTIME( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_RECORDSTATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_CAN_NETMANAGELED val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_CAN_NETMANAGELED_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setVALVECTRL_RECORDSTATE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_RECORDSTATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_CAN_NETMANAGELED oval0;

	(GET_OBJ(context))->getVALVECTRL_RECORDSTATE( val0, &oval0 );
	pLocalStr = MRV_CAN_NETMANAGELED_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_ONNUMS( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getVALVECTRL_ONNUMS( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_ONNUMSCLEAR( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setVALVECTRL_ONNUMSCLEAR( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_OFFNUMS( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getVALVECTRL_OFFNUMS( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_OFFNUMSCLEAR( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setVALVECTRL_OFFNUMSCLEAR( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getVALVECTRL_TIME( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getVALVECTRL_TIME( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setVALVECTRL_TIMECLEAR( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setVALVECTRL_TIMECLEAR( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTHRESHOLD_HIGHPRESSURE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	(SET_OBJ(context))->setTHRESHOLD_HIGHPRESSURE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTHRESHOLD_HIGHPRESSURE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getTHRESHOLD_HIGHPRESSURE( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTHRESHOLD_HIPRESSUREACTION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_THRESHOLD_HIPRESSUREACTION val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_THRESHOLD_HIPRESSUREACTION_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTHRESHOLD_HIPRESSUREACTION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTHRESHOLD_HIPRESSUREACTION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_THRESHOLD_HIPRESSUREACTION oval0;

	(GET_OBJ(context))->getTHRESHOLD_HIPRESSUREACTION( val0, &oval0 );
	pLocalStr = MRV_THRESHOLD_HIPRESSUREACTION_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTHRESHOLD_LOWPRESSURE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	(SET_OBJ(context))->setTHRESHOLD_LOWPRESSURE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTHRESHOLD_LOWPRESSURE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getTHRESHOLD_LOWPRESSURE( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTHRESHOLD_LOWPRESSUREACTION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_THRESHOLD_HIPRESSUREACTION val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_THRESHOLD_HIPRESSUREACTION_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTHRESHOLD_LOWPRESSUREACTION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTHRESHOLD_LOWPRESSUREACTION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_THRESHOLD_HIPRESSUREACTION oval0;

	(GET_OBJ(context))->getTHRESHOLD_LOWPRESSUREACTION( val0, &oval0 );
	pLocalStr = MRV_THRESHOLD_HIPRESSUREACTION_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTHRESHOLD_HIGHCURRENT( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	(SET_OBJ(context))->setTHRESHOLD_HIGHCURRENT( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTHRESHOLD_HIGHCURRENT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getTHRESHOLD_HIGHCURRENT( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTHRESHOLD_HICURRENTACTION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_THRESHOLD_HIPRESSUREACTION val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_THRESHOLD_HIPRESSUREACTION_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTHRESHOLD_HICURRENTACTION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTHRESHOLD_HICURRENTACTION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_THRESHOLD_HIPRESSUREACTION oval0;

	(GET_OBJ(context))->getTHRESHOLD_HICURRENTACTION( val0, &oval0 );
	pLocalStr = MRV_THRESHOLD_HIPRESSUREACTION_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTHRESHOLD_LOWCURRENT( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	(SET_OBJ(context))->setTHRESHOLD_LOWCURRENT( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTHRESHOLD_LOWCURRENT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getTHRESHOLD_LOWCURRENT( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTHRESHOLD_LOWCURRENTACTION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_THRESHOLD_HIPRESSUREACTION val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_THRESHOLD_HIPRESSUREACTION_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTHRESHOLD_LOWCURRENTACTION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTHRESHOLD_LOWCURRENTACTION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_THRESHOLD_HIPRESSUREACTION oval0;

	(GET_OBJ(context))->getTHRESHOLD_LOWCURRENTACTION( val0, &oval0 );
	pLocalStr = MRV_THRESHOLD_HIPRESSUREACTION_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTHRESHOLD_ONNUMS( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint32)localIntVal;

	(SET_OBJ(context))->setTHRESHOLD_ONNUMS( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTHRESHOLD_ONNUMS( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getTHRESHOLD_ONNUMS( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTHRESHOLD_NUMSACTION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_THRESHOLD_HIPRESSUREACTION val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_THRESHOLD_HIPRESSUREACTION_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTHRESHOLD_NUMSACTION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTHRESHOLD_NUMSACTION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_THRESHOLD_HIPRESSUREACTION oval0;

	(GET_OBJ(context))->getTHRESHOLD_NUMSACTION( val0, &oval0 );
	pLocalStr = MRV_THRESHOLD_HIPRESSUREACTION_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTHRESHOLD_TIME( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint32)localIntVal;

	(SET_OBJ(context))->setTHRESHOLD_TIME( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTHRESHOLD_TIME( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getTHRESHOLD_TIME( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTHRESHOLD_TIMEACTION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_THRESHOLD_HIPRESSUREACTION val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_THRESHOLD_HIPRESSUREACTION_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTHRESHOLD_TIMEACTION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTHRESHOLD_TIMEACTION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_THRESHOLD_HIPRESSUREACTION oval0;

	(GET_OBJ(context))->getTHRESHOLD_TIMEACTION( val0, &oval0 );
	pLocalStr = MRV_THRESHOLD_HIPRESSUREACTION_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPVT_EXECMODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_PVT_EXECMODE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_PVT_EXECMODE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setPVT_EXECMODE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPVT_EXECMODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_PVT_EXECMODE oval0;

	(GET_OBJ(context))->getPVT_EXECMODE( val0, &oval0 );
	pLocalStr = MRV_PVT_EXECMODE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPVT_RESET( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setPVT_RESET( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPVT_CYCLES( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint32)localIntVal;

	(SET_OBJ(context))->setPVT_CYCLES( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPVT_CYCLES( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getPVT_CYCLES( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPVT_GRADE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	byte val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (byte)localIntVal;

	uint32 val2;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val2 = (uint32)localIntVal;

	(SET_OBJ(context))->setPVT_GRADE( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPVT_GRADE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	byte val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getPVT_GRADE( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPVT_BUFFERCHECK( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getPVT_BUFFERCHECK( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPVT_START( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setPVT_START( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPVT_START( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte oval0;

	(GET_OBJ(context))->getPVT_START( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPVT_END( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setPVT_END( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPVT_END( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte oval0;

	(GET_OBJ(context))->getPVT_END( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPVT_PDATA( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint16 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint16)localIntVal;

	uint32 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint32)localIntVal;

	(SET_OBJ(context))->setPVT_PDATA( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPVT_PDATA( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint16 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint16)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getPVT_PDATA( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPVT_TDATA( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint16 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint16)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setPVT_TDATA( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPVT_TDATA( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint16 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint16)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getPVT_TDATA( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPVT_HOLD( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_PVT_HOLD val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_PVT_HOLD_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setPVT_HOLD( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPVT_HOLD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_PVT_HOLD oval0;

	(GET_OBJ(context))->getPVT_HOLD( val0, &oval0 );
	pLocalStr = MRV_PVT_HOLD_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPVT_SAVE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setPVT_SAVE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPVT_SAVE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte oval0;

	(GET_OBJ(context))->getPVT_SAVE( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCALIBRATION_TYPE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_CALIBRATION_TYPE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_CALIBRATION_TYPE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCALIBRATION_TYPE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCALIBRATION_TYPE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_CALIBRATION_TYPE oval0;

	(GET_OBJ(context))->getCALIBRATION_TYPE( val0, &oval0 );
	pLocalStr = MRV_CALIBRATION_TYPE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCALIBRATION_SAMPLERATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_CALIBRATION_SAMPLERATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_CALIBRATION_SAMPLERATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCALIBRATION_SAMPLERATE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCALIBRATION_SAMPLERATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_CALIBRATION_SAMPLERATE oval0;

	(GET_OBJ(context))->getCALIBRATION_SAMPLERATE( val0, &oval0 );
	pLocalStr = MRV_CALIBRATION_SAMPLERATE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCALIBRATION_OPENTIME( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	(SET_OBJ(context))->setCALIBRATION_OPENTIME( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCALIBRATION_OPENTIME( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getCALIBRATION_OPENTIME( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCALIBRATION_HOLDTIME( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	(SET_OBJ(context))->setCALIBRATION_HOLDTIME( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCALIBRATION_HOLDTIME( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getCALIBRATION_HOLDTIME( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCALIBRATION_CLOSETIME( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	(SET_OBJ(context))->setCALIBRATION_CLOSETIME( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCALIBRATION_CLOSETIME( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getCALIBRATION_CLOSETIME( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCALIBRATION_OPENDUTY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint32)localIntVal;

	(SET_OBJ(context))->setCALIBRATION_OPENDUTY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCALIBRATION_OPENDUTY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getCALIBRATION_OPENDUTY( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCALIBRATION_HOLDDUTY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint32)localIntVal;

	(SET_OBJ(context))->setCALIBRATION_HOLDDUTY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCALIBRATION_HOLDDUTY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getCALIBRATION_HOLDDUTY( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCALIBRATION_DATALENGTH( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getCALIBRATION_DATALENGTH( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCALIBRATION_DATAUPCTRL( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_CALIBRATION_DATAUPCTRL val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_CALIBRATION_DATAUPCTRL_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCALIBRATION_DATAUPCTRL( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCALIBRATION_DATAUP( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint16 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint16)localIntVal;

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	uint16 val2;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val2 = (uint16)localIntVal;

	(SET_OBJ(context))->setCALIBRATION_DATAUP( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setREPORT_STATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_REPORT_STATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_REPORT_STATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRV_CAN_NETMANAGELED val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_CAN_NETMANAGELED_toValue( pLocalStr, &val2) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setREPORT_STATE( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getREPORT_STATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_REPORT_STATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_REPORT_STATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRV_CAN_NETMANAGELED oval0;

	(GET_OBJ(context))->getREPORT_STATE( val0, val1, &oval0 );
	pLocalStr = MRV_CAN_NETMANAGELED_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setREPORT_PERIOD( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_REPORT_STATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_REPORT_STATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 val2;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val2 = (uint32)localIntVal;

	(SET_OBJ(context))->setREPORT_PERIOD( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getREPORT_PERIOD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_REPORT_STATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_REPORT_STATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 oval0;

	(GET_OBJ(context))->getREPORT_PERIOD( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getREPORT_DATA( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_REPORT_STATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_REPORT_STATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 oval0;

	(GET_OBJ(context))->getREPORT_DATA( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setIOCONFIG_IOFUNCSEL( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_IOCONFIG_IOFUNCSEL val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_IOCONFIG_IOFUNCSEL_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setIOCONFIG_IOFUNCSEL( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getIOCONFIG_IOFUNCSEL( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_IOCONFIG_IOFUNCSEL oval0;

	(GET_OBJ(context))->getIOCONFIG_IOFUNCSEL( val0, &oval0 );
	pLocalStr = MRV_IOCONFIG_IOFUNCSEL_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setIOCONFIG_SWFILTERDELAY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	(SET_OBJ(context))->setIOCONFIG_SWFILTERDELAY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getIOCONFIG_SWFILTERDELAY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getIOCONFIG_SWFILTERDELAY( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setIOCONFIG_SWFILTERCOUNTER( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	(SET_OBJ(context))->setIOCONFIG_SWFILTERCOUNTER( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getIOCONFIG_SWFILTERCOUNTER( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getIOCONFIG_SWFILTERCOUNTER( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setIOCONFIG_TRIGEDGE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_IOCONFIG_TRIGEDGE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_IOCONFIG_TRIGEDGE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setIOCONFIG_TRIGEDGE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getIOCONFIG_TRIGEDGE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_IOCONFIG_TRIGEDGE oval0;

	(GET_OBJ(context))->getIOCONFIG_TRIGEDGE( val0, &oval0 );
	pLocalStr = MRV_IOCONFIG_TRIGEDGE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setIOCONFIG_BAUD( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_RS232_BAUD val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_RS232_BAUD_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setIOCONFIG_BAUD( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getIOCONFIG_BAUD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_RS232_BAUD oval0;

	(GET_OBJ(context))->getIOCONFIG_BAUD( val0, &oval0 );
	pLocalStr = MRV_RS232_BAUD_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setIOCONFIG_WORDLEN( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_RS232_WORDLEN val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_RS232_WORDLEN_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setIOCONFIG_WORDLEN( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getIOCONFIG_WORDLEN( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_RS232_WORDLEN oval0;

	(GET_OBJ(context))->getIOCONFIG_WORDLEN( val0, &oval0 );
	pLocalStr = MRV_RS232_WORDLEN_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setIOCONFIG_FLOWCTL( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_IOCONFIG_FLOWCTL val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_IOCONFIG_FLOWCTL_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setIOCONFIG_FLOWCTL( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getIOCONFIG_FLOWCTL( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_IOCONFIG_FLOWCTL oval0;

	(GET_OBJ(context))->getIOCONFIG_FLOWCTL( val0, &oval0 );
	pLocalStr = MRV_IOCONFIG_FLOWCTL_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setIOCONFIG_PARITY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_RS232_PARITY val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_RS232_PARITY_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setIOCONFIG_PARITY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getIOCONFIG_PARITY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_RS232_PARITY oval0;

	(GET_OBJ(context))->getIOCONFIG_PARITY( val0, &oval0 );
	pLocalStr = MRV_RS232_PARITY_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setIOCONFIG_STOPBIT( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_RS232_STOPBIT val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRV_RS232_STOPBIT_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setIOCONFIG_STOPBIT( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getIOCONFIG_STOPBIT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRV_RS232_STOPBIT oval0;

	(GET_OBJ(context))->getIOCONFIG_STOPBIT( val0, &oval0 );
	pLocalStr = MRV_RS232_STOPBIT_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setIOCONFIG_APPLYPARA( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setIOCONFIG_APPLYPARA( val0 );
	return SCPI_RES_OK;
}
