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
//! 
//! 
int MRQ::getLINK_DEVICEINFO(  MRQ_LINK_DEVICEINFO * val0, MRQ_LINK_DEVICEINFO_1 * val1, uint32 * val2, bool bQuery )
{
	//! 1 2
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	uint32 lval2 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 1, 2 , &lval0, &lval1, &lval2, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_LINK_DEVICEINFO)lval0;
	*val1 = (MRQ_LINK_DEVICEINFO_1)lval1;
	*val2 = lval2;
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
int MRQ::getSYSTEM_TYPE(  MRQ_LINK_DEVICEINFO * val0, MRQ_LINK_DEVICEINFO_1 * val1, bool bQuery )
{
	//! 2 5
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 2, 5 , &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_LINK_DEVICEINFO)lval0;
	*val1 = (MRQ_LINK_DEVICEINFO_1)lval1;
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
int MRQ::getSYSTEM_EVENTCODE(  char * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery )
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
//! 
//! 
int MRQ::getSYSTEM_DIOSTATE(  uint16 * val0, bool bQuery )
{
	//! 2 11
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 2, 11 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
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
int MRQ::setCAN_NETMANAGELED( MRQ_CAN_NETMANAGELED val0 )
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
int MRQ::setCLOCK_SYNCREGISTER( MRQ_CAN_NETMANAGELED val0 )
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
int MRQ::getCLOCK_SYNCREGISTER(  MRQ_CAN_NETMANAGELED * val0, bool bQuery )
{
	//! 5 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 5, 3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_CAN_NETMANAGELED)lval0;
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
//! U8,ENUM,ENUM
//! [CHANNUM],RESET|STOP|RUN|PREPARE|EMERGSTOP,MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::setMOTION_SWITCH( byte val0
,MRQ_MOTION_SWITCH val1
,MRQ_MOTION_SWITCH_1 val2 )
{
	//! 21 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 0 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_SWITCH( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTION_STATE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTION_STATE_2 * val2, bool bQuery )
{
	//! 21 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 1 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTION_STATE_2)lval0;
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
//! [CHANNUM],OFF|ON
int MRQ::setMOTION_OFFSETSTATE( byte val0
,MRQ_CAN_NETMANAGELED val1 )
{
	//! 21 27
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 27 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_OFFSETSTATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_OFFSETSTATE( byte val0, MRQ_CAN_NETMANAGELED * val1, bool bQuery )
{
	//! 21 28
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 28 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_CAN_NETMANAGELED)lval0;
	return 0;
}
//! U8,S32
//! [CHANNUM]
int MRQ::setMOTION_OFFSET( byte val0
,int32 val1 )
{
	//! 21 29
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 29 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_OFFSET( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_COUNTCIRCLE( byte val0, int16 * val1, bool bQuery )
{
	//! 21 30
	int ret = 0;

	int16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 30 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_ABCOUNT( byte val0, int32 * val1, bool bQuery )
{
	//! 21 31
	int ret = 0;

	int32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 31 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int MRQ::setMOTION_REVMOTION( byte val0
,MRQ_CAN_NETMANAGELED val1 )
{
	//! 21 32
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 21, 32 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTION_REVMOTION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getMOTION_REVMOTION( byte val0, MRQ_CAN_NETMANAGELED * val1, bool bQuery )
{
	//! 21 33
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 21, 33 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_CAN_NETMANAGELED)lval0;
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
int MRQ::setIDENTITY_DISTDEVICE( MRQ_CAN_NETMANAGELED val0 )
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
int MRQ::getIDENTITY_DISTDEVICE(  MRQ_CAN_NETMANAGELED * val0, bool bQuery )
{
	//! 22 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 22, 3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_CAN_NETMANAGELED)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],SINANJU|MEGATRON|GOUF,S1|S2|S3|S4
int MRQ::setIDENTITY_LABEL( byte val0
,MRQ_IDENTITY_LABEL val1
,MRQ_IDENTITY_LABEL_1 val2 )
{
	//! 22 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 22, 4 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setIDENTITY_LABEL( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getIDENTITY_LABEL( byte val0, MRQ_IDENTITY_LABEL * val1, MRQ_IDENTITY_LABEL_1 * val2, bool bQuery )
{
	//! 22 5
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 22, 5 , val0, &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_IDENTITY_LABEL)lval0;
	*val2 = (MRQ_IDENTITY_LABEL_1)lval1;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],1.8|0.9|15|7.5
int MRQ::setMOTOR_STEPANGLE( byte val0
,MRQ_MOTOR_STEPANGLE val1 )
{
	//! 51 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 0 , val0, (unsigned char)val1 );

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
	//! 51 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 1 , val0, &lval0, bQuery);
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
	//! 51 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 2 , val0, (unsigned char)val1 );

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
	//! 51 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 3 , val0, &lval0, bQuery);
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
	//! 51 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 4 , val0, (unsigned char)val1 );

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
	//! 51 5
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 5 , val0, &lval0, bQuery);
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
	//! 51 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 6 , val0, val1 );

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
	//! 51 7
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 7 , val0, &lval0, bQuery);
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
	//! 51 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 8 , val0, val1 );

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
	//! 51 9
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 9 , val0, &lval0, bQuery);
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
	//! 51 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 10 , val0, val1 );

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
	//! 51 11
	int ret = 0;

	f32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 11 , val0, &lval0, bQuery);
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
	//! 51 12
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 12 , val0, val1 );

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
	//! 51 13
	int ret = 0;

	f32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 13 , val0, &lval0, bQuery);
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
	//! 51 14
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 14 , val0, val1 );

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
	//! 51 15
	int ret = 0;

	f32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 15 , val0, &lval0, bQuery);
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
	//! 51 16
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 16 , val0, (unsigned char)val1 );

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
	//! 51 17
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 17 , val0, &lval0, bQuery);
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
	//! 51 18
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 18 , val0, val1 );

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
	//! 51 19
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 19 , val0, &lval0, bQuery);
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
	//! 51 20
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 20 , val0, val1 );

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
	//! 51 21
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 21 , val0, &lval0, bQuery);
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
	//! 51 22
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 51, 22 , val0, val1 );

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
	//! 51 23
	int ret = 0;

	f32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 51, 23 , val0, &lval0, bQuery);
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
	//! 52 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 0 , val0, val1 );

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
	//! 52 1
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 52, 1 , val0, &lval0, bQuery);
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
	//! 52 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 2 , val0, (unsigned char)val1 );

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
	//! 52 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 52, 3 , val0, &lval0, bQuery);
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
	//! 52 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 4 , val0, (unsigned char)val1 );

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
	//! 52 5
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 52, 5 , val0, &lval0, bQuery);
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
	//! 52 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 6 , val0, (unsigned char)val1 );

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
	//! 52 7
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 52, 7 , val0, &lval0, bQuery);
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
	//! 52 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 8 , val0, (unsigned char)val1 );

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
	//! 52 9
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 52, 9 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_ENCODER_STATE)lval0;
	return 0;
}
//! U8,U8
//! [CHANNUM]
int MRQ::setENCODER_FEEDBACKRATIO( byte val0
,byte val1 )
{
	//! 52 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 52, 10 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setENCODER_FEEDBACKRATIO( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getENCODER_FEEDBACKRATIO( byte val0, byte * val1, bool bQuery )
{
	//! 52 11
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 52, 11 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,END|CLEAR
int MRQ::setMOTIONPLAN_PVTCONFIG( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_PVTCONFIG_1 val2 )
{
	//! 53 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 0 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_PVTCONFIG( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_PVTCONFIG( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_PVTCONFIG_1 * val2, bool bQuery )
{
	//! 53 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 1 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_PVTCONFIG_1)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,OFF|ON|CALC
int MRQ::setMOTIONPLAN_PRESETSTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_PRESETSTATE_1 val2 )
{
	//! 53 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 2 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_PRESETSTATE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_PRESETSTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_PRESETSTATE_1 * val2, bool bQuery )
{
	//! 53 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 3 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_PRESETSTATE_1)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,CYCLE|FIFO
int MRQ::setMOTIONPLAN_EXECUTEMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_EXECUTEMODE_1 val2 )
{
	//! 53 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 4 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_EXECUTEMODE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_EXECUTEMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_EXECUTEMODE_1 * val2, bool bQuery )
{
	//! 53 5
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 5 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_EXECUTEMODE_1)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,CUBICPOLY|LINEAR|UNIFORM|TRAPEZOID
int MRQ::setMOTIONPLAN_PLANMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_PLANMODE_1 val2 )
{
	//! 53 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 6 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_PLANMODE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_PLANMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_PLANMODE_1 * val2, bool bQuery )
{
	//! 53 7
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 7 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_PLANMODE_1)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,PVT|LVT_CORRECT|LVT_NOCORRECT
int MRQ::setMOTIONPLAN_MOTIONMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_MOTIONMODE_1 val2 )
{
	//! 53 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 8 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_MOTIONMODE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_MOTIONMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_MOTIONMODE_1 * val2, bool bQuery )
{
	//! 53 9
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 9 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_MOTIONMODE_1)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,1/4|1/8|1/16|1/32
int MRQ::setMOTIONPLAN_MODIFYDUTY( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_MODIFYDUTY_1 val2 )
{
	//! 53 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 10 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_MODIFYDUTY( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_MODIFYDUTY( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_MODIFYDUTY_1 * val2, bool bQuery )
{
	//! 53 11
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 11 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_MODIFYDUTY_1)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_REMAINPOINT( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint16 * val2, bool bQuery )
{
	//! 53 12
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 12 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,U32
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::setMOTIONPLAN_CYCLENUM( byte val0
,MRQ_MOTION_SWITCH_1 val1
,uint32 val2 )
{
	//! 53 13
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 13 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_CYCLENUM( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_CYCLENUM( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint32 * val2, bool bQuery )
{
	//! 53 14
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 14 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,U8
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::setMOTIONPLAN_WARNPOINT( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2 )
{
	//! 53 15
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 15 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_WARNPOINT( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_WARNPOINT( byte val0
,MRQ_MOTION_SWITCH_1 val1, byte * val2, bool bQuery )
{
	//! 53 16
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 16 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,STOP|HOLD
int MRQ::setMOTIONPLAN_ENDSTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_ENDSTATE_1 val2 )
{
	//! 53 17
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 17 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_ENDSTATE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_ENDSTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_ENDSTATE_1 * val2, bool bQuery )
{
	//! 53 18
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 18 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_ENDSTATE_1)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_BUFFERSIZE( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint32 * val2, bool bQuery )
{
	//! 53 19
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 19 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,U16
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::setMOTIONPLAN_ACCSCALE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,uint16 val2 )
{
	//! 53 20
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 20 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_ACCSCALE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_ACCSCALE( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint16 * val2, bool bQuery )
{
	//! 53 21
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 21 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,U16
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::setMOTIONPLAN_DECSCALE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,uint16 val2 )
{
	//! 53 22
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 22 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_DECSCALE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_DECSCALE( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint16 * val2, bool bQuery )
{
	//! 53 23
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 23 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,IMMEDIATE|DISTANCE
int MRQ::setMOTIONPLAN_STOPMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_STOPMODE_1 val2 )
{
	//! 53 24
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 24 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_STOPMODE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_STOPMODE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_STOPMODE_1 * val2, bool bQuery )
{
	//! 53 25
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 25 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_STOPMODE_1)lval0;
	return 0;
}
//! U8,ENUM,F32
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::setMOTIONPLAN_STOPDISTANCE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,f32 val2 )
{
	//! 53 26
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 26 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_STOPDISTANCE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_STOPDISTANCE( byte val0
,MRQ_MOTION_SWITCH_1 val1, f32 * val2, bool bQuery )
{
	//! 53 27
	int ret = 0;

	f32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 27 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,F32
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::setMOTIONPLAN_STOPTIME( byte val0
,MRQ_MOTION_SWITCH_1 val1
,f32 val2 )
{
	//! 53 28
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 28 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_STOPTIME( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_STOPTIME( byte val0
,MRQ_MOTION_SWITCH_1 val1, f32 * val2, bool bQuery )
{
	//! 53 29
	int ret = 0;

	f32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 29 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,OFF|ON
int MRQ::setMOTIONPLAN_OOSLINESTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_CAN_NETMANAGELED val2 )
{
	//! 53 30
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 30 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_OOSLINESTATE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_OOSLINESTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_CAN_NETMANAGELED * val2, bool bQuery )
{
	//! 53 31
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 31 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_CAN_NETMANAGELED)lval0;
	return 0;
}
//! U8,ENUM,U16
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::setMOTIONPLAN_OOSLINEOUTNUM( byte val0
,MRQ_MOTION_SWITCH_1 val1
,uint16 val2 )
{
	//! 53 32
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 32 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_OOSLINEOUTNUM( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_OOSLINEOUTNUM( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint16 * val2, bool bQuery )
{
	//! 53 33
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 33 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,NONE|ALARM|STOP|ALARM&STOP
int MRQ::setMOTIONPLAN_OOSLINERESPONSE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val2 )
{
	//! 53 34
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 34 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_OOSLINERESPONSE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_OOSLINERESPONSE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val2, bool bQuery )
{
	//! 53 35
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 35 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,OFF|ON
int MRQ::setMOTIONPLAN_OOSTOTALSTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_CAN_NETMANAGELED val2 )
{
	//! 53 36
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 36 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_OOSTOTALSTATE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_OOSTOTALSTATE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_CAN_NETMANAGELED * val2, bool bQuery )
{
	//! 53 37
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 37 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_CAN_NETMANAGELED)lval0;
	return 0;
}
//! U8,ENUM,U32
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::setMOTIONPLAN_OOSTOTALOUTNUM( byte val0
,MRQ_MOTION_SWITCH_1 val1
,uint32 val2 )
{
	//! 53 38
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 38 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_OOSTOTALOUTNUM( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_OOSTOTALOUTNUM( byte val0
,MRQ_MOTION_SWITCH_1 val1, uint32 * val2, bool bQuery )
{
	//! 53 39
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 39 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,NONE|ALARM|STOP|ALARM&STOP
int MRQ::setMOTIONPLAN_OOSTOTALRESPONSE( byte val0
,MRQ_MOTION_SWITCH_1 val1
,MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val2 )
{
	//! 53 40
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 53, 40 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setMOTIONPLAN_OOSTOTALRESPONSE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
int MRQ::getMOTIONPLAN_OOSTOTALRESPONSE( byte val0
,MRQ_MOTION_SWITCH_1 val1, MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val2, bool bQuery )
{
	//! 53 41
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 53, 41 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)lval0;
	return 0;
}
//! U8,ENUM,U8,F32
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ::setPOSITION( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2
,f32 val3 )
{
	//! 54 -1
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 54 , val0, (unsigned char)val1, val2, val3 );

	if ( ret == 0) 
	{
		MRQ_model::setPOSITION( val0, val1, val2, val3 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM,U8
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ::getPOSITION( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2, f32 * val3, bool bQuery )
{
	//! 55 -1
	int ret = 0;

	f32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 55  , val0, (byte)val1, val2, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val3 = lval0;
	return 0;
}
//! U8,ENUM,U8,F32
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ::setVELOCITY( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2
,f32 val3 )
{
	//! 56 -1
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 56 , val0, (unsigned char)val1, val2, val3 );

	if ( ret == 0) 
	{
		MRQ_model::setVELOCITY( val0, val1, val2, val3 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM,U8
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ::getVELOCITY( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2, f32 * val3, bool bQuery )
{
	//! 57 -1
	int ret = 0;

	f32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 57  , val0, (byte)val1, val2, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val3 = lval0;
	return 0;
}
//! U8,ENUM,U8,F32
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ::setTIME( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2
,f32 val3 )
{
	//! 58 -1
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 58 , val0, (unsigned char)val1, val2, val3 );

	if ( ret == 0) 
	{
		MRQ_model::setTIME( val0, val1, val2, val3 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM,U8
//! [CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
int MRQ::getTIME( byte val0
,MRQ_MOTION_SWITCH_1 val1
,byte val2, f32 * val3, bool bQuery )
{
	//! 59 -1
	int ret = 0;

	f32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 59  , val0, (byte)val1, val2, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val3 = lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],TORQUE|CYCLE|XANGLE|YANGLE|ZANGLE|XVEL|YVEL|ZVEL|XACC|YACC|ZACC|SGALL|SGSE|DIST|ABSENC,OFF|ON
int MRQ::setREPORT_STATE( byte val0
,MRQ_REPORT_STATE val1
,MRQ_CAN_NETMANAGELED val2 )
{
	//! 60 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 60, 0 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setREPORT_STATE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TORQUE|CYCLE|XANGLE|YANGLE|ZANGLE|XVEL|YVEL|ZVEL|XACC|YACC|ZACC|SGALL|SGSE|DIST|ABSENC
int MRQ::getREPORT_STATE( byte val0
,MRQ_REPORT_STATE val1, MRQ_CAN_NETMANAGELED * val2, bool bQuery )
{
	//! 60 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 60, 1 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_CAN_NETMANAGELED)lval0;
	return 0;
}
//! U8,ENUM,U32
//! [CHANNUM],TORQUE|CYCLE|XANGLE|YANGLE|ZANGLE|XVEL|YVEL|ZVEL|XACC|YACC|ZACC|SGALL|SGSE|DIST|ABSENC
int MRQ::setREPORT_PERIOD( byte val0
,MRQ_REPORT_STATE val1
,uint32 val2 )
{
	//! 60 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 60, 2 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setREPORT_PERIOD( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TORQUE|CYCLE|XANGLE|YANGLE|ZANGLE|XVEL|YVEL|ZVEL|XACC|YACC|ZACC|SGALL|SGSE|DIST|ABSENC
int MRQ::getREPORT_PERIOD( byte val0
,MRQ_REPORT_STATE val1, uint32 * val2, bool bQuery )
{
	//! 60 3
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 60, 3 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],TORQUE|CYCLE|XANGLE|YANGLE|ZANGLE|XVEL|YVEL|ZVEL|XACC|YACC|ZACC|SGALL|SGSE|DIST|ABSENC
int MRQ::getREPORT_DATA( byte val0
,MRQ_REPORT_STATE val1, uint32 * val2, bool bQuery )
{
	//! 60 4
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 60, 4 , val0, (byte)val1, &lval0, bQuery);
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
	//! 61 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 0 , val0, (unsigned char)val1 );

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
	//! 61 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_TRIGGER_MODE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int MRQ::setTRIGGER_PATTSTATE( byte val0
,MRQ_CAN_NETMANAGELED val1 )
{
	//! 61 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 2 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_PATTSTATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getTRIGGER_PATTSTATE( byte val0, MRQ_CAN_NETMANAGELED * val1, bool bQuery )
{
	//! 61 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_CAN_NETMANAGELED)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],X|L|R|F|H,X|L|R|F|H
int MRQ::setTRIGGER_PATTERN( byte val0
,MRQ_TRIGGER_PATTERN val1
,MRQ_TRIGGER_PATTERN val2 )
{
	//! 61 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 4 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_PATTERN( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getTRIGGER_PATTERN( byte val0, MRQ_TRIGGER_PATTERN * val1, MRQ_TRIGGER_PATTERN * val2, bool bQuery )
{
	//! 61 5
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 5 , val0, &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_TRIGGER_PATTERN)lval0;
	*val2 = (MRQ_TRIGGER_PATTERN)lval1;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|ALARM|STOP|ALARM&STOP
int MRQ::setTRIGGER_PATTRESP( byte val0
,MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val1 )
{
	//! 61 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 6 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_PATTRESP( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getTRIGGER_PATTRESP( byte val0, MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val1, bool bQuery )
{
	//! 61 7
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],CONTINUED|INTERVAL|SINGLE
int MRQ::setTRIGGER_PATTSMODE( byte val0
,MRQ_TRIGGER_PATTSMODE val1 )
{
	//! 61 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 8 , val0, (unsigned char)val1 );

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
	//! 61 9
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 9 , val0, &lval0, bQuery);
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
	//! 61 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 10 , val0, val1 );

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
	//! 61 11
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 11 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],TRIGL|TRIGR,OFF|ON
int MRQ::setTRIGGER_LEVELSTATE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1
,MRQ_CAN_NETMANAGELED val2 )
{
	//! 61 12
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 12 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_LEVELSTATE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TRIGL|TRIGR
int MRQ::getTRIGGER_LEVELSTATE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, MRQ_CAN_NETMANAGELED * val2, bool bQuery )
{
	//! 61 13
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 13 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_CAN_NETMANAGELED)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],TRIGL|TRIGR,RESERVE|LOW|RISE|FALL|HIGH
int MRQ::setTRIGGER_LEVELTYPE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1
,MRQ_TRIGGER_LEVELTYPE_1 val2 )
{
	//! 61 14
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 14 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_LEVELTYPE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TRIGL|TRIGR
int MRQ::getTRIGGER_LEVELTYPE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, MRQ_TRIGGER_LEVELTYPE_1 * val2, bool bQuery )
{
	//! 61 15
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 15 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_TRIGGER_LEVELTYPE_1)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],TRIGL|TRIGR,NONE|ALARM|STOP|ALARM&STOP
int MRQ::setTRIGGER_LEVELRESP( byte val0
,MRQ_TRIGGER_LEVELSTATE val1
,MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val2 )
{
	//! 61 16
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 16 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_LEVELRESP( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TRIGL|TRIGR
int MRQ::getTRIGGER_LEVELRESP( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val2, bool bQuery )
{
	//! 61 17
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 17 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],TRIGL|TRIGR,CONTINUED|INTERVAL|SINGLE
int MRQ::setTRIGGER_LEVELSMODE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1
,MRQ_TRIGGER_PATTSMODE val2 )
{
	//! 61 18
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 18 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_LEVELSMODE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TRIGL|TRIGR
int MRQ::getTRIGGER_LEVELSMODE( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, MRQ_TRIGGER_PATTSMODE * val2, bool bQuery )
{
	//! 61 19
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 19 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_TRIGGER_PATTSMODE)lval0;
	return 0;
}
//! U8,ENUM,F32
//! [CHANNUM],TRIGL|TRIGR
int MRQ::setTRIGGER_LEVELSPERIOD( byte val0
,MRQ_TRIGGER_LEVELSTATE val1
,f32 val2 )
{
	//! 61 20
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 61, 20 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setTRIGGER_LEVELSPERIOD( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],TRIGL|TRIGR
int MRQ::getTRIGGER_LEVELSPERIOD( byte val0
,MRQ_TRIGGER_LEVELSTATE val1, f32 * val2, bool bQuery )
{
	//! 61 21
	int ret = 0;

	f32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 61, 21 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::getDRIVER_TYPE( byte val0, MRQ_DRIVER_TYPE * val1, bool bQuery )
{
	//! 62 0
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 0 , val0, &lval0, bQuery);
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
	//! 62 1
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 1 , val0, (byte)val1, &lval0, bQuery);
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
	//! 62 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 2 , val0, val1 );

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
	//! 62 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 3 , val0, &lval0, bQuery);
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
	//! 62 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 4 , val0, (unsigned char)val1 );

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
	//! 62 5
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 5 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DRIVER_MICROSTEPS)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int MRQ::setDRIVER_STATE( byte val0
,MRQ_CAN_NETMANAGELED val1 )
{
	//! 62 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 6 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDRIVER_STATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getDRIVER_STATE( byte val0, MRQ_CAN_NETMANAGELED * val1, bool bQuery )
{
	//! 62 7
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_CAN_NETMANAGELED)lval0;
	return 0;
}
//! U8,ENUM,S8
//! [CHANNUM],UPLIMIT|DOWNLIMIT
int MRQ::setDRIVER_SGLIMIT( byte val0
,MRQ_DRIVER_SGLIMIT val1
,int8 val2 )
{
	//! 62 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 8 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setDRIVER_SGLIMIT( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM],UPLIMIT|DOWNLIMIT
int MRQ::getDRIVER_SGLIMIT( byte val0, MRQ_DRIVER_SGLIMIT * val1, int8 * val2, bool bQuery )
{
	//! 62 9
	int ret = 0;

	byte lval0 = 0;
	int8 lval1 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 9 , val0, &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DRIVER_SGLIMIT)lval0;
	*val2 = lval1;
	return 0;
}
//! U8,ENUM,S16
//! [CHANNUM],SG0|SGT|SEMAX|SEMIN
int MRQ::setDRIVER_SGPARASET( byte val0
,MRQ_DRIVER_SGPARASET val1
,int16 val2 )
{
	//! 62 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 10 , val0, (unsigned char)val1, val2 );

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
	//! 62 11
	int ret = 0;

	int16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 11 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,U8
//! [CHANNUM]
int MRQ::setDRIVER_IDLECURRENT( byte val0
,byte val1 )
{
	//! 62 12
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 12 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDRIVER_IDLECURRENT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getDRIVER_IDLECURRENT( byte val0, byte * val1, bool bQuery )
{
	//! 62 13
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 13 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRQ::setDRIVER_SWITCHTIME( byte val0
,uint32 val1 )
{
	//! 62 14
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 14 , val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDRIVER_SWITCHTIME( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getDRIVER_SWITCHTIME( byte val0, uint32 * val1, bool bQuery )
{
	//! 62 15
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 15 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],1/2|1/4
int MRQ::setDRIVER_MINICURRRATIO( byte val0
,MRQ_DRIVER_MINICURRRATIO val1 )
{
	//! 62 16
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 16 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDRIVER_MINICURRRATIO( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRQ::getDRIVER_MINICURRRATIO( byte val0, MRQ_DRIVER_MINICURRRATIO * val1, bool bQuery )
{
	//! 62 17
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 17 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DRIVER_MINICURRRATIO)lval0;
	return 0;
}
//! U8,ENUM,U32
//! [CHANNUM],DRVCTRL|CHOPCONF|SMARTEN|SGCSCONF|DRVCONF
int MRQ::setDRIVER_REGCONFIG( byte val0
,MRQ_DRIVER_REGCONFIG val1
,uint32 val2 )
{
	//! 62 18
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 62, 18 , val0, (unsigned char)val1, val2 );

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
	//! 62 19
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 62, 19 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! ENUM,ENUM
//! DO1|DO2|DO3|DO4,DISABLE|ENABLE
int MRQ::setDIGITALOUTPUT_STATE( MRQ_DIGITALOUTPUT_STATE val0
,MRQ_DIGITALOUTPUT_STATE_1 val1 )
{
	//! 63 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 63, 0 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUTPUT_STATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2|DO3|DO4
int MRQ::getDIGITALOUTPUT_STATE( MRQ_DIGITALOUTPUT_STATE val0, MRQ_DIGITALOUTPUT_STATE_1 * val1, bool bQuery )
{
	//! 63 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 63, 1 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DIGITALOUTPUT_STATE_1)lval0;
	return 0;
}
//! ENUM,ENUM
//! DO1|DO2|DO3|DO4,LEVEL|NONE
int MRQ::setDIGITALOUTPUT_SIGNAL( MRQ_DIGITALOUTPUT_STATE val0
,MRQ_DIGITALOUTPUT_SIGNAL_1 val1 )
{
	//! 63 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 63, 2 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUTPUT_SIGNAL( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2|DO3|DO4
int MRQ::getDIGITALOUTPUT_SIGNAL( MRQ_DIGITALOUTPUT_STATE val0, MRQ_DIGITALOUTPUT_SIGNAL_1 * val1, bool bQuery )
{
	//! 63 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 63, 3 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DIGITALOUTPUT_SIGNAL_1)lval0;
	return 0;
}
//! ENUM,ENUM
//! DO1|DO2|DO3|DO4,POSITIVE|NEGATIVE
int MRQ::setDIGITALOUTPUT_POLARITY( MRQ_DIGITALOUTPUT_STATE val0
,MRQ_DIGITALOUTPUT_POLARITY_1 val1 )
{
	//! 63 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 63, 4 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUTPUT_POLARITY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2|DO3|DO4
int MRQ::getDIGITALOUTPUT_POLARITY( MRQ_DIGITALOUTPUT_STATE val0, MRQ_DIGITALOUTPUT_POLARITY_1 * val1, bool bQuery )
{
	//! 63 5
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 63, 5 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DIGITALOUTPUT_POLARITY_1)lval0;
	return 0;
}
//! ENUM,U8
//! DO1|DO2|DO3|DO4,[CHANNUM]
int MRQ::setDIGITALOUTPUT_SOURCE( MRQ_DIGITALOUTPUT_STATE val0
,byte val1 )
{
	//! 63 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 63, 6 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUTPUT_SOURCE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2|DO3|DO4
int MRQ::getDIGITALOUTPUT_SOURCE( MRQ_DIGITALOUTPUT_STATE val0, byte * val1, bool bQuery )
{
	//! 63 7
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 63, 7 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,ENUM
//! DO1|DO2|DO3|DO4,AUTO|NONE
int MRQ::setDIGITALOUTPUT_CONDITION( MRQ_DIGITALOUTPUT_STATE val0
,MRQ_DIGITALOUTPUT_CONDITION_1 val1 )
{
	//! 63 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 63, 8 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUTPUT_CONDITION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2|DO3|DO4
int MRQ::getDIGITALOUTPUT_CONDITION( MRQ_DIGITALOUTPUT_STATE val0, MRQ_DIGITALOUTPUT_CONDITION_1 * val1, bool bQuery )
{
	//! 63 9
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 63, 9 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DIGITALOUTPUT_CONDITION_1)lval0;
	return 0;
}
//! ENUM,U32
//! DO1|DO2|DO3|DO4
int MRQ::setDIGITALOUTPUT_PERIOD( MRQ_DIGITALOUTPUT_STATE val0
,uint32 val1 )
{
	//! 63 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 63, 10 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUTPUT_PERIOD( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2|DO3|DO4
int MRQ::getDIGITALOUTPUT_PERIOD( MRQ_DIGITALOUTPUT_STATE val0, uint32 * val1, bool bQuery )
{
	//! 63 11
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 63, 11 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,U16
//! DO1|DO2|DO3|DO4
int MRQ::setDIGITALOUTPUT_DUTY( MRQ_DIGITALOUTPUT_STATE val0
,uint16 val1 )
{
	//! 63 12
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 63, 12 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDIGITALOUTPUT_DUTY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! DO1|DO2|DO3|DO4
int MRQ::getDIGITALOUTPUT_DUTY( MRQ_DIGITALOUTPUT_STATE val0, uint16 * val1, bool bQuery )
{
	//! 63 13
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 63, 13 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,ENUM
//! YO1|YO2,DISABLE|ENABLE
int MRQ::setISOLATOROUTPUT_STATE( MRQ_ISOLATOROUTPUT_STATE val0
,MRQ_DIGITALOUTPUT_STATE_1 val1 )
{
	//! 64 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 64, 0 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setISOLATOROUTPUT_STATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! YO1|YO2
int MRQ::getISOLATOROUTPUT_STATE( MRQ_ISOLATOROUTPUT_STATE val0, MRQ_DIGITALOUTPUT_STATE_1 * val1, bool bQuery )
{
	//! 64 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 64, 1 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DIGITALOUTPUT_STATE_1)lval0;
	return 0;
}
//! ENUM,U8
//! YO1|YO2,[CHANNUM]
int MRQ::setISOLATOROUTPUT_SOURCE( MRQ_ISOLATOROUTPUT_STATE val0
,byte val1 )
{
	//! 64 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 64, 2 , (unsigned char)val0, val1 );

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
	//! 64 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 64, 3 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,ENUM
//! YO1|YO2,AUTO|NONE
int MRQ::setISOLATOROUTPUT_CONDITION( MRQ_ISOLATOROUTPUT_STATE val0
,MRQ_DIGITALOUTPUT_CONDITION_1 val1 )
{
	//! 64 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 64, 4 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setISOLATOROUTPUT_CONDITION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! YO1|YO2
int MRQ::getISOLATOROUTPUT_CONDITION( MRQ_ISOLATOROUTPUT_STATE val0, MRQ_DIGITALOUTPUT_CONDITION_1 * val1, bool bQuery )
{
	//! 64 5
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 64, 5 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_DIGITALOUTPUT_CONDITION_1)lval0;
	return 0;
}
//! ENUM,ENUM
//! YO1|YO2,TURNON|CUTOFF
int MRQ::setISOLATOROUTPUT_RESPONSE( MRQ_ISOLATOROUTPUT_STATE val0
,MRQ_ISOLATOROUTPUT_RESPONSE_1 val1 )
{
	//! 64 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 64, 6 , (unsigned char)val0, (unsigned char)val1 );

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
	//! 64 7
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 64, 7 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_ISOLATOROUTPUT_RESPONSE_1)lval0;
	return 0;
}
//! ENUM,ENUM
//! UART1|UART2,4800|7200|9600|14400|19200|38400|57600|115200|128000|2000000|2500000
int MRQ::setSENSORUART_BAUD( MRQ_SENSORUART_BAUD val0
,MRQ_SENSORUART_BAUD_1 val1 )
{
	//! 65 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 65, 0 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_BAUD( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! UART1|UART2
int MRQ::getSENSORUART_BAUD( MRQ_SENSORUART_BAUD val0, MRQ_SENSORUART_BAUD_1 * val1, bool bQuery )
{
	//! 65 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 65, 1 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_SENSORUART_BAUD_1)lval0;
	return 0;
}
//! ENUM,ENUM
//! UART1|UART2,8|9
int MRQ::setSENSORUART_WORDLEN( MRQ_SENSORUART_BAUD val0
,MRQ_RS232_WORDLEN val1 )
{
	//! 65 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 65, 2 , (unsigned char)val0, (unsigned char)val1 );

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
	//! 65 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 65, 3 , (byte)val0, &lval0, bQuery);
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
	//! 65 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 65, 4 , (unsigned char)val0, (unsigned char)val1 );

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
	//! 65 5
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 65, 5 , (byte)val0, &lval0, bQuery);
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
	//! 65 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 65, 6 , (unsigned char)val0, (unsigned char)val1 );

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
	//! 65 7
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 65, 7 , (byte)val0, &lval0, bQuery);
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
	//! 65 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 65, 8 , (unsigned char)val0, (unsigned char)val1 );

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
	//! 65 9
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 65, 9 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_RS232_STOPBIT)lval0;
	return 0;
}
//! ENUM
//! UART1|UART2
int MRQ::setSENSORUART_APPLYPARA( MRQ_SENSORUART_BAUD val0 )
{
	//! 65 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 65, 10 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_APPLYPARA( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM,ENUM,ENUM
//! UART1|UART2,S1|S2|S3|S4,OFF|ON
int MRQ::setSENSORUART_STATE( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1
,MRQ_CAN_NETMANAGELED val2 )
{
	//! 65 11
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 65, 11 , (unsigned char)val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_STATE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM,ENUM
//! UART1|UART2,S1|S2|S3|S4
int MRQ::getSENSORUART_STATE( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1, MRQ_CAN_NETMANAGELED * val2, bool bQuery )
{
	//! 65 12
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 65, 12 , (byte)val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRQ_CAN_NETMANAGELED)lval0;
	return 0;
}
//! ENUM,ENUM,U8
//! UART1|UART2,S1|S2|S3|S4
int MRQ::setSENSORUART_SOF( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1
,byte val2 )
{
	//! 65 13
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 65, 13 , (unsigned char)val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_SOF( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM,ENUM
//! UART1|UART2,S1|S2|S3|S4
int MRQ::getSENSORUART_SOF( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1, byte * val2, bool bQuery )
{
	//! 65 14
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 65, 14 , (byte)val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! ENUM,ENUM,U8
//! UART1|UART2,S1|S2|S3|S4
int MRQ::setSENSORUART_FRAMELEN( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1
,byte val2 )
{
	//! 65 15
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 65, 15 , (unsigned char)val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_FRAMELEN( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM,ENUM
//! UART1|UART2,S1|S2|S3|S4
int MRQ::getSENSORUART_FRAMELEN( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1, byte * val2, bool bQuery )
{
	//! 65 16
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 65, 16 , (byte)val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! ENUM,ENUM,U8
//! UART1|UART2,S1|S2|S3|S4
int MRQ::setSENSORUART_RECEIVENUM( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1
,byte val2 )
{
	//! 65 17
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 65, 17 , (unsigned char)val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_RECEIVENUM( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM,ENUM
//! UART1|UART2,S1|S2|S3|S4
int MRQ::getSENSORUART_RECEIVENUM( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1, byte * val2, bool bQuery )
{
	//! 65 18
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 65, 18 , (byte)val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! ENUM,ENUM,U32
//! UART1|UART2,S1|S2|S3|S4
int MRQ::setSENSORUART_SWITCHTIME( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1
,uint32 val2 )
{
	//! 65 19
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 65, 19 , (unsigned char)val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRQ_model::setSENSORUART_SWITCHTIME( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM,ENUM
//! UART1|UART2,S1|S2|S3|S4
int MRQ::getSENSORUART_SWITCHTIME( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1, uint32 * val2, bool bQuery )
{
	//! 65 20
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 65, 20 , (byte)val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! ENUM,ENUM
//! UART1|UART2,S1|S2|S3|S4
int MRQ::getSENSORUART_DATA( MRQ_SENSORUART_BAUD val0
,MRQ_IDENTITY_LABEL_1 val1, byte * val2, char * val3, char * val4, char * val5, bool bQuery )
{
	//! 65 21
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
	byte lval3 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 65, 21 , (byte)val0, (byte)val1, &lval0, &lval1, &lval2, &lval3, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	*val3 = (char)lval1;
	*val4 = (char)lval2;
	*val5 = (char)lval3;
	return 0;
}
//! ENUM
//! OFF|ON
int MRQ::setISOLATORIN_STATE( MRQ_CAN_NETMANAGELED val0 )
{
	//! 72 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 72, 0 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setISOLATORIN_STATE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getISOLATORIN_STATE(  MRQ_CAN_NETMANAGELED * val0, bool bQuery )
{
	//! 72 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 72, 1 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_CAN_NETMANAGELED)lval0;
	return 0;
}
//! ENUM
//! RESERVE|LOW|RISE|FALL|HIGH
int MRQ::setISOLATORIN_TYPE( MRQ_TRIGGER_LEVELTYPE_1 val0 )
{
	//! 72 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 72, 2 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setISOLATORIN_TYPE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getISOLATORIN_TYPE(  MRQ_TRIGGER_LEVELTYPE_1 * val0, bool bQuery )
{
	//! 72 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 72, 3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_TRIGGER_LEVELTYPE_1)lval0;
	return 0;
}
//! ENUM
//! NONE|ALARM|STOP|ALARM&STOP
int MRQ::setISOLATORIN_RESPONSE( MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val0 )
{
	//! 72 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 72, 4 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setISOLATORIN_RESPONSE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getISOLATORIN_RESPONSE(  MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val0, bool bQuery )
{
	//! 72 5
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 72, 5 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRQ::setISOLATORIN_RESPCHAN( byte val0 )
{
	//! 72 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 72, 6 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setISOLATORIN_RESPCHAN( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getISOLATORIN_RESPCHAN(  byte * val0, bool bQuery )
{
	//! 72 7
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 72, 7 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! ENUM
//! CONTINUED|INTERVAL|SINGLE
int MRQ::setISOLATORIN_SMODE( MRQ_TRIGGER_PATTSMODE val0 )
{
	//! 72 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 72, 8 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setISOLATORIN_SMODE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getISOLATORIN_SMODE(  MRQ_TRIGGER_PATTSMODE * val0, bool bQuery )
{
	//! 72 9
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 72, 9 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_TRIGGER_PATTSMODE)lval0;
	return 0;
}
//! U32
//! 
int MRQ::setISOLATORIN_SPERIOD( uint32 val0 )
{
	//! 72 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 72, 10 , val0 );

	if ( ret == 0) 
	{
		MRQ_model::setISOLATORIN_SPERIOD( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getISOLATORIN_SPERIOD(  uint32 * val0, bool bQuery )
{
	//! 72 11
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 72, 11 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! ENUM,ENUM
//! S1|S2|S3|S4,OFF|ON
int MRQ::setABSENCALARM_STATE( MRQ_IDENTITY_LABEL_1 val0
,MRQ_CAN_NETMANAGELED val1 )
{
	//! 73 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 73, 0 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setABSENCALARM_STATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! S1|S2|S3|S4
int MRQ::getABSENCALARM_STATE( MRQ_IDENTITY_LABEL_1 val0, MRQ_CAN_NETMANAGELED * val1, bool bQuery )
{
	//! 73 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 73, 1 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_CAN_NETMANAGELED)lval0;
	return 0;
}
//! ENUM,U32
//! S1|S2|S3|S4
int MRQ::setABSENCALARM_UPLIMIT( MRQ_IDENTITY_LABEL_1 val0
,uint32 val1 )
{
	//! 73 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 73, 2 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setABSENCALARM_UPLIMIT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! S1|S2|S3|S4
int MRQ::getABSENCALARM_UPLIMIT( MRQ_IDENTITY_LABEL_1 val0, uint32 * val1, bool bQuery )
{
	//! 73 3
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 73, 3 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,U32
//! S1|S2|S3|S4
int MRQ::setABSENCALARM_DOWNLIMIT( MRQ_IDENTITY_LABEL_1 val0
,uint32 val1 )
{
	//! 73 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 73, 4 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setABSENCALARM_DOWNLIMIT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! S1|S2|S3|S4
int MRQ::getABSENCALARM_DOWNLIMIT( MRQ_IDENTITY_LABEL_1 val0, uint32 * val1, bool bQuery )
{
	//! 73 5
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 73, 5 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,U32
//! S1|S2|S3|S4
int MRQ::setABSENCALARM_ZEROVALUE( MRQ_IDENTITY_LABEL_1 val0
,uint32 val1 )
{
	//! 73 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 73, 6 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setABSENCALARM_ZEROVALUE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! S1|S2|S3|S4
int MRQ::getABSENCALARM_ZEROVALUE( MRQ_IDENTITY_LABEL_1 val0, uint32 * val1, bool bQuery )
{
	//! 73 7
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 73, 7 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,ENUM
//! S1|S2|S3|S4,NONE|EXIS
int MRQ::setABSENCALARM_ZEROPOSITION( MRQ_IDENTITY_LABEL_1 val0
,MRQ_ABSENCALARM_ZEROPOSITION_1 val1 )
{
	//! 73 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 73, 8 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setABSENCALARM_ZEROPOSITION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! S1|S2|S3|S4
int MRQ::getABSENCALARM_ZEROPOSITION( MRQ_IDENTITY_LABEL_1 val0, MRQ_ABSENCALARM_ZEROPOSITION_1 * val1, bool bQuery )
{
	//! 73 9
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 73, 9 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_ABSENCALARM_ZEROPOSITION_1)lval0;
	return 0;
}
//! ENUM
//! NONE|ALARM|STOP|ALARM&STOP
int MRQ::setABSENCALARM_RESPONSE( MRQ_MOTIONPLAN_OOSLINERESPONSE_1 val0 )
{
	//! 73 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 73, 10 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRQ_model::setABSENCALARM_RESPONSE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRQ::getABSENCALARM_RESPONSE(  MRQ_MOTIONPLAN_OOSLINERESPONSE_1 * val0, bool bQuery )
{
	//! 73 11
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 73, 11 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)lval0;
	return 0;
}
//! ENUM,ENUM
//! S1|S2|S3|S4,OFF|ON
int MRQ::setDISTANCEALARM_STATE( MRQ_IDENTITY_LABEL_1 val0
,MRQ_CAN_NETMANAGELED val1 )
{
	//! 74 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 74, 0 , (unsigned char)val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDISTANCEALARM_STATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! S1|S2|S3|S4
int MRQ::getDISTANCEALARM_STATE( MRQ_IDENTITY_LABEL_1 val0, MRQ_CAN_NETMANAGELED * val1, bool bQuery )
{
	//! 74 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 74, 1 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRQ_CAN_NETMANAGELED)lval0;
	return 0;
}
//! ENUM,U16
//! S1|S2|S3|S4
int MRQ::setDISTANCEALARM_ALARM1DIST( MRQ_IDENTITY_LABEL_1 val0
,uint16 val1 )
{
	//! 74 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 74, 2 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDISTANCEALARM_ALARM1DIST( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! S1|S2|S3|S4
int MRQ::getDISTANCEALARM_ALARM1DIST( MRQ_IDENTITY_LABEL_1 val0, uint16 * val1, bool bQuery )
{
	//! 74 3
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 74, 3 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,U16
//! S1|S2|S3|S4
int MRQ::setDISTANCEALARM_ALARM2DIST( MRQ_IDENTITY_LABEL_1 val0
,uint16 val1 )
{
	//! 74 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 74, 4 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDISTANCEALARM_ALARM2DIST( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! S1|S2|S3|S4
int MRQ::getDISTANCEALARM_ALARM2DIST( MRQ_IDENTITY_LABEL_1 val0, uint16 * val1, bool bQuery )
{
	//! 74 5
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 74, 5 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! ENUM,U16
//! S1|S2|S3|S4
int MRQ::setDISTANCEALARM_ALARM3DIST( MRQ_IDENTITY_LABEL_1 val0
,uint16 val1 )
{
	//! 74 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, 74, 6 , (unsigned char)val0, val1 );

	if ( ret == 0) 
	{
		MRQ_model::setDISTANCEALARM_ALARM3DIST( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! S1|S2|S3|S4
int MRQ::getDISTANCEALARM_ALARM3DIST( MRQ_IDENTITY_LABEL_1 val0, uint16 * val1, bool bQuery )
{
	//! 74 7
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, 74, 7 , (byte)val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
}
