#include "MRQ.h"
namespace MegaDevice
{
MRQ::MRQ()
{
}
//! ENUM
//! NONE|CAN|RS232
int MRQ::setLINK_INTFC( MRQ_LINK_INTFC val0 )
{
	//! 1 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 1, 0 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setLINK_INTFC( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getLINK_INTFC(  MRQ_LINK_INTFC * val0, bool bQuery )
{
	//! 1 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 1, 1 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_LINK_INTFC)lval0;
	return 0;
}
//! ENUM
//! NORMAL|NONE
int MRQ::setSYSTEM_WORKMODE( MRQ_SYSTEM_WORKMODE val0 )
{
	//! 2 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 2, 0 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setSYSTEM_WORKMODE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getSYSTEM_WORKMODE(  MRQ_SYSTEM_WORKMODE * val0, bool bQuery )
{
	//! 2 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 2, 1 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_SYSTEM_WORKMODE)lval0;
	return 0;
}
//! ENUM
//! DEFAULT|LAST
int MRQ::setSYSTEM_POWERON( MRQ_SYSTEM_POWERON val0 )
{
	//! 2 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 2, 2 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setSYSTEM_POWERON( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getSYSTEM_POWERON(  MRQ_SYSTEM_POWERON * val0, bool bQuery )
{
	//! 2 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 2, 3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_SYSTEM_POWERON)lval0;
	return 0;
}
//! 
//! 
int MRQ::getSYSTEM_SN(  byte * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery )
{
	//! 2 4
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
	byte lval3 = 0;
	byte lval4 = 0;
	byte lval5 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 2, 4 , &lval0, &lval1, &lval2, &lval3, &lval4, &lval5, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	*val1 = (char)lval1;
	*val2 = (char)lval2;
	*val3 = (char)lval3;
	*val4 = (char)lval4;
	*val5 = (char)lval5;
	return 0;
}
//! 
//! 
int MRQ::getSYSTEM_TYPE(  MRQ_SYSTEM_TYPE * val0, MRQ_SYSTEM_TYPE_1 * val1, bool bQuery )
{
	//! 2 5
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 2, 5 , &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_SYSTEM_TYPE)lval0;
	*val1 = (MRQ_SYSTEM_TYPE_1)lval1;
	return 0;
}
//! 
//! 
int MRQ::getSYSTEM_SOFTVER(  char * val0, char * val1, char * val2, bool bQuery )
{
	//! 2 6
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 2, 6 , &lval0, &lval1, &lval2, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (char)lval0;
	*val1 = (char)lval1;
	*val2 = (char)lval2;
	return 0;
}
//! 
//! 
int MRQ::getSYSTEM_FPGAVER(  char * val0, char * val1, char * val2, bool bQuery )
{
	//! 2 7
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 2, 7 , &lval0, &lval1, &lval2, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (char)lval0;
	*val1 = (char)lval1;
	*val2 = (char)lval2;
	return 0;
}
//! 
//! 
int MRQ::getSYSTEM_HARDVER(  char * val0, char * val1, bool bQuery )
{
	//! 2 8
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 2, 8 , &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (char)lval0;
	*val1 = (char)lval1;
	return 0;
}
//! 
//! 
int MRQ::getSYSTEM_BOOTVER(  char * val0, char * val1, bool bQuery )
{
	//! 2 9
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 2, 9 , &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (char)lval0;
	*val1 = (char)lval1;
	return 0;
}
//! 
//! 
int MRQ::getSYSTEM_ERROR(  char * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery )
{
	//! 2 10
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
	byte lval3 = 0;
	byte lval4 = 0;
	byte lval5 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 2, 10 , &lval0, &lval1, &lval2, &lval3, &lval4, &lval5, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (char)lval0;
	*val1 = (char)lval1;
	*val2 = (char)lval2;
	*val3 = (char)lval3;
	*val4 = (char)lval4;
	*val5 = (char)lval5;
	return 0;
}
//! ENUM
//! OFF|ON
int MRQ::setSYSTEM_REPORTSWITCH( MRQ_SYSTEM_REPORTSWITCH val0 )
{
	//! 2 11
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 2, 11 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setSYSTEM_REPORTSWITCH( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getSYSTEM_REPORTSWITCH(  MRQ_SYSTEM_REPORTSWITCH * val0, bool bQuery )
{
	//! 2 12
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 2, 12 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! ENUM
//! 4800|7200|9600|14400|19200|38400|57600|115200|128000
int MRQ::setRS232_BAUD( MRQ_RS232_BAUD val0 )
{
	//! 3 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 3, 0 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setRS232_BAUD( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getRS232_BAUD(  MRQ_RS232_BAUD * val0, bool bQuery )
{
	//! 3 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 3, 1 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_RS232_BAUD)lval0;
	return 0;
}
//! ENUM
//! 8|9
int MRQ::setRS232_WORDLEN( MRQ_RS232_WORDLEN val0 )
{
	//! 3 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 3, 2 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setRS232_WORDLEN( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getRS232_WORDLEN(  MRQ_RS232_WORDLEN * val0, bool bQuery )
{
	//! 3 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 3, 3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_RS232_WORDLEN)lval0;
	return 0;
}
//! ENUM
//! NONE|RTS|CTS|RTS&CTS
int MRQ::setRS232_FLOWCTL( MRQ_RS232_FLOWCTL val0 )
{
	//! 3 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 3, 4 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setRS232_FLOWCTL( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getRS232_FLOWCTL(  MRQ_RS232_FLOWCTL * val0, bool bQuery )
{
	//! 3 5
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 3, 5 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_RS232_FLOWCTL)lval0;
	return 0;
}
//! ENUM
//! NONE|EVEN|ODD
int MRQ::setRS232_PARITY( MRQ_RS232_PARITY val0 )
{
	//! 3 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 3, 6 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setRS232_PARITY( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getRS232_PARITY(  MRQ_RS232_PARITY * val0, bool bQuery )
{
	//! 3 7
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 3, 7 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_RS232_PARITY)lval0;
	return 0;
}
//! ENUM
//! 1|0.5|2|1.5
int MRQ::setRS232_STOPBIT( MRQ_RS232_STOPBIT val0 )
{
	//! 3 8
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 3, 8 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setRS232_STOPBIT( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getRS232_STOPBIT(  MRQ_RS232_STOPBIT * val0, bool bQuery )
{
	//! 3 9
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 3, 9 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_RS232_STOPBIT)lval0;
	return 0;
}
//! 
//! 
int MRQ::setRS232_APPLYPARA(  )
{
	//! 3 10
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 3, 10  );

	if ( ret == 0) 
	{
		MRQ_model::setRS232_APPLYPARA(  );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! A|B
int MRQ::setCAN_TYPE( MRQ_CAN_TYPE val0 )
{
	//! 4 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 0 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_TYPE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCAN_TYPE(  MRQ_CAN_TYPE * val0, bool bQuery )
{
	//! 4 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 4, 1 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_CAN_TYPE)lval0;
	return 0;
}
//! ENUM
//! 1000|500|250|125|100|50|20|10
int MRQ::setCAN_BAUD( MRQ_CAN_BAUD val0 )
{
	//! 4 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 2 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_BAUD( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCAN_BAUD(  MRQ_CAN_BAUD * val0, bool bQuery )
{
	//! 4 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 4, 3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_CAN_BAUD)lval0;
	return 0;
}
//! U8
//! 
int MRQ::setCAN_GROUP( byte val0 )
{
	//! 4 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 4 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_GROUP( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCAN_GROUP(  byte * val0, bool bQuery )
{
	//! 4 5
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 4, 5 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRQ::setCAN_SENDID( uint32 val0 )
{
	//! 4 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 6 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_SENDID( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCAN_SENDID(  uint32 * val0, bool bQuery )
{
	//! 4 7
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 4, 7 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRQ::setCAN_RECEIVEID( uint32 val0 )
{
	//! 4 8
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 8 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_RECEIVEID( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCAN_RECEIVEID(  uint32 * val0, bool bQuery )
{
	//! 4 9
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 4, 9 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRQ::setCAN_GROUPID1( uint32 val0 )
{
	//! 4 10
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 10 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_GROUPID1( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCAN_GROUPID1(  uint32 * val0, bool bQuery )
{
	//! 4 11
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 4, 11 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRQ::setCAN_GROUPID2( uint32 val0 )
{
	//! 4 12
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 12 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_GROUPID2( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCAN_GROUPID2(  uint32 * val0, bool bQuery )
{
	//! 4 13
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 4, 13 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRQ::setCAN_BROADCASTID( uint32 val0 )
{
	//! 4 14
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 14 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_BROADCASTID( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCAN_BROADCASTID(  uint32 * val0, bool bQuery )
{
	//! 4 15
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 4, 15 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! 
//! 
int MRQ::setCAN_APPLYPARA(  )
{
	//! 4 16
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 16  );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_APPLYPARA(  );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! OFF|ON
int MRQ::setCAN_NETMANAGELED( MRQ_SYSTEM_REPORTSWITCH val0 )
{
	//! 4 17
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 17 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_NETMANAGELED( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! IDLE|HASH|SIGNATURE
int MRQ::setCAN_NETMANAGESTATE( MRQ_CAN_NETMANAGESTATE val0 )
{
	//! 4 18
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 18 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_NETMANAGESTATE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCAN_NETMANAGESTATE(  MRQ_CAN_NETMANAGESTATE * val0, bool bQuery )
{
	//! 4 19
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 4, 19 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_CAN_NETMANAGESTATE)lval0;
	return 0;
}
//! ENUM,U32
//! NONE|SENDID|RECEIVEID
int MRQ::setCAN_NETMANAGEID( MRQ_CAN_NETMANAGEID val0
,uint32 val1 )
{
	//! 4 20
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 20 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_NETMANAGEID( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! NONE|SENDID|RECEIVEID
int MRQ::getCAN_NETMANAGEID( MRQ_CAN_NETMANAGEID val0, uint32 * val1, bool bQuery )
{
	//! 4 21
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 4, 21 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U32
//! 
int MRQ::setCAN_NETMANAGEHASH( uint32 val0 )
{
	//! 4 22
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 4, 22 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCAN_NETMANAGEHASH( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCAN_NETMANAGEHASH(  uint32 * val0, bool bQuery )
{
	//! 4 23
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 4, 23 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRQ::setCLOCK_FREQUENCY( uint32 val0 )
{
	//! 5 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 5, 0 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCLOCK_FREQUENCY( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCLOCK_FREQUENCY(  uint32 * val0, bool bQuery )
{
	//! 5 1
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 5, 1 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! ENUM
//! OFF|ON
int MRQ::setCLOCK_SYNCREGISTER( MRQ_SYSTEM_REPORTSWITCH val0 )
{
	//! 5 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 5, 2 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCLOCK_SYNCREGISTER( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCLOCK_SYNCREGISTER(  MRQ_SYSTEM_REPORTSWITCH * val0, bool bQuery )
{
	//! 5 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 5, 3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! ENUM
//! RADIOID|RECEIVEID|GROUPID1|GROUPID2
int MRQ::setCLOCK_STARTTYPE( MRQ_CLOCK_STARTTYPE val0 )
{
	//! 5 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 5, 4 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCLOCK_STARTTYPE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCLOCK_STARTTYPE(  MRQ_CLOCK_STARTTYPE * val0, bool bQuery )
{
	//! 5 5
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 5, 5 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_CLOCK_STARTTYPE)lval0;
	return 0;
}
//! ENUM
//! END|START
int MRQ::setCLOCK_SYNCSTATE( MRQ_CLOCK_SYNCSTATE val0 )
{
	//! 5 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 5, 6 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setCLOCK_SYNCSTATE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getCLOCK_SYNCSTATE(  MRQ_CLOCK_SYNCSTATE * val0, bool bQuery )
{
	//! 5 7
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 5, 7 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_CLOCK_SYNCSTATE)lval0;
	return 0;
}
//! 
//! 
int MRQ::getCLOCK_COUNT(  uint32 * val0, bool bQuery )
{
	//! 5 8
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 5, 8 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],RESET|STOP|RUN|PREPARE|EMERGSTOP
int MRQ::setMOTION_SWITCH( byte val0
,MRQ_MOTION_SWITCH val1 )
{
	//! 21 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 0 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_SWITCH( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_STATE( byte val0, MRQ_MOTION_STATE * val1, bool bQuery )
{
	//! 21 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTION_STATE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],ACTIVE|QUERY
int MRQ::setMOTION_STATEREPORT( byte val0
,MRQ_MOTION_STATEREPORT val1 )
{
	//! 21 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 2 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_STATEREPORT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_STATEREPORT( byte val0, MRQ_MOTION_STATEREPORT * val1, bool bQuery )
{
	//! 21 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTION_STATEREPORT)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],SOFTWARE|DIGITALIO|CAN|ALL
int MRQ::setMOTION_STARTSOURCE( byte val0
,MRQ_MOTION_STARTSOURCE val1 )
{
	//! 21 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 4 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_STARTSOURCE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_STARTSOURCE( byte val0, MRQ_MOTION_STARTSOURCE * val1, bool bQuery )
{
	//! 21 5
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 5 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTION_STARTSOURCE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],RADIOID|RECEIVEID|GROUPID1|GROUPID2
int MRQ::setMOTION_STARTTYPE( byte val0
,MRQ_CLOCK_STARTTYPE val1 )
{
	//! 21 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 6 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_STARTTYPE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_STARTTYPE( byte val0, MRQ_CLOCK_STARTTYPE * val1, bool bQuery )
{
	//! 21 7
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_CLOCK_STARTTYPE)lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTION_MAXSPEED( byte val0
,f32 val1 )
{
	//! 21 8
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 8 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_MAXSPEED( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_MAXSPEED( byte val0, f32 * val1, bool bQuery )
{
	//! 21 9
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 9 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTION_MINSPEED( byte val0
,f32 val1 )
{
	//! 21 10
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 10 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_MINSPEED( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_MINSPEED( byte val0, f32 * val1, bool bQuery )
{
	//! 21 11
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 11 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTION_MAXPOSITION( byte val0
,f32 val1 )
{
	//! 21 12
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 12 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_MAXPOSITION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_MAXPOSITION( byte val0, f32 * val1, bool bQuery )
{
	//! 21 13
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 13 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTION_MINPOSITION( byte val0
,f32 val1 )
{
	//! 21 14
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 14 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_MINPOSITION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_MINPOSITION( byte val0, f32 * val1, bool bQuery )
{
	//! 21 15
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 15 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTION_MAXTORQUE( byte val0
,f32 val1 )
{
	//! 21 16
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 16 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_MAXTORQUE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_MAXTORQUE( byte val0, f32 * val1, bool bQuery )
{
	//! 21 17
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 17 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTION_MINTORQUE( byte val0
,f32 val1 )
{
	//! 21 18
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 18 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_MINTORQUE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_MINTORQUE( byte val0, f32 * val1, bool bQuery )
{
	//! 21 19
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 19 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTION_MAXACCELERATION( byte val0
,f32 val1 )
{
	//! 21 20
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 20 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_MAXACCELERATION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_MAXACCELERATION( byte val0, f32 * val1, bool bQuery )
{
	//! 21 21
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 21 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTION_MINACCELERATION( byte val0
,f32 val1 )
{
	//! 21 22
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 22 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_MINACCELERATION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_MINACCELERATION( byte val0, f32 * val1, bool bQuery )
{
	//! 21 23
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 23 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRQ::setMOTION_ORIGIN( byte val0
,uint32 val1 )
{
	//! 21 24
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 24 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_ORIGIN( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_ORIGIN( byte val0, uint32 * val1, bool bQuery )
{
	//! 21 25
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 25 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::setMOTION_GOORIGIN( byte val0 )
{
	//! 21 26
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 26 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_GOORIGIN( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],ANGLE|RADIAN|MILLIMETER|IOSIGNAL
int MRQ::setMOTION_INITPOSITIONUNIT( byte val0
,MRQ_MOTION_INITPOSITIONUNIT val1 )
{
	//! 21 27
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 27 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_INITPOSITIONUNIT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_INITPOSITIONUNIT( byte val0, MRQ_MOTION_INITPOSITIONUNIT * val1, bool bQuery )
{
	//! 21 28
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 28 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTION_INITPOSITIONUNIT)lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTION_INITPOSITION( byte val0
,f32 val1 )
{
	//! 21 29
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 29 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_INITPOSITION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_INITPOSITION( byte val0, f32 * val1, bool bQuery )
{
	//! 21 30
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 30 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
int MRQ::setMOTION_INITIOSIGNAL( byte val0
,MRQ_MOTION_INITIOSIGNAL val1 )
{
	//! 21 31
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 31 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_INITIOSIGNAL( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_INITIOSIGNAL( byte val0, MRQ_MOTION_INITIOSIGNAL * val1, bool bQuery )
{
	//! 21 32
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 32 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTION_INITIOSIGNAL)lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::setMOTION_GOINITPOSITION( byte val0 )
{
	//! 21 33
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 33 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_GOINITPOSITION( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],RELTANGLE|RELTRADIAN|RELTMILLIMETER
int MRQ::setMOTION_INCHINGMODE( byte val0
,MRQ_MOTION_INCHINGMODE val1 )
{
	//! 21 34
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 34 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_INCHINGMODE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_INCHINGMODE( byte val0, MRQ_MOTION_INCHINGMODE * val1, bool bQuery )
{
	//! 21 35
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 35 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTION_INCHINGMODE)lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTION_INCHINGTIME( byte val0
,f32 val1 )
{
	//! 21 36
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 36 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_INCHINGTIME( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_INCHINGTIME( byte val0, f32 * val1, bool bQuery )
{
	//! 21 37
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 37 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTION_INCHING( byte val0
,f32 val1 )
{
	//! 21 38
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 38 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_INCHING( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int MRQ::setMOTION_OFFSETSTATE( byte val0
,MRQ_SYSTEM_REPORTSWITCH val1 )
{
	//! 21 39
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 39 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_OFFSETSTATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_OFFSETSTATE( byte val0, MRQ_SYSTEM_REPORTSWITCH * val1, bool bQuery )
{
	//! 21 40
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 40 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::setMOTION_OFFSET( byte val0 )
{
	//! 21 41
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 41 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_OFFSET( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_PVTSTEPS( byte val0, int32 * val1, bool bQuery )
{
	//! 21 42
	int ret = 0;

	int32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 42 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_COUNTSTEPS( byte val0, int32 * val1, bool bQuery )
{
	//! 21 43
	int ret = 0;

	int32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 43 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_PVTCIRCLE( byte val0, int16 * val1, bool bQuery )
{
	//! 21 44
	int ret = 0;

	int16 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 44 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_COUNTCIRCLE( byte val0, int16 * val1, bool bQuery )
{
	//! 21 45
	int ret = 0;

	int16 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 45 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_ABCOUNT( byte val0, int32 * val1, bool bQuery )
{
	//! 21 46
	int ret = 0;

	int32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 46 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int MRQ::setMOTION_REVMOTION( byte val0
,MRQ_SYSTEM_REPORTSWITCH val1 )
{
	//! 21 47
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 47 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_REVMOTION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_REVMOTION( byte val0, MRQ_SYSTEM_REPORTSWITCH * val1, bool bQuery )
{
	//! 21 48
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 48 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! U8,ENUM,U8
//! [CHANNUM],GROUP1|GROUP2
int MRQ::setIDENTITY_GROUP( byte val0
,MRQ_IDENTITY_GROUP val1
,byte val2 )
{
	//! 22 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 22, 0 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setIDENTITY_GROUP( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],GROUP1|GROUP2
int MRQ::getIDENTITY_GROUP( byte val0
,MRQ_IDENTITY_GROUP val1, byte * val2, bool bQuery )
{
	//! 22 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 22, 1 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! ENUM
//! OFF|ON
int MRQ::setIDENTITY_DISTDEVICE( MRQ_SYSTEM_REPORTSWITCH val0 )
{
	//! 22 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 22, 2 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setIDENTITY_DISTDEVICE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getIDENTITY_DISTDEVICE(  MRQ_SYSTEM_REPORTSWITCH * val0, bool bQuery )
{
	//! 22 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 22, 3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],IMMEDIATE|DISTANCE
int MRQ::setSTOPDECEL_MODE( byte val0
,MRQ_STOPDECEL_MODE val1 )
{
	//! 51 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 0 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setSTOPDECEL_MODE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getSTOPDECEL_MODE( byte val0, MRQ_STOPDECEL_MODE * val1, bool bQuery )
{
	//! 51 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_STOPDECEL_MODE)lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setSTOPDECEL_DISTANCE( byte val0
,f32 val1 )
{
	//! 51 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 2 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setSTOPDECEL_DISTANCE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getSTOPDECEL_DISTANCE( byte val0, f32 * val1, bool bQuery )
{
	//! 51 3
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setSTOPDECEL_TIME( byte val0
,f32 val1 )
{
	//! 51 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 4 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setSTOPDECEL_TIME( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getSTOPDECEL_TIME( byte val0, f32 * val1, bool bQuery )
{
	//! 51 5
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 5 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int MRQ::setOUTOFSTEP_LINESTATE( byte val0
,MRQ_SYSTEM_REPORTSWITCH val1 )
{
	//! 52 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 0 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setOUTOFSTEP_LINESTATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getOUTOFSTEP_LINESTATE( byte val0, MRQ_SYSTEM_REPORTSWITCH * val1, bool bQuery )
{
	//! 52 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 52, 1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRQ::setOUTOFSTEP_LINEOUTNUM( byte val0
,uint32 val1 )
{
	//! 52 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 2 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setOUTOFSTEP_LINEOUTNUM( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getOUTOFSTEP_LINEOUTNUM( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 3
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 52, 3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|ALARM|STOP|ALARM&STOP
int MRQ::setOUTOFSTEP_LINERESPONSE( byte val0
,MRQ_OUTOFSTEP_LINERESPONSE val1 )
{
	//! 52 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 4 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setOUTOFSTEP_LINERESPONSE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getOUTOFSTEP_LINERESPONSE( byte val0, MRQ_OUTOFSTEP_LINERESPONSE * val1, bool bQuery )
{
	//! 52 5
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 52, 5 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_OUTOFSTEP_LINERESPONSE)lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::setOUTOFSTEP_LINEWARN( byte val0 )
{
	//! 52 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 6 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setOUTOFSTEP_LINEWARN( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int MRQ::setOUTOFSTEP_TOTALSTATE( byte val0
,MRQ_SYSTEM_REPORTSWITCH val1 )
{
	//! 52 7
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 7 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setOUTOFSTEP_TOTALSTATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getOUTOFSTEP_TOTALSTATE( byte val0, MRQ_SYSTEM_REPORTSWITCH * val1, bool bQuery )
{
	//! 52 8
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 52, 8 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRQ::setOUTOFSTEP_TOTALOUTNUM( byte val0
,uint32 val1 )
{
	//! 52 9
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 9 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setOUTOFSTEP_TOTALOUTNUM( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getOUTOFSTEP_TOTALOUTNUM( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 10
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 52, 10 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|ALARM|STOP|ALARM&STOP
int MRQ::setOUTOFSTEP_TOTALRESPONSE( byte val0
,MRQ_OUTOFSTEP_LINERESPONSE val1 )
{
	//! 52 11
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 11 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setOUTOFSTEP_TOTALRESPONSE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getOUTOFSTEP_TOTALRESPONSE( byte val0, MRQ_OUTOFSTEP_LINERESPONSE * val1, bool bQuery )
{
	//! 52 12
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 52, 12 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_OUTOFSTEP_LINERESPONSE)lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::setOUTOFSTEP_TOTALWARN( byte val0 )
{
	//! 52 13
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 13 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setOUTOFSTEP_TOTALWARN( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],1.8|0.9|15|7.5
int MRQ::setMOTOR_STEPANGLE( byte val0
,MRQ_MOTOR_STEPANGLE val1 )
{
	//! 53 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 0 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTOR_STEPANGLE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTOR_STEPANGLE( byte val0, MRQ_MOTOR_STEPANGLE * val1, bool bQuery )
{
	//! 53 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTOR_STEPANGLE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],ROTARY|LINEAR
int MRQ::setMOTOR_TYPE( byte val0
,MRQ_MOTOR_TYPE val1 )
{
	//! 53 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 2 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTOR_TYPE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTOR_TYPE( byte val0, MRQ_MOTOR_TYPE * val1, bool bQuery )
{
	//! 53 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTOR_TYPE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],ANGLE|RADIAN|MILLIMETER
int MRQ::setMOTOR_POSITIONUNIT( byte val0
,MRQ_MOTOR_POSITIONUNIT val1 )
{
	//! 53 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 4 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTOR_POSITIONUNIT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTOR_POSITIONUNIT( byte val0, MRQ_MOTOR_POSITIONUNIT * val1, bool bQuery )
{
	//! 53 5
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 5 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTOR_POSITIONUNIT)lval0;
	return 0;
}
//! U8,U16
//! [CHANNUM]
int MRQ::setMOTOR_GEARRATIONUM( byte val0
,uint16 val1 )
{
	//! 53 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 6 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTOR_GEARRATIONUM( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTOR_GEARRATIONUM( byte val0, uint16 * val1, bool bQuery )
{
	//! 53 7
	int ret = 0;

	uint16 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U16
//! [CHANNUM]
int MRQ::setMOTOR_GEARRATIODEN( byte val0
,uint16 val1 )
{
	//! 53 8
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 8 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTOR_GEARRATIODEN( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTOR_GEARRATIODEN( byte val0, uint16 * val1, bool bQuery )
{
	//! 53 9
	int ret = 0;

	uint16 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 9 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTOR_LEAD( byte val0
,f32 val1 )
{
	//! 53 10
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 10 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTOR_LEAD( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTOR_LEAD( byte val0, f32 * val1, bool bQuery )
{
	//! 53 11
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 11 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTOR_PEAKSPEED( byte val0
,f32 val1 )
{
	//! 53 12
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 12 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTOR_PEAKSPEED( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTOR_PEAKSPEED( byte val0, f32 * val1, bool bQuery )
{
	//! 53 13
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 13 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTOR_PEAKACCELERATION( byte val0
,f32 val1 )
{
	//! 53 14
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 14 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTOR_PEAKACCELERATION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTOR_PEAKACCELERATION( byte val0, f32 * val1, bool bQuery )
{
	//! 53 15
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 15 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],8|11|14|17|23|24
int MRQ::setMOTOR_SIZE( byte val0
,MRQ_MOTOR_SIZE val1 )
{
	//! 53 16
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 16 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTOR_SIZE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTOR_SIZE( byte val0, MRQ_MOTOR_SIZE * val1, bool bQuery )
{
	//! 53 17
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 17 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTOR_SIZE)lval0;
	return 0;
}
//! U8,U8
//! [CHANNUM]
int MRQ::setMOTOR_VOLTAGE( byte val0
,byte val1 )
{
	//! 53 18
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 18 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTOR_VOLTAGE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTOR_VOLTAGE( byte val0, byte * val1, bool bQuery )
{
	//! 53 19
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 19 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U8
//! [CHANNUM]
int MRQ::setMOTOR_CURRENT( byte val0
,byte val1 )
{
	//! 53 20
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 20 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTOR_CURRENT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTOR_CURRENT( byte val0, byte * val1, bool bQuery )
{
	//! 53 21
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 21 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,F32
//! [CHANNUM]
int MRQ::setMOTOR_BACKLASH( byte val0
,f32 val1 )
{
	//! 53 22
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 22 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTOR_BACKLASH( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTOR_BACKLASH( byte val0, f32 * val1, bool bQuery )
{
	//! 53 23
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 23 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRQ::setENCODER_LINENUM( byte val0
,uint32 val1 )
{
	//! 54 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 54, 0 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setENCODER_LINENUM( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getENCODER_LINENUM( byte val0, uint32 * val1, bool bQuery )
{
	//! 54 1
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 54, 1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],1|3
int MRQ::setENCODER_CHANNELNUM( byte val0
,MRQ_ENCODER_CHANNELNUM val1 )
{
	//! 54 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 54, 2 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setENCODER_CHANNELNUM( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getENCODER_CHANNELNUM( byte val0, MRQ_ENCODER_CHANNELNUM * val1, bool bQuery )
{
	//! 54 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 54, 3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_ENCODER_CHANNELNUM)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],INCREMENTAL|ABSOLUTE
int MRQ::setENCODER_TYPE( byte val0
,MRQ_ENCODER_TYPE val1 )
{
	//! 54 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 54, 4 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setENCODER_TYPE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getENCODER_TYPE( byte val0, MRQ_ENCODER_TYPE * val1, bool bQuery )
{
	//! 54 5
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 54, 5 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_ENCODER_TYPE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],SINGLE|DOUBLE|QUADRUPLE
int MRQ::setENCODER_MULTIPLE( byte val0
,MRQ_ENCODER_MULTIPLE val1 )
{
	//! 54 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 54, 6 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setENCODER_MULTIPLE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getENCODER_MULTIPLE( byte val0, MRQ_ENCODER_MULTIPLE * val1, bool bQuery )
{
	//! 54 7
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 54, 7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_ENCODER_MULTIPLE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|OFF|ON
int MRQ::setENCODER_STATE( byte val0
,MRQ_ENCODER_STATE val1 )
{
	//! 54 8
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 54, 8 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setENCODER_STATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getENCODER_STATE( byte val0, MRQ_ENCODER_STATE * val1, bool bQuery )
{
	//! 54 9
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 54, 9 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_ENCODER_STATE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],END|START|STOP|CLEAR
int MRQ::setMOTIONPLAN_PVTCONFIG( byte val0
,MRQ_MOTIONPLAN_PVTCONFIG val1 )
{
	//! 55 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 0 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_PVTCONFIG( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_PVTCONFIG( byte val0, MRQ_MOTIONPLAN_PVTCONFIG * val1, bool bQuery )
{
	//! 55 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTIONPLAN_PVTCONFIG)lval0;
	return 0;
}
//! U16,F32
//! [INDEX]
int MRQ::setMOTIONPLAN_POSITION( uint16 val0
,f32 val1 )
{
	//! 55 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 2 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_POSITION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U16
//! [INDEX]
int MRQ::getMOTIONPLAN_POSITION( uint16 val0, f32 * val1, bool bQuery )
{
	//! 55 3
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U16,F32
//! [INDEX]
int MRQ::setMOTIONPLAN_VELOCITY( uint16 val0
,f32 val1 )
{
	//! 55 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 4 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_VELOCITY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U16
//! [INDEX]
int MRQ::getMOTIONPLAN_VELOCITY( uint16 val0, f32 * val1, bool bQuery )
{
	//! 55 5
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 5 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U16,F32
//! [INDEX]
int MRQ::setMOTIONPLAN_TIME( uint16 val0
,f32 val1 )
{
	//! 55 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 6 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_TIME( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U16
//! [INDEX]
int MRQ::getMOTIONPLAN_TIME( uint16 val0, f32 * val1, bool bQuery )
{
	//! 55 7
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],CYCLE|INFINITY|FIFO
int MRQ::setMOTIONPLAN_EXECUTEMODE( byte val0
,MRQ_MOTIONPLAN_EXECUTEMODE val1 )
{
	//! 55 8
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 8 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_EXECUTEMODE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_EXECUTEMODE( byte val0, MRQ_MOTIONPLAN_EXECUTEMODE * val1, bool bQuery )
{
	//! 55 9
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 9 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTIONPLAN_EXECUTEMODE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],CUBICPOLY|LINEAR|UNIFORM
int MRQ::setMOTIONPLAN_PLANMODE( byte val0
,MRQ_MOTIONPLAN_PLANMODE val1 )
{
	//! 55 10
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 10 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_PLANMODE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_PLANMODE( byte val0, MRQ_MOTIONPLAN_PLANMODE * val1, bool bQuery )
{
	//! 55 11
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 11 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTIONPLAN_PLANMODE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],PVT|LVT_CORRECT|LVT_NOCORRECT
int MRQ::setMOTIONPLAN_MOTIONMODE( byte val0
,MRQ_MOTIONPLAN_MOTIONMODE val1 )
{
	//! 55 12
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 12 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_MOTIONMODE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_MOTIONMODE( byte val0, MRQ_MOTIONPLAN_MOTIONMODE * val1, bool bQuery )
{
	//! 55 13
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 13 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTIONPLAN_MOTIONMODE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],1/4|1/8|1/16|1/32
int MRQ::setMOTIONPLAN_MODIFYDUTY( byte val0
,MRQ_MOTIONPLAN_MODIFYDUTY val1 )
{
	//! 55 14
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 14 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_MODIFYDUTY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_MODIFYDUTY( byte val0, MRQ_MOTIONPLAN_MODIFYDUTY * val1, bool bQuery )
{
	//! 55 15
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 15 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTIONPLAN_MODIFYDUTY)lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_REMAINPOINT( byte val0, uint16 * val1, bool bQuery )
{
	//! 55 16
	int ret = 0;

	uint16 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 16 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_OUTPUTPOINT( byte val0, uint16 * val1, bool bQuery )
{
	//! 55 17
	int ret = 0;

	uint16 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 17 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U16
//! [CHANNUM]
int MRQ::setMOTIONPLAN_STARTPOINT( byte val0
,uint16 val1 )
{
	//! 55 18
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 18 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_STARTPOINT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_STARTPOINT( byte val0, uint16 * val1, bool bQuery )
{
	//! 55 19
	int ret = 0;

	uint16 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 19 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U16
//! [CHANNUM]
int MRQ::setMOTIONPLAN_ENDPOINT( byte val0
,uint16 val1 )
{
	//! 55 20
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 20 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_ENDPOINT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_ENDPOINT( byte val0, uint16 * val1, bool bQuery )
{
	//! 55 21
	int ret = 0;

	uint16 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 21 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRQ::setMOTIONPLAN_CYCLENUM( byte val0
,uint32 val1 )
{
	//! 55 22
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 22 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_CYCLENUM( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_CYCLENUM( byte val0, uint32 * val1, bool bQuery )
{
	//! 55 23
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 23 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U16
//! [CHANNUM]
int MRQ::setMOTIONPLAN_WARNPOINT( byte val0
,uint16 val1 )
{
	//! 55 24
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 24 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_WARNPOINT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_WARNPOINT( byte val0, uint16 * val1, bool bQuery )
{
	//! 55 25
	int ret = 0;

	uint16 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 25 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::setMOTIONPLAN_WARN( byte val0 )
{
	//! 55 26
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 26 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_WARN( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],STOP|HOLD
int MRQ::setMOTIONPLAN_ENDSTATE( byte val0
,MRQ_MOTIONPLAN_ENDSTATE val1 )
{
	//! 55 27
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 27 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_ENDSTATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_ENDSTATE( byte val0, MRQ_MOTIONPLAN_ENDSTATE * val1, bool bQuery )
{
	//! 55 28
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 28 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTIONPLAN_ENDSTATE)lval0;
	return 0;
}
//! U8,U8
//! [CHANNUM]
int MRQ::setMOTIONPLAN_FEEDBACKRATIO( byte val0
,byte val1 )
{
	//! 55 29
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 55, 29 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_FEEDBACKRATIO( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTIONPLAN_FEEDBACKRATIO( byte val0, byte * val1, bool bQuery )
{
	//! 55 30
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 55, 30 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],TORQUE|CYCLE|XANGLE|YANGLE|ZANGLE|XVEL|YVEL|ZVEL|XACC|YACC|ZACC|SGALL|SGSE|DIST,OFF|ON
int MRQ::setREPORT_STATE( byte val0
,MRQ_REPORT_STATE val1
,MRQ_SYSTEM_REPORTSWITCH val2 )
{
	//! 56 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 56, 0 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setREPORT_STATE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TORQUE|CYCLE|XANGLE|YANGLE|ZANGLE|XVEL|YVEL|ZVEL|XACC|YACC|ZACC|SGALL|SGSE|DIST
int MRQ::getREPORT_STATE( byte val0
,MRQ_REPORT_STATE val1, MRQ_SYSTEM_REPORTSWITCH * val2, bool bQuery )
{
	//! 56 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 56, 1 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! U8,ENUM,U32
//! [CHANNUM],TORQUE|CYCLE|XANGLE|YANGLE|ZANGLE|XVEL|YVEL|ZVEL|XACC|YACC|ZACC|SGALL|SGSE|DIST
int MRQ::setREPORT_PERIOD( byte val0
,MRQ_REPORT_STATE val1
,uint32 val2 )
{
	//! 56 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 56, 2 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setREPORT_PERIOD( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TORQUE|CYCLE|XANGLE|YANGLE|ZANGLE|XVEL|YVEL|ZVEL|XACC|YACC|ZACC|SGALL|SGSE|DIST
int MRQ::getREPORT_PERIOD( byte val0
,MRQ_REPORT_STATE val1, uint32 * val2, bool bQuery )
{
	//! 56 3
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 56, 3 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],TORQUE|CYCLE|XANGLE|YANGLE|ZANGLE|XVEL|YVEL|ZVEL|XACC|YACC|ZACC|SGALL|SGSE|DIST
int MRQ::getREPORT_DATA( byte val0
,MRQ_REPORT_STATE val1, uint32 * val2, bool bQuery )
{
	//! 56 4
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 56, 4 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],PATTERN|LEVEL
int MRQ::setTRIGGER_MODE( byte val0
,MRQ_TRIGGER_MODE val1 )
{
	//! 57 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 57, 0 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_MODE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getTRIGGER_MODE( byte val0, MRQ_TRIGGER_MODE * val1, bool bQuery )
{
	//! 57 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 57, 1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_TRIGGER_MODE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int MRQ::setTRIGGER_PATTSTATE( byte val0
,MRQ_SYSTEM_REPORTSWITCH val1 )
{
	//! 57 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 57, 2 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_PATTSTATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getTRIGGER_PATTSTATE( byte val0, MRQ_SYSTEM_REPORTSWITCH * val1, bool bQuery )
{
	//! 57 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 57, 3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! U8,ENUM,ENUM,ENUM,ENUM,ENUM
//! [CHANNUM],X|L|R|F|H,X|L|R|F|H,X|L|R|F|H,X|L|R|F|H,X|L|R|F|H
int MRQ::setTRIGGER_PATTERN( byte val0
,MRQ_TRIGGER_PATTERN val1
,MRQ_TRIGGER_PATTERN val2
,MRQ_TRIGGER_PATTERN val3
,MRQ_TRIGGER_PATTERN val4
,MRQ_TRIGGER_PATTERN val5 )
{
	//! 57 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 57, 4 , val0, (unsigned char)val1, (unsigned char)val2, (unsigned char)val3, (unsigned char)val4, (unsigned char)val5 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_PATTERN( val0, val1, val2, val3, val4, val5 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getTRIGGER_PATTERN( byte val0, MRQ_TRIGGER_PATTERN * val1, MRQ_TRIGGER_PATTERN * val2, MRQ_TRIGGER_PATTERN * val3, MRQ_TRIGGER_PATTERN * val4, MRQ_TRIGGER_PATTERN * val5, bool bQuery )
{
	//! 57 5
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
	byte lval3 = 0;
	byte lval4 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 57, 5 , val0, &lval0, &lval1, &lval2, &lval3, &lval4, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_TRIGGER_PATTERN)lval0;
	*val2 = (MRQ_TRIGGER_PATTERN)lval1;
	*val3 = (MRQ_TRIGGER_PATTERN)lval2;
	*val4 = (MRQ_TRIGGER_PATTERN)lval3;
	*val5 = (MRQ_TRIGGER_PATTERN)lval4;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|ALARM|STOP|ALARM&STOP
int MRQ::setTRIGGER_PATTRESP( byte val0
,MRQ_OUTOFSTEP_LINERESPONSE val1 )
{
	//! 57 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 57, 6 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_PATTRESP( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getTRIGGER_PATTRESP( byte val0, MRQ_OUTOFSTEP_LINERESPONSE * val1, bool bQuery )
{
	//! 57 7
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 57, 7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_OUTOFSTEP_LINERESPONSE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],CONTINUED|INTERVAL|SINGLE
int MRQ::setTRIGGER_PATTSMODE( byte val0
,MRQ_TRIGGER_PATTSMODE val1 )
{
	//! 57 8
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 57, 8 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_PATTSMODE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getTRIGGER_PATTSMODE( byte val0, MRQ_TRIGGER_PATTSMODE * val1, bool bQuery )
{
	//! 57 9
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 57, 9 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_TRIGGER_PATTSMODE)lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRQ::setTRIGGER_PATTSPERIOD( byte val0
,uint32 val1 )
{
	//! 57 10
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 57, 10 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_PATTSPERIOD( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getTRIGGER_PATTSPERIOD( byte val0, uint32 * val1, bool bQuery )
{
	//! 57 11
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 57, 11 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,OFF|ON
int MRQ::setTRIGGER_LEVELSTATE( byte val0
,MRQ_MOTION_INITIOSIGNAL val1
,MRQ_SYSTEM_REPORTSWITCH val2 )
{
	//! 57 12
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 57, 12 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_LEVELSTATE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
int MRQ::getTRIGGER_LEVELSTATE( byte val0
,MRQ_MOTION_INITIOSIGNAL val1, MRQ_SYSTEM_REPORTSWITCH * val2, bool bQuery )
{
	//! 57 13
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 57, 13 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,RESERVE|LOW|RISE|FALL|HIGH
int MRQ::setTRIGGER_LEVELTYPE( byte val0
,MRQ_MOTION_INITIOSIGNAL val1
,MRQ_TRIGGER_LEVELTYPE_1 val2 )
{
	//! 57 14
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 57, 14 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_LEVELTYPE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
int MRQ::getTRIGGER_LEVELTYPE( byte val0
,MRQ_MOTION_INITIOSIGNAL val1, MRQ_TRIGGER_LEVELTYPE_1 * val2, bool bQuery )
{
	//! 57 15
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 57, 15 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_TRIGGER_LEVELTYPE_1)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,NONE|ALARM|STOP|ALARM&STOP
int MRQ::setTRIGGER_LEVELRESP( byte val0
,MRQ_MOTION_INITIOSIGNAL val1
,MRQ_OUTOFSTEP_LINERESPONSE val2 )
{
	//! 57 16
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 57, 16 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_LEVELRESP( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
int MRQ::getTRIGGER_LEVELRESP( byte val0
,MRQ_MOTION_INITIOSIGNAL val1, MRQ_OUTOFSTEP_LINERESPONSE * val2, bool bQuery )
{
	//! 57 17
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 57, 17 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_OUTOFSTEP_LINERESPONSE)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,CONTINUED|INTERVAL|SINGLE
int MRQ::setTRIGGER_LEVELSMODE( byte val0
,MRQ_MOTION_INITIOSIGNAL val1
,MRQ_TRIGGER_PATTSMODE val2 )
{
	//! 57 18
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 57, 18 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_LEVELSMODE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
int MRQ::getTRIGGER_LEVELSMODE( byte val0
,MRQ_MOTION_INITIOSIGNAL val1, MRQ_TRIGGER_PATTSMODE * val2, bool bQuery )
{
	//! 57 19
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 57, 19 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_TRIGGER_PATTSMODE)lval0;
	return 0;
}
//! U8,ENUM,F32
//! [CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
int MRQ::setTRIGGER_LEVELSPERIOD( byte val0
,MRQ_MOTION_INITIOSIGNAL val1
,f32 val2 )
{
	//! 57 20
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 57, 20 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_LEVELSPERIOD( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
int MRQ::getTRIGGER_LEVELSPERIOD( byte val0
,MRQ_MOTION_INITIOSIGNAL val1, f32 * val2, bool bQuery )
{
	//! 57 21
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 57, 21 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::getDRIVER_TYPE( byte val0, MRQ_DRIVER_TYPE * val1, bool bQuery )
{
	//! 58 0
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 58, 0 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DRIVER_TYPE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],R0|R1|R2
int MRQ::getDRIVER_STATEREG( byte val0
,MRQ_DRIVER_STATEREG val1, uint32 * val2, bool bQuery )
{
	//! 58 1
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 58, 1 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,U8
//! [CHANNUM]
int MRQ::setDRIVER_CURRENT( byte val0
,byte val1 )
{
	//! 58 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 58, 2 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDRIVER_CURRENT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getDRIVER_CURRENT( byte val0, byte * val1, bool bQuery )
{
	//! 58 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 58, 3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],256|128|64|32|16|8|4|2|1
int MRQ::setDRIVER_MICROSTEPS( byte val0
,MRQ_DRIVER_MICROSTEPS val1 )
{
	//! 58 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 58, 4 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDRIVER_MICROSTEPS( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getDRIVER_MICROSTEPS( byte val0, MRQ_DRIVER_MICROSTEPS * val1, bool bQuery )
{
	//! 58 5
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 58, 5 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DRIVER_MICROSTEPS)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int MRQ::setDRIVER_SWITCH( byte val0
,MRQ_SYSTEM_REPORTSWITCH val1 )
{
	//! 58 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 58, 6 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDRIVER_SWITCH( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getDRIVER_SWITCH( byte val0, MRQ_SYSTEM_REPORTSWITCH * val1, bool bQuery )
{
	//! 58 7
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 58, 7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! U8,ENUM,U32
//! [CHANNUM],DRVCTRL|CHOPCONF|SMARTEN|SGCSCONF|DRVCONF
int MRQ::setDRIVER_REGCONFIG( byte val0
,MRQ_DRIVER_REGCONFIG val1
,uint32 val2 )
{
	//! 58 8
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 58, 8 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setDRIVER_REGCONFIG( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],DRVCTRL|CHOPCONF|SMARTEN|SGCSCONF|DRVCONF
int MRQ::getDRIVER_REGCONFIG( byte val0
,MRQ_DRIVER_REGCONFIG val1, uint32 * val2, bool bQuery )
{
	//! 58 9
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 58, 9 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,S8
//! [CHANNUM],UPLIMIT|DOWNLIMIT
int MRQ::setDRIVER_SGLIMIT( byte val0
,MRQ_DRIVER_SGLIMIT val1
,int8 val2 )
{
	//! 58 10
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 58, 10 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setDRIVER_SGLIMIT( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],UPLIMIT|DOWNLIMIT
int MRQ::getDRIVER_SGLIMIT( byte val0
,MRQ_DRIVER_SGLIMIT val1, int8 * val2, bool bQuery )
{
	//! 58 11
	int ret = 0;

	int8 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 58, 11 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,S16
//! [CHANNUM],SG0|SGT|SEMAX|SEMIN
int MRQ::setDRIVER_SGPARASET( byte val0
,MRQ_DRIVER_SGPARASET val1
,int16 val2 )
{
	//! 58 12
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 58, 12 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setDRIVER_SGPARASET( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],SG0|SGT|SEMAX|SEMIN
int MRQ::getDRIVER_SGPARASET( byte val0
,MRQ_DRIVER_SGPARASET val1, int16 * val2, bool bQuery )
{
	//! 58 13
	int ret = 0;

	int16 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 58, 13 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! ENUM,ENUM
//! DO1|DO2,DISABLE|ENABLE
int MRQ::setDIGITALOUT_STATE( MRQ_DIGITALOUT_STATE val0
,MRQ_DIGITALOUT_STATE_1 val1 )
{
	//! 59 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 59, 0 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUT_STATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2
int MRQ::getDIGITALOUT_STATE( MRQ_DIGITALOUT_STATE val0, MRQ_DIGITALOUT_STATE_1 * val1, bool bQuery )
{
	//! 59 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 59, 1 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DIGITALOUT_STATE_1)lval0;
	return 0;
}
//! ENUM,ENUM
//! DO1|DO2,LEVEL|NONE
int MRQ::setDIGITALOUT_SIGNAL( MRQ_DIGITALOUT_STATE val0
,MRQ_DIGITALOUT_SIGNAL_1 val1 )
{
	//! 59 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 59, 2 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUT_SIGNAL( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2
int MRQ::getDIGITALOUT_SIGNAL( MRQ_DIGITALOUT_STATE val0, MRQ_DIGITALOUT_SIGNAL_1 * val1, bool bQuery )
{
	//! 59 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 59, 3 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DIGITALOUT_SIGNAL_1)lval0;
	return 0;
}
//! ENUM,ENUM
//! DO1|DO2,POSITIVE|NEGATIVE
int MRQ::setDIGITALOUT_POLARITY( MRQ_DIGITALOUT_STATE val0
,MRQ_DIGITALOUT_POLARITY_1 val1 )
{
	//! 59 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 59, 4 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUT_POLARITY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2
int MRQ::getDIGITALOUT_POLARITY( MRQ_DIGITALOUT_STATE val0, MRQ_DIGITALOUT_POLARITY_1 * val1, bool bQuery )
{
	//! 59 5
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 59, 5 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DIGITALOUT_POLARITY_1)lval0;
	return 0;
}
//! ENUM,U8
//! DO1|DO2,[CHANNUM]
int MRQ::setDIGITALOUT_SOURCE( MRQ_DIGITALOUT_STATE val0
,byte val1 )
{
	//! 59 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 59, 6 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUT_SOURCE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2
int MRQ::getDIGITALOUT_SOURCE( MRQ_DIGITALOUT_STATE val0, byte * val1, bool bQuery )
{
	//! 59 7
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 59, 7 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,ENUM
//! DO1|DO2,AUTO|NONE
int MRQ::setDIGITALOUT_CONDITION( MRQ_DIGITALOUT_STATE val0
,MRQ_DIGITALOUT_CONDITION_1 val1 )
{
	//! 59 8
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 59, 8 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUT_CONDITION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2
int MRQ::getDIGITALOUT_CONDITION( MRQ_DIGITALOUT_STATE val0, MRQ_DIGITALOUT_CONDITION_1 * val1, bool bQuery )
{
	//! 59 9
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 59, 9 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DIGITALOUT_CONDITION_1)lval0;
	return 0;
}
//! ENUM,U32
//! DO1|DO2
int MRQ::setDIGITALOUT_PERIOD( MRQ_DIGITALOUT_STATE val0
,uint32 val1 )
{
	//! 59 10
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 59, 10 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUT_PERIOD( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2
int MRQ::getDIGITALOUT_PERIOD( MRQ_DIGITALOUT_STATE val0, uint32 * val1, bool bQuery )
{
	//! 59 11
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 59, 11 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,U16
//! DO1|DO2
int MRQ::setDIGITALOUT_DUTY( MRQ_DIGITALOUT_STATE val0
,uint16 val1 )
{
	//! 59 12
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 59, 12 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUT_DUTY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2
int MRQ::getDIGITALOUT_DUTY( MRQ_DIGITALOUT_STATE val0, uint16 * val1, bool bQuery )
{
	//! 59 13
	int ret = 0;

	uint16 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 59, 13 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,ENUM
//! YO1|YO2,DISABLE|ENABLE
int MRQ::setISOLATOROUTPUT_STATE( MRQ_ISOLATOROUTPUT_STATE val0
,MRQ_DIGITALOUT_STATE_1 val1 )
{
	//! 60 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 60, 0 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setISOLATOROUTPUT_STATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! YO1|YO2
int MRQ::getISOLATOROUTPUT_STATE( MRQ_ISOLATOROUTPUT_STATE val0, MRQ_DIGITALOUT_STATE_1 * val1, bool bQuery )
{
	//! 60 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 60, 1 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DIGITALOUT_STATE_1)lval0;
	return 0;
}
//! ENUM,U8
//! YO1|YO2,[CHANNUM]
int MRQ::setISOLATOROUTPUT_SOURCE( MRQ_ISOLATOROUTPUT_STATE val0
,byte val1 )
{
	//! 60 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 60, 2 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setISOLATOROUTPUT_SOURCE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! YO1|YO2
int MRQ::getISOLATOROUTPUT_SOURCE( MRQ_ISOLATOROUTPUT_STATE val0, byte * val1, bool bQuery )
{
	//! 60 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 60, 3 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,ENUM
//! YO1|YO2,AUTO|NONE
int MRQ::setISOLATOROUTPUT_CONDITION( MRQ_ISOLATOROUTPUT_STATE val0
,MRQ_DIGITALOUT_CONDITION_1 val1 )
{
	//! 60 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 60, 4 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setISOLATOROUTPUT_CONDITION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! YO1|YO2
int MRQ::getISOLATOROUTPUT_CONDITION( MRQ_ISOLATOROUTPUT_STATE val0, MRQ_DIGITALOUT_CONDITION_1 * val1, bool bQuery )
{
	//! 60 5
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 60, 5 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DIGITALOUT_CONDITION_1)lval0;
	return 0;
}
//! ENUM,ENUM
//! YO1|YO2,TURNON|CUTOFF
int MRQ::setISOLATOROUTPUT_RESPONSE( MRQ_ISOLATOROUTPUT_STATE val0
,MRQ_ISOLATOROUTPUT_RESPONSE_1 val1 )
{
	//! 60 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 60, 6 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setISOLATOROUTPUT_RESPONSE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! YO1|YO2
int MRQ::getISOLATOROUTPUT_RESPONSE( MRQ_ISOLATOROUTPUT_STATE val0, MRQ_ISOLATOROUTPUT_RESPONSE_1 * val1, bool bQuery )
{
	//! 60 7
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 60, 7 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_ISOLATOROUTPUT_RESPONSE_1)lval0;
	return 0;
}
//! ENUM,ENUM
//! UART1|UART2,4800|7200|9600|14400|19200|38400|57600|115200|128000
int MRQ::setSENSORUART_BAUD( MRQ_SENSORUART_BAUD val0
,MRQ_RS232_BAUD val1 )
{
	//! 61 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 0 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_BAUD( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! UART1|UART2
int MRQ::getSENSORUART_BAUD( MRQ_SENSORUART_BAUD val0, MRQ_RS232_BAUD * val1, bool bQuery )
{
	//! 61 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 1 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_RS232_BAUD)lval0;
	return 0;
}
//! ENUM,ENUM
//! UART1|UART2,8|9
int MRQ::setSENSORUART_WORDLEN( MRQ_SENSORUART_BAUD val0
,MRQ_RS232_WORDLEN val1 )
{
	//! 61 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 2 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_WORDLEN( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! UART1|UART2
int MRQ::getSENSORUART_WORDLEN( MRQ_SENSORUART_BAUD val0, MRQ_RS232_WORDLEN * val1, bool bQuery )
{
	//! 61 3
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 3 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_RS232_WORDLEN)lval0;
	return 0;
}
//! ENUM,ENUM
//! UART1|UART2,NONE|RTS|CTS|RTS&CTS
int MRQ::setSENSORUART_FLOWCTL( MRQ_SENSORUART_BAUD val0
,MRQ_RS232_FLOWCTL val1 )
{
	//! 61 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 4 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_FLOWCTL( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! UART1|UART2
int MRQ::getSENSORUART_FLOWCTL( MRQ_SENSORUART_BAUD val0, MRQ_RS232_FLOWCTL * val1, bool bQuery )
{
	//! 61 5
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 5 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_RS232_FLOWCTL)lval0;
	return 0;
}
//! ENUM,ENUM
//! UART1|UART2,NONE|EVEN|ODD
int MRQ::setSENSORUART_PARITY( MRQ_SENSORUART_BAUD val0
,MRQ_RS232_PARITY val1 )
{
	//! 61 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 6 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_PARITY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! UART1|UART2
int MRQ::getSENSORUART_PARITY( MRQ_SENSORUART_BAUD val0, MRQ_RS232_PARITY * val1, bool bQuery )
{
	//! 61 7
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 7 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_RS232_PARITY)lval0;
	return 0;
}
//! ENUM,ENUM
//! UART1|UART2,1|0.5|2|1.5
int MRQ::setSENSORUART_STOPBIT( MRQ_SENSORUART_BAUD val0
,MRQ_RS232_STOPBIT val1 )
{
	//! 61 8
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 8 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_STOPBIT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! UART1|UART2
int MRQ::getSENSORUART_STOPBIT( MRQ_SENSORUART_BAUD val0, MRQ_RS232_STOPBIT * val1, bool bQuery )
{
	//! 61 9
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 9 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_RS232_STOPBIT)lval0;
	return 0;
}
//! ENUM
//! UART1|UART2
int MRQ::setSENSORUART_APPLYPARA( MRQ_SENSORUART_BAUD val0 )
{
	//! 61 10
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 10 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_APPLYPARA( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM,ENUM,ENUM
//! UART1|UART2,S1|NONE,OFF|ON
int MRQ::setSENSORUART_STATE( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1
,MRQ_SYSTEM_REPORTSWITCH val2 )
{
	//! 61 11
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 11 , (unsigned char)val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_STATE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM,ENUM
//! UART1|UART2,S1|NONE
int MRQ::getSENSORUART_STATE( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1, MRQ_SYSTEM_REPORTSWITCH * val2, bool bQuery )
{
	//! 61 12
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 12 , (byte)val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! ENUM,ENUM,U8
//! UART1|UART2,S1|NONE
int MRQ::setSENSORUART_SOF( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1
,byte val2 )
{
	//! 61 13
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 13 , (unsigned char)val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_SOF( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM,ENUM
//! UART1|UART2,S1|NONE
int MRQ::getSENSORUART_SOF( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1, byte * val2, bool bQuery )
{
	//! 61 14
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 14 , (byte)val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! ENUM,ENUM,U8
//! UART1|UART2,S1|NONE
int MRQ::setSENSORUART_FRAMELEN( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1
,byte val2 )
{
	//! 61 15
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 15 , (unsigned char)val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_FRAMELEN( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM,ENUM
//! UART1|UART2,S1|NONE
int MRQ::getSENSORUART_FRAMELEN( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1, byte * val2, bool bQuery )
{
	//! 61 16
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 16 , (byte)val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! ENUM,ENUM,U8
//! UART1|UART2,S1|NONE
int MRQ::setSENSORUART_RECEIVENUM( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1
,byte val2 )
{
	//! 61 17
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 17 , (unsigned char)val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_RECEIVENUM( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM,ENUM
//! UART1|UART2,S1|NONE
int MRQ::getSENSORUART_RECEIVENUM( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1, byte * val2, bool bQuery )
{
	//! 61 18
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 18 , (byte)val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! ENUM,ENUM,U32
//! UART1|UART2,S1|NONE
int MRQ::setSENSORUART_SWITCHTIME( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1
,uint32 val2 )
{
	//! 61 19
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 19 , (unsigned char)val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_SWITCHTIME( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM,ENUM
//! UART1|UART2,S1|NONE
int MRQ::getSENSORUART_SWITCHTIME( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1, uint32 * val2, bool bQuery )
{
	//! 61 20
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 20 , (byte)val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! ENUM,ENUM
//! UART1|UART2,S1|NONE
int MRQ::getSENSORUART_DATA( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_STATE_1 val1, byte * val2, char * val3, char * val4, char * val5, bool bQuery )
{
	//! 61 21
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
	byte lval3 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 21 , (byte)val0, (byte)val1, &lval0, &lval1, &lval2, &lval3, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	*val3 = (char)lval1;
	*val4 = (char)lval2;
	*val5 = (char)lval3;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],PRESET1|2,END|START|STOP|CLEAR
int MRQ::setPRESET_PVTCONFIG( byte val0
,MRQ_PRESET_PVTCONFIG val1
,MRQ_MOTIONPLAN_PVTCONFIG val2 )
{
	//! 62 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 0 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setPRESET_PVTCONFIG( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],PRESET1|2
int MRQ::getPRESET_PVTCONFIG( byte val0
,MRQ_PRESET_PVTCONFIG val1, MRQ_MOTIONPLAN_PVTCONFIG * val2, bool bQuery )
{
	//! 62 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 1 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_PVTCONFIG)lval0;
	return 0;
}
//! U8,U8,F32
//! [CHANNUM],[INDEX]
int MRQ::setPRESET_POSITION( byte val0
,byte val1
,f32 val2 )
{
	//! 62 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 2 , val0, val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setPRESET_POSITION( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,U8
//! [CHANNUM],[INDEX]
int MRQ::getPRESET_POSITION( byte val0
,byte val1, f32 * val2, bool bQuery )
{
	//! 62 3
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 3 , val0, val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,U8,F32
//! [CHANNUM],[INDEX]
int MRQ::setPRESET_SPEED( byte val0
,byte val1
,f32 val2 )
{
	//! 62 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 4 , val0, val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setPRESET_SPEED( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,U8
//! [CHANNUM],[INDEX]
int MRQ::getPRESET_SPEED( byte val0
,byte val1, f32 * val2, bool bQuery )
{
	//! 62 5
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 5 , val0, val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,U8,F32
//! [CHANNUM],[INDEX]
int MRQ::setPRESET_TIME( byte val0
,byte val1
,f32 val2 )
{
	//! 62 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 6 , val0, val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setPRESET_TIME( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,U8
//! [CHANNUM],[INDEX]
int MRQ::getPRESET_TIME( byte val0
,byte val1, f32 * val2, bool bQuery )
{
	//! 62 7
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 7 , val0, val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],PRESET1|2
int MRQ::getPRESET_REMAINPOINT( byte val0
,MRQ_PRESET_PVTCONFIG val1, byte * val2, bool bQuery )
{
	//! 62 8
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 8 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],PRESET1|2,CLEAR|RUN|PREPARE|CALC|SAVE
int MRQ::setPRESET_EXECUTE( byte val0
,MRQ_PRESET_PVTCONFIG val1
,MRQ_PRESET_EXECUTE_1 val2 )
{
	//! 62 9
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 9 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setPRESET_EXECUTE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],PRESET1|2
int MRQ::getPRESET_STATE( byte val0
,MRQ_PRESET_PVTCONFIG val1, MRQ_SYSTEM_REPORTSWITCH * val2, bool bQuery )
{
	//! 62 10
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 10 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],PRESET1|2,STOP|HOLD
int MRQ::setPRESET_ENDSTATE( byte val0
,MRQ_PRESET_PVTCONFIG val1
,MRQ_MOTIONPLAN_ENDSTATE val2 )
{
	//! 62 11
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 11 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setPRESET_ENDSTATE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],PRESET1|2
int MRQ::getPRESET_ENDSTATE( byte val0
,MRQ_PRESET_PVTCONFIG val1, MRQ_MOTIONPLAN_ENDSTATE * val2, bool bQuery )
{
	//! 62 12
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 12 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_ENDSTATE)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],PRESET1|2,IMMEDIATE|DISTANCE
int MRQ::setPRESET_STOPMODE( byte val0
,MRQ_PRESET_PVTCONFIG val1
,MRQ_STOPDECEL_MODE val2 )
{
	//! 62 13
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 13 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setPRESET_STOPMODE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],PRESET1|2
int MRQ::getPRESET_STOPMODE( byte val0
,MRQ_PRESET_PVTCONFIG val1, MRQ_STOPDECEL_MODE * val2, bool bQuery )
{
	//! 62 14
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 14 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_STOPDECEL_MODE)lval0;
	return 0;
}
//! U8,ENUM,F32
//! [CHANNUM],PRESET1|2
int MRQ::setPRESET_STOPDISTANCE( byte val0
,MRQ_PRESET_PVTCONFIG val1
,f32 val2 )
{
	//! 62 15
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 15 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setPRESET_STOPDISTANCE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],PRESET1|2
int MRQ::getPRESET_STOPDISTANCE( byte val0
,MRQ_PRESET_PVTCONFIG val1, f32 * val2, bool bQuery )
{
	//! 62 16
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 16 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! ENUM
//! OFF|ON
int MRQ::setOTP_STATE( MRQ_SYSTEM_REPORTSWITCH val0 )
{
	//! 70 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 70, 0 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setOTP_STATE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getOTP_STATE(  MRQ_SYSTEM_REPORTSWITCH * val0, bool bQuery )
{
	//! 70 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 70, 1 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! U16
//! 
int MRQ::setOTP_THRESHOLD( uint16 val0 )
{
	//! 70 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 70, 2 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setOTP_THRESHOLD( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getOTP_THRESHOLD(  uint16 * val0, bool bQuery )
{
	//! 70 3
	int ret = 0;

	uint16 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 70, 3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! ENUM
//! NONE|ALARM|STOP|ALARM&STOP
int MRQ::setOTP_RESPONSE( MRQ_OUTOFSTEP_LINERESPONSE val0 )
{
	//! 70 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 70, 4 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setOTP_RESPONSE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getOTP_RESPONSE(  MRQ_OUTOFSTEP_LINERESPONSE * val0, bool bQuery )
{
	//! 70 5
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 70, 5 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_OUTOFSTEP_LINERESPONSE)lval0;
	return 0;
}
//! U32
//! 
int MRQ::setOTP_PERIOD( uint32 val0 )
{
	//! 70 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 70, 6 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setOTP_PERIOD( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getOTP_PERIOD(  uint32 * val0, bool bQuery )
{
	//! 70 7
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 70, 7 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! 
//! 
int MRQ::getOTP_DATA(  uint32 * val0, bool bQuery )
{
	//! 70 8
	int ret = 0;

	uint32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 70, 8 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! ENUM
//! OFF|ON
int MRQ::setANALOGIN_STATE( MRQ_SYSTEM_REPORTSWITCH val0 )
{
	//! 71 0
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 71, 0 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setANALOGIN_STATE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getANALOGIN_STATE(  MRQ_SYSTEM_REPORTSWITCH * val0, bool bQuery )
{
	//! 71 1
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 71, 1 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_SYSTEM_REPORTSWITCH)lval0;
	return 0;
}
//! F32
//! 
int MRQ::setANALOGIN_THRESHOLDH( f32 val0 )
{
	//! 71 2
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 71, 2 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setANALOGIN_THRESHOLDH( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getANALOGIN_THRESHOLDH(  f32 * val0, bool bQuery )
{
	//! 71 3
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 71, 3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! F32
//! 
int MRQ::setANALOGIN_THRESHOLDL( f32 val0 )
{
	//! 71 4
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 71, 4 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setANALOGIN_THRESHOLDL( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getANALOGIN_THRESHOLDL(  f32 * val0, bool bQuery )
{
	//! 71 5
	int ret = 0;

	f32 lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 71, 5 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! ENUM
//! NONE|ALARM|STOP|ALARM&STOP
int MRQ::setANALOGIN_RESPONSEH( MRQ_OUTOFSTEP_LINERESPONSE val0 )
{
	//! 71 6
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 71, 6 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setANALOGIN_RESPONSEH( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getANALOGIN_RESPONSEH(  MRQ_OUTOFSTEP_LINERESPONSE * val0, bool bQuery )
{
	//! 71 7
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 71, 7 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_OUTOFSTEP_LINERESPONSE)lval0;
	return 0;
}
//! ENUM
//! NONE|ALARM|STOP|ALARM&STOP
int MRQ::setANALOGIN_RESPONSEL( MRQ_OUTOFSTEP_LINERESPONSE val0 )
{
	//! 71 8
	int ret; 
        ret = m_pBus->write( DEVICE_RECEIVE_ID, 71, 8 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setANALOGIN_RESPONSEL( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getANALOGIN_RESPONSEL(  MRQ_OUTOFSTEP_LINERESPONSE * val0, bool bQuery )
{
	//! 71 9
	int ret = 0;

	byte lval0 = 0;
        ret = m_pBus->read( DEVICE_RECEIVE_ID, 71, 9 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_OUTOFSTEP_LINERESPONSE)lval0;
	return 0;
}
}
