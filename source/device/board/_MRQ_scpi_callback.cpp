static scpi_result_t _scpi_setLINK_INTFC( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_LINK_INTFC val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_LINK_INTFC_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setLINK_INTFC( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getLINK_INTFC( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_LINK_INTFC oval0;

	(GET_OBJ(context))->getLINK_INTFC( &oval0 );
	pLocalStr = MRQ_LINK_INTFC_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSYSTEM_WORKMODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SYSTEM_WORKMODE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_WORKMODE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSYSTEM_WORKMODE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSYSTEM_WORKMODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SYSTEM_WORKMODE oval0;

	(GET_OBJ(context))->getSYSTEM_WORKMODE( &oval0 );
	pLocalStr = MRQ_SYSTEM_WORKMODE_toString( oval0);
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

	MRQ_SYSTEM_POWERON val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_POWERON_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSYSTEM_POWERON( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSYSTEM_POWERON( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SYSTEM_POWERON oval0;

	(GET_OBJ(context))->getSYSTEM_POWERON( &oval0 );
	pLocalStr = MRQ_SYSTEM_POWERON_toString( oval0);
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

	MRQ_SYSTEM_TYPE oval0;

	MRQ_SYSTEM_TYPE_1 oval1;

	(GET_OBJ(context))->getSYSTEM_TYPE( &oval0, &oval1 );
	pLocalStr = MRQ_SYSTEM_TYPE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	pLocalStr = MRQ_SYSTEM_TYPE_1_toString( oval1);
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
static scpi_result_t _scpi_getSYSTEM_FPGAVER( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	char oval0;

	char oval1;

	char oval2;

	(GET_OBJ(context))->getSYSTEM_FPGAVER( &oval0, &oval1, &oval2 );
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
static scpi_result_t _scpi_setSYSTEM_REPORTSWITCH( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SYSTEM_REPORTSWITCH val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSYSTEM_REPORTSWITCH( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSYSTEM_REPORTSWITCH( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getSYSTEM_REPORTSWITCH( &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setRS232_BAUD( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_RS232_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_RS232_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setRS232_BAUD( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getRS232_BAUD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_RS232_BAUD oval0;

	(GET_OBJ(context))->getRS232_BAUD( &oval0 );
	pLocalStr = MRQ_RS232_BAUD_toString( oval0);
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

	MRQ_RS232_WORDLEN val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_RS232_WORDLEN_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setRS232_WORDLEN( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getRS232_WORDLEN( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_RS232_WORDLEN oval0;

	(GET_OBJ(context))->getRS232_WORDLEN( &oval0 );
	pLocalStr = MRQ_RS232_WORDLEN_toString( oval0);
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

	MRQ_RS232_FLOWCTL val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_RS232_FLOWCTL_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setRS232_FLOWCTL( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getRS232_FLOWCTL( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_RS232_FLOWCTL oval0;

	(GET_OBJ(context))->getRS232_FLOWCTL( &oval0 );
	pLocalStr = MRQ_RS232_FLOWCTL_toString( oval0);
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

	MRQ_RS232_PARITY val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_RS232_PARITY_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setRS232_PARITY( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getRS232_PARITY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_RS232_PARITY oval0;

	(GET_OBJ(context))->getRS232_PARITY( &oval0 );
	pLocalStr = MRQ_RS232_PARITY_toString( oval0);
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

	MRQ_RS232_STOPBIT val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_RS232_STOPBIT_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setRS232_STOPBIT( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getRS232_STOPBIT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_RS232_STOPBIT oval0;

	(GET_OBJ(context))->getRS232_STOPBIT( &oval0 );
	pLocalStr = MRQ_RS232_STOPBIT_toString( oval0);
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

	MRQ_CAN_TYPE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_CAN_TYPE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCAN_TYPE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_TYPE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_CAN_TYPE oval0;

	(GET_OBJ(context))->getCAN_TYPE( &oval0 );
	pLocalStr = MRQ_CAN_TYPE_toString( oval0);
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

	MRQ_CAN_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_CAN_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCAN_BAUD( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_BAUD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_CAN_BAUD oval0;

	(GET_OBJ(context))->getCAN_BAUD( &oval0 );
	pLocalStr = MRQ_CAN_BAUD_toString( oval0);
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

	MRQ_SYSTEM_REPORTSWITCH val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCAN_NETMANAGELED( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCAN_NETMANAGESTATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_CAN_NETMANAGESTATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_CAN_NETMANAGESTATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCAN_NETMANAGESTATE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_NETMANAGESTATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_CAN_NETMANAGESTATE oval0;

	(GET_OBJ(context))->getCAN_NETMANAGESTATE( &oval0 );
	pLocalStr = MRQ_CAN_NETMANAGESTATE_toString( oval0);
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

	MRQ_CAN_NETMANAGEID val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_CAN_NETMANAGEID_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint32)localIntVal;

	(SET_OBJ(context))->setCAN_NETMANAGEID( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCAN_NETMANAGEID( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_CAN_NETMANAGEID val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_CAN_NETMANAGEID_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 oval0;

	(GET_OBJ(context))->getCAN_NETMANAGEID( val0, &oval0 );
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
static scpi_result_t _scpi_setCLOCK_FREQUENCY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint32 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint32)localIntVal;

	(SET_OBJ(context))->setCLOCK_FREQUENCY( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCLOCK_FREQUENCY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint32 oval0;

	(GET_OBJ(context))->getCLOCK_FREQUENCY( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCLOCK_SYNCREGISTER( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SYSTEM_REPORTSWITCH val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCLOCK_SYNCREGISTER( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCLOCK_SYNCREGISTER( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getCLOCK_SYNCREGISTER( &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCLOCK_STARTTYPE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_CLOCK_STARTTYPE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_CLOCK_STARTTYPE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCLOCK_STARTTYPE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCLOCK_STARTTYPE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_CLOCK_STARTTYPE oval0;

	(GET_OBJ(context))->getCLOCK_STARTTYPE( &oval0 );
	pLocalStr = MRQ_CLOCK_STARTTYPE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setCLOCK_SYNCSTATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_CLOCK_SYNCSTATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_CLOCK_SYNCSTATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setCLOCK_SYNCSTATE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCLOCK_SYNCSTATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_CLOCK_SYNCSTATE oval0;

	(GET_OBJ(context))->getCLOCK_SYNCSTATE( &oval0 );
	pLocalStr = MRQ_CLOCK_SYNCSTATE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getCLOCK_COUNT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint32 oval0;

	(GET_OBJ(context))->getCLOCK_COUNT( &oval0 );
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

	MRQ_MOTION_SWITCH val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTION_SWITCH_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_SWITCH( val0, val1 );
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

	MRQ_MOTION_STATE oval0;

	(GET_OBJ(context))->getMOTION_STATE( val0, &oval0 );
	pLocalStr = MRQ_MOTION_STATE_toString( oval0);
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

	MRQ_MOTION_STATEREPORT val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTION_STATEREPORT_toValue( pLocalStr, &val1) != 0 ) 
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

	MRQ_MOTION_STATEREPORT oval0;

	(GET_OBJ(context))->getMOTION_STATEREPORT( val0, &oval0 );
	pLocalStr = MRQ_MOTION_STATEREPORT_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_STARTSOURCE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTION_STARTSOURCE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTION_STARTSOURCE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_STARTSOURCE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_STARTSOURCE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTION_STARTSOURCE oval0;

	(GET_OBJ(context))->getMOTION_STARTSOURCE( val0, &oval0 );
	pLocalStr = MRQ_MOTION_STARTSOURCE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_STARTTYPE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_CLOCK_STARTTYPE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_CLOCK_STARTTYPE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_STARTTYPE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_STARTTYPE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_CLOCK_STARTTYPE oval0;

	(GET_OBJ(context))->getMOTION_STARTTYPE( val0, &oval0 );
	pLocalStr = MRQ_CLOCK_STARTTYPE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_MAXSPEED( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_MAXSPEED( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_MAXSPEED( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTION_MAXSPEED( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_MINSPEED( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_MINSPEED( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_MINSPEED( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTION_MINSPEED( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_MAXPOSITION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_MAXPOSITION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_MAXPOSITION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTION_MAXPOSITION( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_MINPOSITION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_MINPOSITION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_MINPOSITION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTION_MINPOSITION( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_MAXTORQUE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_MAXTORQUE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_MAXTORQUE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTION_MAXTORQUE( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_MINTORQUE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_MINTORQUE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_MINTORQUE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTION_MINTORQUE( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_MAXACCELERATION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_MAXACCELERATION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_MAXACCELERATION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTION_MAXACCELERATION( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_MINACCELERATION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_MINACCELERATION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_MINACCELERATION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTION_MINACCELERATION( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_ORIGIN( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_ORIGIN( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_ORIGIN( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getMOTION_ORIGIN( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_GOORIGIN( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setMOTION_GOORIGIN( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_INITPOSITIONUNIT( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTION_INITPOSITIONUNIT val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTION_INITPOSITIONUNIT_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_INITPOSITIONUNIT( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_INITPOSITIONUNIT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTION_INITPOSITIONUNIT oval0;

	(GET_OBJ(context))->getMOTION_INITPOSITIONUNIT( val0, &oval0 );
	pLocalStr = MRQ_MOTION_INITPOSITIONUNIT_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_INITPOSITION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_INITPOSITION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_INITPOSITION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTION_INITPOSITION( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_INITIOSIGNAL( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTION_INITIOSIGNAL val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTION_INITIOSIGNAL_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_INITIOSIGNAL( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_INITIOSIGNAL( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTION_INITIOSIGNAL oval0;

	(GET_OBJ(context))->getMOTION_INITIOSIGNAL( val0, &oval0 );
	pLocalStr = MRQ_MOTION_INITIOSIGNAL_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_GOINITPOSITION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setMOTION_GOINITPOSITION( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_INCHINGMODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTION_INCHINGMODE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTION_INCHINGMODE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_INCHINGMODE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_INCHINGMODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTION_INCHINGMODE oval0;

	(GET_OBJ(context))->getMOTION_INCHINGMODE( val0, &oval0 );
	pLocalStr = MRQ_MOTION_INCHINGMODE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_INCHINGTIME( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_INCHINGTIME( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_INCHINGTIME( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTION_INCHINGTIME( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_INCHING( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_INCHING( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_OFFSETSTATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_SYSTEM_REPORTSWITCH val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_OFFSETSTATE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_OFFSETSTATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getMOTION_OFFSETSTATE( val0, &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_OFFSET( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setMOTION_OFFSET( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_PVTSTEPS( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	int32 oval0;

	(GET_OBJ(context))->getMOTION_PVTSTEPS( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_COUNTSTEPS( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	int32 oval0;

	(GET_OBJ(context))->getMOTION_COUNTSTEPS( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_PVTCIRCLE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	int16 oval0;

	(GET_OBJ(context))->getMOTION_PVTCIRCLE( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_COUNTCIRCLE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	int16 oval0;

	(GET_OBJ(context))->getMOTION_COUNTCIRCLE( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_ABCOUNT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	int32 oval0;

	(GET_OBJ(context))->getMOTION_ABCOUNT( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTION_REVMOTION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_SYSTEM_REPORTSWITCH val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTION_REVMOTION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTION_REVMOTION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getMOTION_REVMOTION( val0, &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setIDENTITY_GROUP( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_IDENTITY_GROUP val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_IDENTITY_GROUP_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	byte val2;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val2 = (byte)localIntVal;

	(SET_OBJ(context))->setIDENTITY_GROUP( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getIDENTITY_GROUP( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_IDENTITY_GROUP val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_IDENTITY_GROUP_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	byte oval0;

	(GET_OBJ(context))->getIDENTITY_GROUP( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setIDENTITY_DISTDEVICE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SYSTEM_REPORTSWITCH val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setIDENTITY_DISTDEVICE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getIDENTITY_DISTDEVICE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getIDENTITY_DISTDEVICE( &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSTOPDECEL_MODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_STOPDECEL_MODE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_STOPDECEL_MODE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSTOPDECEL_MODE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSTOPDECEL_MODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_STOPDECEL_MODE oval0;

	(GET_OBJ(context))->getSTOPDECEL_MODE( val0, &oval0 );
	pLocalStr = MRQ_STOPDECEL_MODE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSTOPDECEL_DISTANCE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSTOPDECEL_DISTANCE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSTOPDECEL_DISTANCE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getSTOPDECEL_DISTANCE( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSTOPDECEL_TIME( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSTOPDECEL_TIME( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSTOPDECEL_TIME( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getSTOPDECEL_TIME( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setOUTOFSTEP_LINESTATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_SYSTEM_REPORTSWITCH val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setOUTOFSTEP_LINESTATE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getOUTOFSTEP_LINESTATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getOUTOFSTEP_LINESTATE( val0, &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setOUTOFSTEP_LINEOUTNUM( scpi_t * context )
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

	(SET_OBJ(context))->setOUTOFSTEP_LINEOUTNUM( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getOUTOFSTEP_LINEOUTNUM( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getOUTOFSTEP_LINEOUTNUM( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setOUTOFSTEP_LINERESPONSE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_OUTOFSTEP_LINERESPONSE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_OUTOFSTEP_LINERESPONSE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setOUTOFSTEP_LINERESPONSE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getOUTOFSTEP_LINERESPONSE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_OUTOFSTEP_LINERESPONSE oval0;

	(GET_OBJ(context))->getOUTOFSTEP_LINERESPONSE( val0, &oval0 );
	pLocalStr = MRQ_OUTOFSTEP_LINERESPONSE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setOUTOFSTEP_LINEWARN( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setOUTOFSTEP_LINEWARN( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setOUTOFSTEP_TOTALSTATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_SYSTEM_REPORTSWITCH val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setOUTOFSTEP_TOTALSTATE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getOUTOFSTEP_TOTALSTATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getOUTOFSTEP_TOTALSTATE( val0, &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setOUTOFSTEP_TOTALOUTNUM( scpi_t * context )
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

	(SET_OBJ(context))->setOUTOFSTEP_TOTALOUTNUM( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getOUTOFSTEP_TOTALOUTNUM( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getOUTOFSTEP_TOTALOUTNUM( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setOUTOFSTEP_TOTALRESPONSE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_OUTOFSTEP_LINERESPONSE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_OUTOFSTEP_LINERESPONSE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setOUTOFSTEP_TOTALRESPONSE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getOUTOFSTEP_TOTALRESPONSE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_OUTOFSTEP_LINERESPONSE oval0;

	(GET_OBJ(context))->getOUTOFSTEP_TOTALRESPONSE( val0, &oval0 );
	pLocalStr = MRQ_OUTOFSTEP_LINERESPONSE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setOUTOFSTEP_TOTALWARN( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setOUTOFSTEP_TOTALWARN( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTOR_STEPANGLE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTOR_STEPANGLE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTOR_STEPANGLE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTOR_STEPANGLE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTOR_STEPANGLE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTOR_STEPANGLE oval0;

	(GET_OBJ(context))->getMOTOR_STEPANGLE( val0, &oval0 );
	pLocalStr = MRQ_MOTOR_STEPANGLE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTOR_TYPE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTOR_TYPE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTOR_TYPE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTOR_TYPE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTOR_TYPE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTOR_TYPE oval0;

	(GET_OBJ(context))->getMOTOR_TYPE( val0, &oval0 );
	pLocalStr = MRQ_MOTOR_TYPE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTOR_POSITIONUNIT( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTOR_POSITIONUNIT val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTOR_POSITIONUNIT_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTOR_POSITIONUNIT( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTOR_POSITIONUNIT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTOR_POSITIONUNIT oval0;

	(GET_OBJ(context))->getMOTOR_POSITIONUNIT( val0, &oval0 );
	pLocalStr = MRQ_MOTOR_POSITIONUNIT_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTOR_GEARRATIONUM( scpi_t * context )
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

	(SET_OBJ(context))->setMOTOR_GEARRATIONUM( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTOR_GEARRATIONUM( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getMOTOR_GEARRATIONUM( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTOR_GEARRATIODEN( scpi_t * context )
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

	(SET_OBJ(context))->setMOTOR_GEARRATIODEN( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTOR_GEARRATIODEN( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getMOTOR_GEARRATIODEN( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTOR_LEAD( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTOR_LEAD( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTOR_LEAD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTOR_LEAD( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTOR_PEAKSPEED( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTOR_PEAKSPEED( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTOR_PEAKSPEED( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTOR_PEAKSPEED( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTOR_PEAKACCELERATION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTOR_PEAKACCELERATION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTOR_PEAKACCELERATION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTOR_PEAKACCELERATION( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTOR_SIZE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTOR_SIZE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTOR_SIZE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTOR_SIZE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTOR_SIZE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTOR_SIZE oval0;

	(GET_OBJ(context))->getMOTOR_SIZE( val0, &oval0 );
	pLocalStr = MRQ_MOTOR_SIZE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTOR_VOLTAGE( scpi_t * context )
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

	(SET_OBJ(context))->setMOTOR_VOLTAGE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTOR_VOLTAGE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	byte oval0;

	(GET_OBJ(context))->getMOTOR_VOLTAGE( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTOR_CURRENT( scpi_t * context )
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

	(SET_OBJ(context))->setMOTOR_CURRENT( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTOR_CURRENT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	byte oval0;

	(GET_OBJ(context))->getMOTOR_CURRENT( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTOR_BACKLASH( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 val1;
	if ( SCPI_ParamFloat(context, &val1, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTOR_BACKLASH( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTOR_BACKLASH( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTOR_BACKLASH( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setENCODER_LINENUM( scpi_t * context )
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

	(SET_OBJ(context))->setENCODER_LINENUM( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getENCODER_LINENUM( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getENCODER_LINENUM( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setENCODER_CHANNELNUM( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_ENCODER_CHANNELNUM val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ENCODER_CHANNELNUM_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setENCODER_CHANNELNUM( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getENCODER_CHANNELNUM( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_ENCODER_CHANNELNUM oval0;

	(GET_OBJ(context))->getENCODER_CHANNELNUM( val0, &oval0 );
	pLocalStr = MRQ_ENCODER_CHANNELNUM_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setENCODER_TYPE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_ENCODER_TYPE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ENCODER_TYPE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setENCODER_TYPE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getENCODER_TYPE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_ENCODER_TYPE oval0;

	(GET_OBJ(context))->getENCODER_TYPE( val0, &oval0 );
	pLocalStr = MRQ_ENCODER_TYPE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setENCODER_MULTIPLE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_ENCODER_MULTIPLE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ENCODER_MULTIPLE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setENCODER_MULTIPLE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getENCODER_MULTIPLE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_ENCODER_MULTIPLE oval0;

	(GET_OBJ(context))->getENCODER_MULTIPLE( val0, &oval0 );
	pLocalStr = MRQ_ENCODER_MULTIPLE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setENCODER_STATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_ENCODER_STATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ENCODER_STATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setENCODER_STATE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getENCODER_STATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_ENCODER_STATE oval0;

	(GET_OBJ(context))->getENCODER_STATE( val0, &oval0 );
	pLocalStr = MRQ_ENCODER_STATE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_PVTCONFIG( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTIONPLAN_PVTCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTIONPLAN_PVTCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTIONPLAN_PVTCONFIG( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_PVTCONFIG( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTIONPLAN_PVTCONFIG oval0;

	(GET_OBJ(context))->getMOTIONPLAN_PVTCONFIG( val0, &oval0 );
	pLocalStr = MRQ_MOTIONPLAN_PVTCONFIG_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_POSITION( scpi_t * context )
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

	(SET_OBJ(context))->setMOTIONPLAN_POSITION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_POSITION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint16 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint16)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTIONPLAN_POSITION( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_VELOCITY( scpi_t * context )
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

	(SET_OBJ(context))->setMOTIONPLAN_VELOCITY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_VELOCITY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint16 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint16)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTIONPLAN_VELOCITY( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_TIME( scpi_t * context )
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

	(SET_OBJ(context))->setMOTIONPLAN_TIME( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_TIME( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint16 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint16)localIntVal;

	f32 oval0;

	(GET_OBJ(context))->getMOTIONPLAN_TIME( val0, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_EXECUTEMODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTIONPLAN_EXECUTEMODE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTIONPLAN_EXECUTEMODE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTIONPLAN_EXECUTEMODE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_EXECUTEMODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTIONPLAN_EXECUTEMODE oval0;

	(GET_OBJ(context))->getMOTIONPLAN_EXECUTEMODE( val0, &oval0 );
	pLocalStr = MRQ_MOTIONPLAN_EXECUTEMODE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_PLANMODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTIONPLAN_PLANMODE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTIONPLAN_PLANMODE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTIONPLAN_PLANMODE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_PLANMODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTIONPLAN_PLANMODE oval0;

	(GET_OBJ(context))->getMOTIONPLAN_PLANMODE( val0, &oval0 );
	pLocalStr = MRQ_MOTIONPLAN_PLANMODE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_MOTIONMODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTIONPLAN_MOTIONMODE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTIONPLAN_MOTIONMODE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTIONPLAN_MOTIONMODE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_MOTIONMODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTIONPLAN_MOTIONMODE oval0;

	(GET_OBJ(context))->getMOTIONPLAN_MOTIONMODE( val0, &oval0 );
	pLocalStr = MRQ_MOTIONPLAN_MOTIONMODE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_MODIFYDUTY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTIONPLAN_MODIFYDUTY val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTIONPLAN_MODIFYDUTY_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTIONPLAN_MODIFYDUTY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_MODIFYDUTY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTIONPLAN_MODIFYDUTY oval0;

	(GET_OBJ(context))->getMOTIONPLAN_MODIFYDUTY( val0, &oval0 );
	pLocalStr = MRQ_MOTIONPLAN_MODIFYDUTY_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_REMAINPOINT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getMOTIONPLAN_REMAINPOINT( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_OUTPUTPOINT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getMOTIONPLAN_OUTPUTPOINT( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_STARTPOINT( scpi_t * context )
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

	(SET_OBJ(context))->setMOTIONPLAN_STARTPOINT( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_STARTPOINT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getMOTIONPLAN_STARTPOINT( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_ENDPOINT( scpi_t * context )
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

	(SET_OBJ(context))->setMOTIONPLAN_ENDPOINT( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_ENDPOINT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getMOTIONPLAN_ENDPOINT( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_CYCLENUM( scpi_t * context )
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

	(SET_OBJ(context))->setMOTIONPLAN_CYCLENUM( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_CYCLENUM( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getMOTIONPLAN_CYCLENUM( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_WARNPOINT( scpi_t * context )
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

	(SET_OBJ(context))->setMOTIONPLAN_WARNPOINT( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_WARNPOINT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint16 oval0;

	(GET_OBJ(context))->getMOTIONPLAN_WARNPOINT( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_WARN( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setMOTIONPLAN_WARN( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_ENDSTATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTIONPLAN_ENDSTATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTIONPLAN_ENDSTATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setMOTIONPLAN_ENDSTATE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_ENDSTATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_MOTIONPLAN_ENDSTATE oval0;

	(GET_OBJ(context))->getMOTIONPLAN_ENDSTATE( val0, &oval0 );
	pLocalStr = MRQ_MOTIONPLAN_ENDSTATE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setMOTIONPLAN_FEEDBACKRATIO( scpi_t * context )
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

	(SET_OBJ(context))->setMOTIONPLAN_FEEDBACKRATIO( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getMOTIONPLAN_FEEDBACKRATIO( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	byte oval0;

	(GET_OBJ(context))->getMOTIONPLAN_FEEDBACKRATIO( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

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

	MRQ_REPORT_STATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_REPORT_STATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SYSTEM_REPORTSWITCH val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val2) != 0 ) 
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

	MRQ_REPORT_STATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_REPORT_STATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getREPORT_STATE( val0, val1, &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
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

	MRQ_REPORT_STATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_REPORT_STATE_toValue( pLocalStr, &val1) != 0 ) 
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

	MRQ_REPORT_STATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_REPORT_STATE_toValue( pLocalStr, &val1) != 0 ) 
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

	MRQ_REPORT_STATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_REPORT_STATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 oval0;

	(GET_OBJ(context))->getREPORT_DATA( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTRIGGER_MODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_MODE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_MODE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTRIGGER_MODE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTRIGGER_MODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_MODE oval0;

	(GET_OBJ(context))->getTRIGGER_MODE( val0, &oval0 );
	pLocalStr = MRQ_TRIGGER_MODE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTRIGGER_PATTSTATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_SYSTEM_REPORTSWITCH val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTRIGGER_PATTSTATE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTRIGGER_PATTSTATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getTRIGGER_PATTSTATE( val0, &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTRIGGER_PATTERN( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_PATTERN val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_PATTERN_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_TRIGGER_PATTERN val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_PATTERN_toValue( pLocalStr, &val2) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTRIGGER_PATTERN( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTRIGGER_PATTERN( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_PATTERN oval0;

	MRQ_TRIGGER_PATTERN oval1;

	(GET_OBJ(context))->getTRIGGER_PATTERN( val0, &oval0, &oval1 );
	pLocalStr = MRQ_TRIGGER_PATTERN_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	pLocalStr = MRQ_TRIGGER_PATTERN_toString( oval1);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTRIGGER_PATTRESP( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_OUTOFSTEP_LINERESPONSE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_OUTOFSTEP_LINERESPONSE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTRIGGER_PATTRESP( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTRIGGER_PATTRESP( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_OUTOFSTEP_LINERESPONSE oval0;

	(GET_OBJ(context))->getTRIGGER_PATTRESP( val0, &oval0 );
	pLocalStr = MRQ_OUTOFSTEP_LINERESPONSE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTRIGGER_PATTSMODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_PATTSMODE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_PATTSMODE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTRIGGER_PATTSMODE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTRIGGER_PATTSMODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_PATTSMODE oval0;

	(GET_OBJ(context))->getTRIGGER_PATTSMODE( val0, &oval0 );
	pLocalStr = MRQ_TRIGGER_PATTSMODE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTRIGGER_PATTSPERIOD( scpi_t * context )
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

	(SET_OBJ(context))->setTRIGGER_PATTSPERIOD( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTRIGGER_PATTSPERIOD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	uint32 oval0;

	(GET_OBJ(context))->getTRIGGER_PATTSPERIOD( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTRIGGER_LEVELSTATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_LEVELSTATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_LEVELSTATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SYSTEM_REPORTSWITCH val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val2) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTRIGGER_LEVELSTATE( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTRIGGER_LEVELSTATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_LEVELSTATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_LEVELSTATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getTRIGGER_LEVELSTATE( val0, val1, &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTRIGGER_LEVELTYPE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_LEVELSTATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_LEVELSTATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_TRIGGER_LEVELTYPE_1 val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_LEVELTYPE_1_toValue( pLocalStr, &val2) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTRIGGER_LEVELTYPE( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTRIGGER_LEVELTYPE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_LEVELSTATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_LEVELSTATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_TRIGGER_LEVELTYPE_1 oval0;

	(GET_OBJ(context))->getTRIGGER_LEVELTYPE( val0, val1, &oval0 );
	pLocalStr = MRQ_TRIGGER_LEVELTYPE_1_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTRIGGER_LEVELRESP( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_LEVELSTATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_LEVELSTATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_OUTOFSTEP_LINERESPONSE val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_OUTOFSTEP_LINERESPONSE_toValue( pLocalStr, &val2) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTRIGGER_LEVELRESP( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTRIGGER_LEVELRESP( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_LEVELSTATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_LEVELSTATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_OUTOFSTEP_LINERESPONSE oval0;

	(GET_OBJ(context))->getTRIGGER_LEVELRESP( val0, val1, &oval0 );
	pLocalStr = MRQ_OUTOFSTEP_LINERESPONSE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTRIGGER_LEVELSMODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_LEVELSTATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_LEVELSTATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_TRIGGER_PATTSMODE val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_PATTSMODE_toValue( pLocalStr, &val2) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTRIGGER_LEVELSMODE( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTRIGGER_LEVELSMODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_LEVELSTATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_LEVELSTATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_TRIGGER_PATTSMODE oval0;

	(GET_OBJ(context))->getTRIGGER_LEVELSMODE( val0, val1, &oval0 );
	pLocalStr = MRQ_TRIGGER_PATTSMODE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setTRIGGER_LEVELSPERIOD( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_LEVELSTATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_LEVELSTATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	f32 val2;
	if ( SCPI_ParamFloat(context, &val2, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setTRIGGER_LEVELSPERIOD( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getTRIGGER_LEVELSPERIOD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_TRIGGER_LEVELSTATE val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_LEVELSTATE_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	f32 oval0;

	(GET_OBJ(context))->getTRIGGER_LEVELSPERIOD( val0, val1, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDRIVER_TYPE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_DRIVER_TYPE oval0;

	(GET_OBJ(context))->getDRIVER_TYPE( val0, &oval0 );
	pLocalStr = MRQ_DRIVER_TYPE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDRIVER_STATEREG( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_DRIVER_STATEREG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DRIVER_STATEREG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 oval0;

	(GET_OBJ(context))->getDRIVER_STATEREG( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDRIVER_CURRENT( scpi_t * context )
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

	(SET_OBJ(context))->setDRIVER_CURRENT( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDRIVER_CURRENT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	byte oval0;

	(GET_OBJ(context))->getDRIVER_CURRENT( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDRIVER_MICROSTEPS( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_DRIVER_MICROSTEPS val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DRIVER_MICROSTEPS_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setDRIVER_MICROSTEPS( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDRIVER_MICROSTEPS( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_DRIVER_MICROSTEPS oval0;

	(GET_OBJ(context))->getDRIVER_MICROSTEPS( val0, &oval0 );
	pLocalStr = MRQ_DRIVER_MICROSTEPS_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDRIVER_SWITCH( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_SYSTEM_REPORTSWITCH val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setDRIVER_SWITCH( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDRIVER_SWITCH( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getDRIVER_SWITCH( val0, &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDRIVER_REGCONFIG( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_DRIVER_REGCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DRIVER_REGCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 val2;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val2 = (uint32)localIntVal;

	(SET_OBJ(context))->setDRIVER_REGCONFIG( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDRIVER_REGCONFIG( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_DRIVER_REGCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DRIVER_REGCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 oval0;

	(GET_OBJ(context))->getDRIVER_REGCONFIG( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDRIVER_SGLIMIT( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_DRIVER_SGLIMIT val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DRIVER_SGLIMIT_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	int8 val2;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val2 = (int8)localIntVal;

	(SET_OBJ(context))->setDRIVER_SGLIMIT( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDRIVER_SGLIMIT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_DRIVER_SGLIMIT oval0;

	int8 oval1;

	(GET_OBJ(context))->getDRIVER_SGLIMIT( val0, &oval0, &oval1 );
	pLocalStr = MRQ_DRIVER_SGLIMIT_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	SCPI_ResultInt32(context, oval1);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDRIVER_SGPARASET( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_DRIVER_SGPARASET val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DRIVER_SGPARASET_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	int16 val2;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val2 = (int16)localIntVal;

	(SET_OBJ(context))->setDRIVER_SGPARASET( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDRIVER_SGPARASET( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_DRIVER_SGPARASET val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DRIVER_SGPARASET_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	int16 oval0;

	(GET_OBJ(context))->getDRIVER_SGPARASET( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDIGITALOUT_STATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_DIGITALOUT_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setDIGITALOUT_STATE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDIGITALOUT_STATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_DIGITALOUT_STATE_1 oval0;

	(GET_OBJ(context))->getDIGITALOUT_STATE( val0, &oval0 );
	pLocalStr = MRQ_DIGITALOUT_STATE_1_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDIGITALOUT_SIGNAL( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_DIGITALOUT_SIGNAL_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_SIGNAL_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setDIGITALOUT_SIGNAL( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDIGITALOUT_SIGNAL( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_DIGITALOUT_SIGNAL_1 oval0;

	(GET_OBJ(context))->getDIGITALOUT_SIGNAL( val0, &oval0 );
	pLocalStr = MRQ_DIGITALOUT_SIGNAL_1_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDIGITALOUT_POLARITY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_DIGITALOUT_POLARITY_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_POLARITY_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setDIGITALOUT_POLARITY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDIGITALOUT_POLARITY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_DIGITALOUT_POLARITY_1 oval0;

	(GET_OBJ(context))->getDIGITALOUT_POLARITY( val0, &oval0 );
	pLocalStr = MRQ_DIGITALOUT_POLARITY_1_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDIGITALOUT_SOURCE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	byte val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (byte)localIntVal;

	(SET_OBJ(context))->setDIGITALOUT_SOURCE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDIGITALOUT_SOURCE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	byte oval0;

	(GET_OBJ(context))->getDIGITALOUT_SOURCE( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDIGITALOUT_CONDITION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_DIGITALOUT_CONDITION_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_CONDITION_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setDIGITALOUT_CONDITION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDIGITALOUT_CONDITION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_DIGITALOUT_CONDITION_1 oval0;

	(GET_OBJ(context))->getDIGITALOUT_CONDITION( val0, &oval0 );
	pLocalStr = MRQ_DIGITALOUT_CONDITION_1_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDIGITALOUT_PERIOD( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint32)localIntVal;

	(SET_OBJ(context))->setDIGITALOUT_PERIOD( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDIGITALOUT_PERIOD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 oval0;

	(GET_OBJ(context))->getDIGITALOUT_PERIOD( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setDIGITALOUT_DUTY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	uint16 val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (uint16)localIntVal;

	(SET_OBJ(context))->setDIGITALOUT_DUTY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getDIGITALOUT_DUTY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_DIGITALOUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	uint16 oval0;

	(GET_OBJ(context))->getDIGITALOUT_DUTY( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setISOLATOROUTPUT_STATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_ISOLATOROUTPUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ISOLATOROUTPUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_DIGITALOUT_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setISOLATOROUTPUT_STATE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getISOLATOROUTPUT_STATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_ISOLATOROUTPUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ISOLATOROUTPUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_DIGITALOUT_STATE_1 oval0;

	(GET_OBJ(context))->getISOLATOROUTPUT_STATE( val0, &oval0 );
	pLocalStr = MRQ_DIGITALOUT_STATE_1_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setISOLATOROUTPUT_SOURCE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_ISOLATOROUTPUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ISOLATOROUTPUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	byte val1;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val1 = (byte)localIntVal;

	(SET_OBJ(context))->setISOLATOROUTPUT_SOURCE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getISOLATOROUTPUT_SOURCE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_ISOLATOROUTPUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ISOLATOROUTPUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	byte oval0;

	(GET_OBJ(context))->getISOLATOROUTPUT_SOURCE( val0, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setISOLATOROUTPUT_CONDITION( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_ISOLATOROUTPUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ISOLATOROUTPUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_DIGITALOUT_CONDITION_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_DIGITALOUT_CONDITION_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setISOLATOROUTPUT_CONDITION( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getISOLATOROUTPUT_CONDITION( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_ISOLATOROUTPUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ISOLATOROUTPUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_DIGITALOUT_CONDITION_1 oval0;

	(GET_OBJ(context))->getISOLATOROUTPUT_CONDITION( val0, &oval0 );
	pLocalStr = MRQ_DIGITALOUT_CONDITION_1_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setISOLATOROUTPUT_RESPONSE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_ISOLATOROUTPUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ISOLATOROUTPUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_ISOLATOROUTPUT_RESPONSE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ISOLATOROUTPUT_RESPONSE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setISOLATOROUTPUT_RESPONSE( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getISOLATOROUTPUT_RESPONSE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_ISOLATOROUTPUT_STATE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_ISOLATOROUTPUT_STATE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_ISOLATOROUTPUT_RESPONSE_1 oval0;

	(GET_OBJ(context))->getISOLATOROUTPUT_RESPONSE( val0, &oval0 );
	pLocalStr = MRQ_ISOLATOROUTPUT_RESPONSE_1_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSENSORUART_BAUD( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }logDbg()<<pLocalStr<<strLen;
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_RS232_BAUD val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }logDbg()<<pLocalStr;
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_RS232_BAUD_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSENSORUART_BAUD( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSENSORUART_BAUD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_RS232_BAUD oval0;

	(GET_OBJ(context))->getSENSORUART_BAUD( val0, &oval0 );
	pLocalStr = MRQ_RS232_BAUD_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSENSORUART_WORDLEN( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_RS232_WORDLEN val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_RS232_WORDLEN_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSENSORUART_WORDLEN( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSENSORUART_WORDLEN( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_RS232_WORDLEN oval0;

	(GET_OBJ(context))->getSENSORUART_WORDLEN( val0, &oval0 );
	pLocalStr = MRQ_RS232_WORDLEN_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSENSORUART_FLOWCTL( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_RS232_FLOWCTL val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_RS232_FLOWCTL_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSENSORUART_FLOWCTL( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSENSORUART_FLOWCTL( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_RS232_FLOWCTL oval0;

	(GET_OBJ(context))->getSENSORUART_FLOWCTL( val0, &oval0 );
	pLocalStr = MRQ_RS232_FLOWCTL_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSENSORUART_PARITY( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_RS232_PARITY val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_RS232_PARITY_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSENSORUART_PARITY( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSENSORUART_PARITY( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_RS232_PARITY oval0;

	(GET_OBJ(context))->getSENSORUART_PARITY( val0, &oval0 );
	pLocalStr = MRQ_RS232_PARITY_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSENSORUART_STOPBIT( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_RS232_STOPBIT val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_RS232_STOPBIT_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSENSORUART_STOPBIT( val0, val1 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSENSORUART_STOPBIT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_RS232_STOPBIT oval0;

	(GET_OBJ(context))->getSENSORUART_STOPBIT( val0, &oval0 );
	pLocalStr = MRQ_RS232_STOPBIT_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSENSORUART_APPLYPARA( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setSENSORUART_APPLYPARA( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSENSORUART_STATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SENSORUART_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SYSTEM_REPORTSWITCH val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val2) != 0 ) 
        { return SCPI_RES_ERR; }logDbg()<<pLocalStr<<val2;

	(SET_OBJ(context))->setSENSORUART_STATE( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSENSORUART_STATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SENSORUART_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getSENSORUART_STATE( val0, val1, &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSENSORUART_SOF( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SENSORUART_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	byte val2;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val2 = (byte)localIntVal;

	(SET_OBJ(context))->setSENSORUART_SOF( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSENSORUART_SOF( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SENSORUART_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	byte oval0;

	(GET_OBJ(context))->getSENSORUART_SOF( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSENSORUART_FRAMELEN( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SENSORUART_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	byte val2;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val2 = (byte)localIntVal;

	(SET_OBJ(context))->setSENSORUART_FRAMELEN( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSENSORUART_FRAMELEN( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SENSORUART_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	byte oval0;

	(GET_OBJ(context))->getSENSORUART_FRAMELEN( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSENSORUART_RECEIVENUM( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SENSORUART_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	byte val2;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val2 = (byte)localIntVal;

	(SET_OBJ(context))->setSENSORUART_RECEIVENUM( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSENSORUART_RECEIVENUM( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SENSORUART_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	byte oval0;

	(GET_OBJ(context))->getSENSORUART_RECEIVENUM( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setSENSORUART_SWITCHTIME( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SENSORUART_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 val2;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val2 = (uint32)localIntVal;

	(SET_OBJ(context))->setSENSORUART_SWITCHTIME( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSENSORUART_SWITCHTIME( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SENSORUART_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	uint32 oval0;

	(GET_OBJ(context))->getSENSORUART_SWITCHTIME( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getSENSORUART_DATA( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SENSORUART_BAUD val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_BAUD_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SENSORUART_STATE_1 val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SENSORUART_STATE_1_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	byte oval0;

	char oval1;

	char oval2;

	char oval3;

	(GET_OBJ(context))->getSENSORUART_DATA( val0, val1, &oval0, &oval1, &oval2, &oval3 );
	SCPI_ResultInt32(context, oval0);

	SCPI_ResultInt32(context, oval1);

	SCPI_ResultInt32(context, oval2);

	SCPI_ResultInt32(context, oval3);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPRESET_PVTCONFIG( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_PRESET_PVTCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_PRESET_PVTCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_MOTIONPLAN_PVTCONFIG val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTIONPLAN_PVTCONFIG_toValue( pLocalStr, &val2) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setPRESET_PVTCONFIG( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPRESET_PVTCONFIG( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_PRESET_PVTCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_PRESET_PVTCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_MOTIONPLAN_PVTCONFIG oval0;

	(GET_OBJ(context))->getPRESET_PVTCONFIG( val0, val1, &oval0 );
	pLocalStr = MRQ_MOTIONPLAN_PVTCONFIG_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPRESET_POSITION( scpi_t * context )
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

	f32 val2;
	if ( SCPI_ParamFloat(context, &val2, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setPRESET_POSITION( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPRESET_POSITION( scpi_t * context )
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

	f32 oval0;

	(GET_OBJ(context))->getPRESET_POSITION( val0, val1, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPRESET_SPEED( scpi_t * context )
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

	f32 val2;
	if ( SCPI_ParamFloat(context, &val2, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setPRESET_SPEED( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPRESET_SPEED( scpi_t * context )
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

	f32 oval0;

	(GET_OBJ(context))->getPRESET_SPEED( val0, val1, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPRESET_TIME( scpi_t * context )
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

	f32 val2;
	if ( SCPI_ParamFloat(context, &val2, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setPRESET_TIME( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPRESET_TIME( scpi_t * context )
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

	f32 oval0;

	(GET_OBJ(context))->getPRESET_TIME( val0, val1, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPRESET_REMAINPOINT( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_PRESET_PVTCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_PRESET_PVTCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	byte oval0;

	(GET_OBJ(context))->getPRESET_REMAINPOINT( val0, val1, &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPRESET_EXECUTE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_PRESET_PVTCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_PRESET_PVTCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_PRESET_EXECUTE_1 val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_PRESET_EXECUTE_1_toValue( pLocalStr, &val2) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setPRESET_EXECUTE( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPRESET_STATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_PRESET_PVTCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_PRESET_PVTCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getPRESET_STATE( val0, val1, &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPRESET_ENDSTATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_PRESET_PVTCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_PRESET_PVTCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_MOTIONPLAN_ENDSTATE val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_MOTIONPLAN_ENDSTATE_toValue( pLocalStr, &val2) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setPRESET_ENDSTATE( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPRESET_ENDSTATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_PRESET_PVTCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_PRESET_PVTCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_MOTIONPLAN_ENDSTATE oval0;

	(GET_OBJ(context))->getPRESET_ENDSTATE( val0, val1, &oval0 );
	pLocalStr = MRQ_MOTIONPLAN_ENDSTATE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPRESET_STOPMODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_PRESET_PVTCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_PRESET_PVTCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_STOPDECEL_MODE val2;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_STOPDECEL_MODE_toValue( pLocalStr, &val2) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setPRESET_STOPMODE( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPRESET_STOPMODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_PRESET_PVTCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_PRESET_PVTCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	MRQ_STOPDECEL_MODE oval0;

	(GET_OBJ(context))->getPRESET_STOPMODE( val0, val1, &oval0 );
	pLocalStr = MRQ_STOPDECEL_MODE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setPRESET_STOPDISTANCE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_PRESET_PVTCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_PRESET_PVTCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	f32 val2;
	if ( SCPI_ParamFloat(context, &val2, true) != SCPI_RES_OK )
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setPRESET_STOPDISTANCE( val0, val1, val2 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getPRESET_STOPDISTANCE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	MRQ_PRESET_PVTCONFIG val1;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_PRESET_PVTCONFIG_toValue( pLocalStr, &val1) != 0 ) 
        { return SCPI_RES_ERR; }

	f32 oval0;

	(GET_OBJ(context))->getPRESET_STOPDISTANCE( val0, val1, &oval0 );
	SCPI_ResultFloat(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setISOLATORIN_STATE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_SYSTEM_REPORTSWITCH val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_SYSTEM_REPORTSWITCH_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setISOLATORIN_STATE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getISOLATORIN_STATE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_SYSTEM_REPORTSWITCH oval0;

	(GET_OBJ(context))->getISOLATORIN_STATE( &oval0 );
	pLocalStr = MRQ_SYSTEM_REPORTSWITCH_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setISOLATORIN_TYPE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_TRIGGER_LEVELTYPE_1 val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_LEVELTYPE_1_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setISOLATORIN_TYPE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getISOLATORIN_TYPE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_TRIGGER_LEVELTYPE_1 oval0;

	(GET_OBJ(context))->getISOLATORIN_TYPE( &oval0 );
	pLocalStr = MRQ_TRIGGER_LEVELTYPE_1_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setISOLATORIN_RESPONSE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_OUTOFSTEP_LINERESPONSE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_OUTOFSTEP_LINERESPONSE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setISOLATORIN_RESPONSE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getISOLATORIN_RESPONSE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_OUTOFSTEP_LINERESPONSE oval0;

	(GET_OBJ(context))->getISOLATORIN_RESPONSE( &oval0 );
	pLocalStr = MRQ_OUTOFSTEP_LINERESPONSE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setISOLATORIN_RESPCHAN( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	byte val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (byte)localIntVal;

	(SET_OBJ(context))->setISOLATORIN_RESPCHAN( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getISOLATORIN_RESPCHAN( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	byte oval0;

	(GET_OBJ(context))->getISOLATORIN_RESPCHAN( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setISOLATORIN_SMODE( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	MRQ_TRIGGER_PATTSMODE val0;
	if ( SCPI_ParamCharacters(context, &pLocalStr, &strLen, true) != true )
        { return SCPI_RES_ERR; }
	if (strLen < 1)
        { return SCPI_RES_ERR; }
	if ( MRQ_TRIGGER_PATTSMODE_toValue( pLocalStr, &val0) != 0 ) 
        { return SCPI_RES_ERR; }

	(SET_OBJ(context))->setISOLATORIN_SMODE( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getISOLATORIN_SMODE( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	MRQ_TRIGGER_PATTSMODE oval0;

	(GET_OBJ(context))->getISOLATORIN_SMODE( &oval0 );
	pLocalStr = MRQ_TRIGGER_PATTSMODE_toString( oval0);
	if ( pLocalStr == NULL ) 
        { return SCPI_RES_ERR; } 
	SCPI_ResultText(context, pLocalStr);

	return SCPI_RES_OK;
}
static scpi_result_t _scpi_setISOLATORIN_SPERIOD( scpi_t * context )
{
	// write
	CHECK_CONTEXT();
	DEF_LOCAL_VAR();

	uint32 val0;
	if ( SCPI_ParamInt32(context, &localIntVal, true) != SCPI_RES_OK ) 
        { return SCPI_RES_ERR; }
	val0 = (uint32)localIntVal;

	(SET_OBJ(context))->setISOLATORIN_SPERIOD( val0 );
	return SCPI_RES_OK;
}
static scpi_result_t _scpi_getISOLATORIN_SPERIOD( scpi_t * context )
{
	// read
	DEF_LOCAL_VAR();

	uint32 oval0;

	(GET_OBJ(context))->getISOLATORIN_SPERIOD( &oval0 );
	SCPI_ResultInt32(context, oval0);

	return SCPI_RES_OK;
}
