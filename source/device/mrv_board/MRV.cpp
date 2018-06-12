#include "MRV.h"
namespace MegaDevice
{
MRV::MRV()
{
}
//! ENUM
//! NONE|CAN|RS232
int MRV::setLINK_INTFC( MRV_LINK_INTFC val0 )
{
	//! 1 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)1, (byte)0 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRV_model::setLINK_INTFC( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getLINK_INTFC(  MRV_LINK_INTFC * val0, bool bQuery )
{
	//! 1 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)1, (byte)1 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_LINK_INTFC)lval0;
	return 0;
}
//! 
//! 
int MRV::getLINK_DEVICEINFO(  MRV_LINK_DEVICEINFO * val0, MRV_LINK_DEVICEINFO_1 * val1, uint32 * val2, bool bQuery )
{
	//! 1 2
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	uint32 lval2 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)1, (byte)2 , &lval0, &lval1, &lval2, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_LINK_DEVICEINFO)lval0;
	*val1 = (MRV_LINK_DEVICEINFO_1)lval1;
	*val2 = lval2;
	return 0;
}
//! ENUM
//! NORMAL|NONE
int MRV::setSYSTEM_WORKMODE( MRV_SYSTEM_WORKMODE val0 )
{
	//! 2 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)2, (byte)0 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRV_model::setSYSTEM_WORKMODE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getSYSTEM_WORKMODE(  MRV_SYSTEM_WORKMODE * val0, bool bQuery )
{
	//! 2 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)2, (byte)1 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_SYSTEM_WORKMODE)lval0;
	return 0;
}
//! ENUM
//! DEFAULT|LAST
int MRV::setSYSTEM_POWERON( MRV_SYSTEM_POWERON val0 )
{
	//! 2 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)2, (byte)2 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRV_model::setSYSTEM_POWERON( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getSYSTEM_POWERON(  MRV_SYSTEM_POWERON * val0, bool bQuery )
{
	//! 2 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)2, (byte)3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_SYSTEM_POWERON)lval0;
	return 0;
}
//! 
//! 
int MRV::getSYSTEM_SN(  byte * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery )
{
	//! 2 4
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
	byte lval3 = 0;
	byte lval4 = 0;
	byte lval5 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)2, (byte)4 , &lval0, &lval1, &lval2, &lval3, &lval4, &lval5, bQuery);
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
int MRV::getSYSTEM_TYPE(  MRV_SYSTEM_TYPE * val0, MRV_SYSTEM_TYPE_1 * val1, bool bQuery )
{
	//! 2 5
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)2, (byte)5 , &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_SYSTEM_TYPE)lval0;
	*val1 = (MRV_SYSTEM_TYPE_1)lval1;
	return 0;
}
//! 
//! 
int MRV::getSYSTEM_SOFTVER(  char * val0, char * val1, char * val2, bool bQuery )
{
	//! 2 6
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)2, (byte)6 , &lval0, &lval1, &lval2, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (char)lval0;
	*val1 = (char)lval1;
	*val2 = (char)lval2;
	return 0;
}
//! 
//! 
int MRV::getSYSTEM_HARDVER(  char * val0, char * val1, bool bQuery )
{
	//! 2 7
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)2, (byte)7 , &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (char)lval0;
	*val1 = (char)lval1;
	return 0;
}
//! 
//! 
int MRV::getSYSTEM_BOOTVER(  char * val0, char * val1, bool bQuery )
{
	//! 2 8
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)2, (byte)8 , &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (char)lval0;
	*val1 = (char)lval1;
	return 0;
}
//! 
//! 
int MRV::getSYSTEM_ERROR(  char * val0, char * val1, char * val2, char * val3, char * val4, char * val5, bool bQuery )
{
	//! 2 9
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	byte lval2 = 0;
	byte lval3 = 0;
	byte lval4 = 0;
	byte lval5 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)2, (byte)9 , &lval0, &lval1, &lval2, &lval3, &lval4, &lval5, bQuery);
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
//! U8
//! [CHANNUM]
int MRV::setSYSTEM_WARNCLEAR( byte val0 )
{
	//! 2 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)2, (byte)10 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setSYSTEM_WARNCLEAR( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! 4800|7200|9600|14400|19200|38400|57600|115200|128000
int MRV::setRS232_BAUD( MRV_RS232_BAUD val0 )
{
	//! 3 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)3, (byte)0 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRV_model::setRS232_BAUD( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getRS232_BAUD(  MRV_RS232_BAUD * val0, bool bQuery )
{
	//! 3 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)3, (byte)1 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_RS232_BAUD)lval0;
	return 0;
}
//! ENUM
//! 8|9
int MRV::setRS232_WORDLEN( MRV_RS232_WORDLEN val0 )
{
	//! 3 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)3, (byte)2 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRV_model::setRS232_WORDLEN( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getRS232_WORDLEN(  MRV_RS232_WORDLEN * val0, bool bQuery )
{
	//! 3 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)3, (byte)3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_RS232_WORDLEN)lval0;
	return 0;
}
//! ENUM
//! NONE|RTS|CTS|RTS&CTS
int MRV::setRS232_FLOWCTL( MRV_RS232_FLOWCTL val0 )
{
	//! 3 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)3, (byte)4 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRV_model::setRS232_FLOWCTL( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getRS232_FLOWCTL(  MRV_RS232_FLOWCTL * val0, bool bQuery )
{
	//! 3 5
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)3, (byte)5 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_RS232_FLOWCTL)lval0;
	return 0;
}
//! ENUM
//! NONE|EVEN|ODD
int MRV::setRS232_PARITY( MRV_RS232_PARITY val0 )
{
	//! 3 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)3, (byte)6 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRV_model::setRS232_PARITY( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getRS232_PARITY(  MRV_RS232_PARITY * val0, bool bQuery )
{
	//! 3 7
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)3, (byte)7 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_RS232_PARITY)lval0;
	return 0;
}
//! ENUM
//! 1|0.5|2|1.5
int MRV::setRS232_STOPBIT( MRV_RS232_STOPBIT val0 )
{
	//! 3 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)3, (byte)8 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRV_model::setRS232_STOPBIT( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getRS232_STOPBIT(  MRV_RS232_STOPBIT * val0, bool bQuery )
{
	//! 3 9
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)3, (byte)9 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_RS232_STOPBIT)lval0;
	return 0;
}
//! 
//! 
int MRV::setRS232_APPLYPARA(  )
{
	//! 3 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)3, (byte)10  );

	if ( ret == 0) 
	{
		MRV_model::setRS232_APPLYPARA(  );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! A|B
int MRV::setCAN_TYPE( MRV_CAN_TYPE val0 )
{
	//! 4 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)0 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_TYPE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getCAN_TYPE(  MRV_CAN_TYPE * val0, bool bQuery )
{
	//! 4 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)4, (byte)1 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_CAN_TYPE)lval0;
	return 0;
}
//! ENUM
//! 1000|500|250|125|100|50|20|10
int MRV::setCAN_BAUD( MRV_CAN_BAUD val0 )
{
	//! 4 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)2 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_BAUD( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getCAN_BAUD(  MRV_CAN_BAUD * val0, bool bQuery )
{
	//! 4 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)4, (byte)3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_CAN_BAUD)lval0;
	return 0;
}
//! U8
//! 
int MRV::setCAN_GROUP( byte val0 )
{
	//! 4 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)4 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_GROUP( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getCAN_GROUP(  byte * val0, bool bQuery )
{
	//! 4 5
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)4, (byte)5 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRV::setCAN_SENDID( uint32 val0 )
{
	//! 4 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)6 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_SENDID( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getCAN_SENDID(  uint32 * val0, bool bQuery )
{
	//! 4 7
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)4, (byte)7 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRV::setCAN_RECEIVEID( uint32 val0 )
{
	//! 4 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)8 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_RECEIVEID( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getCAN_RECEIVEID(  uint32 * val0, bool bQuery )
{
	//! 4 9
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)4, (byte)9 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRV::setCAN_GROUPID1( uint32 val0 )
{
	//! 4 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)10 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_GROUPID1( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getCAN_GROUPID1(  uint32 * val0, bool bQuery )
{
	//! 4 11
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)4, (byte)11 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRV::setCAN_GROUPID2( uint32 val0 )
{
	//! 4 12
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)12 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_GROUPID2( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getCAN_GROUPID2(  uint32 * val0, bool bQuery )
{
	//! 4 13
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)4, (byte)13 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRV::setCAN_BROADCASTID( uint32 val0 )
{
	//! 4 14
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)14 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_BROADCASTID( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getCAN_BROADCASTID(  uint32 * val0, bool bQuery )
{
	//! 4 15
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)4, (byte)15 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! 
//! 
int MRV::setCAN_APPLYPARA(  )
{
	//! 4 16
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)16  );

	if ( ret == 0) 
	{
		MRV_model::setCAN_APPLYPARA(  );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! OFF|ON
int MRV::setCAN_NETMANAGELED( MRV_CAN_NETMANAGELED val0 )
{
	//! 4 17
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)17 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_NETMANAGELED( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! ENUM
//! IDLE|HASH|SIGNATURE
int MRV::setCAN_NETMANAGESTATE( MRV_CAN_NETMANAGESTATE val0 )
{
	//! 4 18
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)18 , (unsigned char)val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_NETMANAGESTATE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getCAN_NETMANAGESTATE(  MRV_CAN_NETMANAGESTATE * val0, bool bQuery )
{
	//! 4 19
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)4, (byte)19 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = (MRV_CAN_NETMANAGESTATE)lval0;
	return 0;
}
//! U32
//! 
int MRV::setCAN_NETMANAGEID( uint32 val0 )
{
	//! 4 20
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)20 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_NETMANAGEID( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getCAN_NETMANAGEID(  uint32 * val0, bool bQuery )
{
	//! 4 21
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)4, (byte)21 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRV::setCAN_NETMANAGEHASH( uint32 val0 )
{
	//! 4 22
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)22 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_NETMANAGEHASH( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getCAN_NETMANAGEHASH(  uint32 * val0, bool bQuery )
{
	//! 4 23
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)4, (byte)23 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U32
//! 
int MRV::setCAN_NETMANAGESIGNATURE( uint32 val0 )
{
	//! 4 24
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)4, (byte)24 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setCAN_NETMANAGESIGNATURE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getCAN_NETMANAGESIGNATURE(  uint32 * val0, bool bQuery )
{
	//! 4 25
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)4, (byte)25 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U8,ENUM,ENUM
//! [CHANNUM],RESET|STOP|RUN,MAIN|RESSERVE1
int MRV::setMOTION_SWITCH( byte val0
,MRV_MOTION_SWITCH val1
,MRV_MOTION_SWITCH_1 val2 )
{
	//! 21 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)21, (byte)0 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRV_model::setMOTION_SWITCH( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],MAIN|RESSERVE1
int MRV::getMOTION_STATE( byte val0
,MRV_MOTION_SWITCH_1 val1, MRV_MOTION_STATE_1 * val2, bool bQuery )
{
	//! 21 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)21, (byte)1 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRV_MOTION_STATE_1)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],ACTIVE|QUERY
int MRV::setMOTION_STATEREPORT( byte val0
,MRV_MOTION_STATEREPORT val1 )
{
	//! 21 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)21, (byte)2 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setMOTION_STATEREPORT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getMOTION_STATEREPORT( byte val0, MRV_MOTION_STATEREPORT * val1, bool bQuery )
{
	//! 21 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)21, (byte)3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_MOTION_STATEREPORT)lval0;
	return 0;
}
//! U8,ENUM,U8
//! [CHANNUM],GROUP1|GROUP2
int MRV::setGLOBAL_GROUP( byte val0
,MRV_GLOBAL_GROUP val1
,byte val2 )
{
	//! 22 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)22, (byte)0 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRV_model::setGLOBAL_GROUP( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],GROUP1|GROUP2
int MRV::getGLOBAL_GROUP( byte val0
,MRV_GLOBAL_GROUP val1, byte * val2, bool bQuery )
{
	//! 22 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)22, (byte)1 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int MRV::setGLOBAL_DISTINGUISH( byte val0
,MRV_CAN_NETMANAGELED val1 )
{
	//! 22 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)22, (byte)2 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setGLOBAL_DISTINGUISH( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! 
int MRV::getGLOBAL_DISTINGUISH( byte val0, MRV_CAN_NETMANAGELED * val1, bool bQuery )
{
	//! 22 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)22, (byte)3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_CAN_NETMANAGELED)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],VALVE|MOTOR
int MRV::setVALVECTRL_DEVICE( byte val0
,MRV_VALVECTRL_DEVICE val1 )
{
	//! 51 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)0 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_DEVICE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getVALVECTRL_DEVICE( byte val0, MRV_VALVECTRL_DEVICE * val1, bool bQuery )
{
	//! 51 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_VALVECTRL_DEVICE)lval0;
	return 0;
}
//! U32
//! [CHANNUM]
int MRV::setVALVECTRL_PWMFREQ( uint32 val0 )
{
	//! 51 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)2 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_PWMFREQ( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getVALVECTRL_PWMFREQ(  uint32 * val0, bool bQuery )
{
	//! 51 3
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)3 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRV::setVALVECTRL_RTDUTY( byte val0
,uint32 val1 )
{
	//! 51 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)4 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_RTDUTY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getVALVECTRL_RTDUTY( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 5
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)5 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON|IDLE
int MRV::setVALVECTRL_ACTION( byte val0
,MRV_VALVECTRL_ACTION val1 )
{
	//! 51 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)6 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_ACTION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getVALVECTRL_ACTION( byte val0, MRV_VALVECTRL_ACTION * val1, bool bQuery )
{
	//! 51 7
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_VALVECTRL_ACTION)lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRV::setVALVECTRL_IDLEDUTY( byte val0
,uint32 val1 )
{
	//! 51 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)8 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_IDLEDUTY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getVALVECTRL_IDLEDUTY( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 9
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)9 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRV::setVALVECTRL_HOLDDUTY( byte val0
,uint32 val1 )
{
	//! 51 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)10 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_HOLDDUTY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getVALVECTRL_HOLDDUTY( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 11
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)11 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRV::setVALVECTRL_OPENDUTY( byte val0
,uint32 val1 )
{
	//! 51 12
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)12 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_OPENDUTY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getVALVECTRL_OPENDUTY( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 13
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)13 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U16
//! [CHANNUM]
int MRV::setVALVECTRL_OPENDLYTIME( byte val0
,uint16 val1 )
{
	//! 51 14
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)14 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_OPENDLYTIME( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getVALVECTRL_OPENDLYTIME( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 15
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)15 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U16
//! [CHANNUM]
int MRV::setVALVECTRL_OPENTIME( byte val0
,uint16 val1 )
{
	//! 51 16
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)16 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_OPENTIME( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getVALVECTRL_OPENTIME( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 17
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)17 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],OFF|ON
int MRV::setVALVECTRL_RECORDSTATE( byte val0
,MRV_CAN_NETMANAGELED val1 )
{
	//! 51 18
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)18 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_RECORDSTATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getVALVECTRL_RECORDSTATE( byte val0, MRV_CAN_NETMANAGELED * val1, bool bQuery )
{
	//! 51 19
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)19 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_CAN_NETMANAGELED)lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRV::getVALVECTRL_ONNUMS( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 20
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)20 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRV::setVALVECTRL_ONNUMSCLEAR( byte val0 )
{
	//! 51 21
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)21 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_ONNUMSCLEAR( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getVALVECTRL_OFFNUMS( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 22
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)22 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRV::setVALVECTRL_OFFNUMSCLEAR( byte val0 )
{
	//! 51 23
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)23 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_OFFNUMSCLEAR( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getVALVECTRL_TIME( byte val0, uint32 * val1, bool bQuery )
{
	//! 51 24
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)51, (byte)24 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRV::setVALVECTRL_TIMECLEAR( byte val0 )
{
	//! 51 25
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)51, (byte)25 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setVALVECTRL_TIMECLEAR( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,U32
//! [CHANNUM]
int MRV::setTHRESHOLD_HIGHPRESSURE( byte val0
,uint32 val1 )
{
	//! 52 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)52, (byte)0 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setTHRESHOLD_HIGHPRESSURE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getTHRESHOLD_HIGHPRESSURE( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 1
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)52, (byte)1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|WARN|STOP
int MRV::setTHRESHOLD_HIPRESSUREACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 )
{
	//! 52 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)52, (byte)2 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setTHRESHOLD_HIPRESSUREACTION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getTHRESHOLD_HIPRESSUREACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery )
{
	//! 52 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)52, (byte)3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_THRESHOLD_HIPRESSUREACTION)lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRV::setTHRESHOLD_LOWPRESSURE( byte val0
,uint32 val1 )
{
	//! 52 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)52, (byte)4 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setTHRESHOLD_LOWPRESSURE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getTHRESHOLD_LOWPRESSURE( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 5
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)52, (byte)5 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|WARN|STOP
int MRV::setTHRESHOLD_LOWPRESSUREACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 )
{
	//! 52 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)52, (byte)6 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setTHRESHOLD_LOWPRESSUREACTION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getTHRESHOLD_LOWPRESSUREACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery )
{
	//! 52 7
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)52, (byte)7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_THRESHOLD_HIPRESSUREACTION)lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRV::setTHRESHOLD_HIGHCURRENT( byte val0
,uint32 val1 )
{
	//! 52 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)52, (byte)8 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setTHRESHOLD_HIGHCURRENT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getTHRESHOLD_HIGHCURRENT( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 9
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)52, (byte)9 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|WARN|STOP
int MRV::setTHRESHOLD_HICURRENTACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 )
{
	//! 52 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)52, (byte)10 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setTHRESHOLD_HICURRENTACTION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! [CHANNUM]
int MRV::getTHRESHOLD_HICURRENTACTION(  byte * val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery )
{
	//! 52 11
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)52, (byte)11 , &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	*val1 = (MRV_THRESHOLD_HIPRESSUREACTION)lval1;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRV::setTHRESHOLD_LOWCURRENT( byte val0
,uint32 val1 )
{
	//! 52 12
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)52, (byte)12 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setTHRESHOLD_LOWCURRENT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getTHRESHOLD_LOWCURRENT( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 13
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)52, (byte)13 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|WARN|STOP
int MRV::setTHRESHOLD_LOWCURRENTACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 )
{
	//! 52 14
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)52, (byte)14 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setTHRESHOLD_LOWCURRENTACTION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! [CHANNUM]
int MRV::getTHRESHOLD_LOWCURRENTACTION(  byte * val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery )
{
	//! 52 15
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)52, (byte)15 , &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	*val1 = (MRV_THRESHOLD_HIPRESSUREACTION)lval1;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRV::setTHRESHOLD_ONNUMS( byte val0
,uint32 val1 )
{
	//! 52 16
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)52, (byte)16 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setTHRESHOLD_ONNUMS( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getTHRESHOLD_ONNUMS( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 17
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)52, (byte)17 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|WARN|STOP
int MRV::setTHRESHOLD_NUMSACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 )
{
	//! 52 18
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)52, (byte)18 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setTHRESHOLD_NUMSACTION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getTHRESHOLD_NUMSACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery )
{
	//! 52 19
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)52, (byte)19 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_THRESHOLD_HIPRESSUREACTION)lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRV::setTHRESHOLD_TIME( byte val0
,uint32 val1 )
{
	//! 52 20
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)52, (byte)20 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setTHRESHOLD_TIME( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getTHRESHOLD_TIME( byte val0, uint32 * val1, bool bQuery )
{
	//! 52 21
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)52, (byte)21 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|WARN|STOP
int MRV::setTHRESHOLD_TIMEACTION( byte val0
,MRV_THRESHOLD_HIPRESSUREACTION val1 )
{
	//! 52 22
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)52, (byte)22 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setTHRESHOLD_TIMEACTION( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getTHRESHOLD_TIMEACTION( byte val0, MRV_THRESHOLD_HIPRESSUREACTION * val1, bool bQuery )
{
	//! 52 23
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)52, (byte)23 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_THRESHOLD_HIPRESSUREACTION)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],RT|NCYCLE|FIFO
int MRV::setPVT_EXECMODE( byte val0
,MRV_PVT_EXECMODE val1 )
{
	//! 53 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)53, (byte)0 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setPVT_EXECMODE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getPVT_EXECMODE( byte val0, MRV_PVT_EXECMODE * val1, bool bQuery )
{
	//! 53 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)53, (byte)1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_PVT_EXECMODE)lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRV::setPVT_RESET( byte val0 )
{
	//! 53 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)53, (byte)2 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setPVT_RESET( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,U32
//! [CHANNUM]
int MRV::setPVT_CYCLES( byte val0
,uint32 val1 )
{
	//! 53 3
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)53, (byte)3 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setPVT_CYCLES( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getPVT_CYCLES( byte val0, uint32 * val1, bool bQuery )
{
	//! 53 4
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)53, (byte)4 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U8,U32
//! [CHANNUM]
int MRV::setPVT_GRADE( byte val0
,byte val1
,uint32 val2 )
{
	//! 53 5
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)53, (byte)5 , val0, val1, val2 );

	if ( ret == 0) 
	{
		MRV_model::setPVT_GRADE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,U8
//! [CHANNUM]
int MRV::getPVT_GRADE( byte val0
,byte val1, uint32 * val2, bool bQuery )
{
	//! 53 6
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)53, (byte)6 , val0, val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRV::getPVT_BUFFERCHECK( byte val0, uint32 * val1, bool bQuery )
{
	//! 53 7
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)53, (byte)7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRV::setPVT_START( byte val0 )
{
	//! 53 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)53, (byte)8 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setPVT_START( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getPVT_START(  byte * val0, bool bQuery )
{
	//! 53 9
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)53, (byte)9 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRV::setPVT_END( byte val0 )
{
	//! 53 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)53, (byte)10 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setPVT_END( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getPVT_END(  byte * val0, bool bQuery )
{
	//! 53 11
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)53, (byte)11 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U16,U32
//! 
int MRV::setPVT_PDATA( uint16 val0
,uint32 val1 )
{
	//! 53 12
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)53, (byte)12 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setPVT_PDATA( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U16
//! 
int MRV::getPVT_PDATA( uint16 val0, uint32 * val1, bool bQuery )
{
	//! 53 13
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)53, (byte)13 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U16,U32
//! 
int MRV::setPVT_TDATA( uint16 val0
,uint32 val1 )
{
	//! 53 14
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)53, (byte)14 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setPVT_TDATA( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U16
//! 
int MRV::getPVT_TDATA( uint16 val0, uint32 * val1, bool bQuery )
{
	//! 53 15
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)53, (byte)15 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],TRUE|FALSE
int MRV::setPVT_HOLD( byte val0
,MRV_PVT_HOLD val1 )
{
	//! 53 16
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)53, (byte)16 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setPVT_HOLD( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! 
int MRV::getPVT_HOLD( byte val0, MRV_PVT_HOLD * val1, bool bQuery )
{
	//! 53 17
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)53, (byte)17 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_PVT_HOLD)lval0;
	return 0;
}
//! U8
//! 
int MRV::setPVT_SAVE( byte val0 )
{
	//! 53 18
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)53, (byte)18 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setPVT_SAVE( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! 
int MRV::getPVT_SAVE(  byte * val0, bool bQuery )
{
	//! 53 19
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)53, (byte)19 , &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|OPEN|CLOSE
int MRV::setCALIBRATION_TYPE( byte val0
,MRV_CALIBRATION_TYPE val1 )
{
	//! 54 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)54, (byte)0 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setCALIBRATION_TYPE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getCALIBRATION_TYPE( byte val0, MRV_CALIBRATION_TYPE * val1, bool bQuery )
{
	//! 54 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)54, (byte)1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_CALIBRATION_TYPE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],222KHz|176KHz|142KHz|47KHz
int MRV::setCALIBRATION_SAMPLERATE( byte val0
,MRV_CALIBRATION_SAMPLERATE val1 )
{
	//! 54 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)54, (byte)2 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setCALIBRATION_SAMPLERATE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getCALIBRATION_SAMPLERATE( byte val0, MRV_CALIBRATION_SAMPLERATE * val1, bool bQuery )
{
	//! 54 3
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)54, (byte)3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_CALIBRATION_SAMPLERATE)lval0;
	return 0;
}
//! U8,U16
//! [CHANNUM]
int MRV::setCALIBRATION_OPENTIME( byte val0
,uint16 val1 )
{
	//! 54 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)54, (byte)4 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setCALIBRATION_OPENTIME( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getCALIBRATION_OPENTIME( byte val0, uint16 * val1, bool bQuery )
{
	//! 54 5
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)54, (byte)5 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U16
//! [CHANNUM]
int MRV::setCALIBRATION_HOLDTIME( byte val0
,uint16 val1 )
{
	//! 54 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)54, (byte)6 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setCALIBRATION_HOLDTIME( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getCALIBRATION_HOLDTIME( byte val0, uint16 * val1, bool bQuery )
{
	//! 54 7
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)54, (byte)7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U16
//! [CHANNUM]
int MRV::setCALIBRATION_CLOSETIME( byte val0
,uint16 val1 )
{
	//! 54 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)54, (byte)8 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setCALIBRATION_CLOSETIME( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getCALIBRATION_CLOSETIME( byte val0, uint16 * val1, bool bQuery )
{
	//! 54 9
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)54, (byte)9 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRV::setCALIBRATION_OPENDUTY( byte val0
,uint32 val1 )
{
	//! 54 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)54, (byte)10 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setCALIBRATION_OPENDUTY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getCALIBRATION_OPENDUTY( byte val0, uint32 * val1, bool bQuery )
{
	//! 54 11
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)54, (byte)11 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U32
//! [CHANNUM]
int MRV::setCALIBRATION_HOLDDUTY( byte val0
,uint32 val1 )
{
	//! 54 12
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)54, (byte)12 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setCALIBRATION_HOLDDUTY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getCALIBRATION_HOLDDUTY( byte val0, uint32 * val1, bool bQuery )
{
	//! 54 13
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)54, (byte)13 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8
//! [CHANNUM]
int MRV::getCALIBRATION_DATALENGTH( byte val0, uint16 * val1, bool bQuery )
{
	//! 54 14
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)54, (byte)14 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],START|STOP
int MRV::setCALIBRATION_DATAUPCTRL( byte val0
,MRV_CALIBRATION_DATAUPCTRL val1 )
{
	//! 54 15
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)54, (byte)15 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setCALIBRATION_DATAUPCTRL( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U16,U16,U16
//! 
int MRV::setCALIBRATION_DATAUP( uint16 val0
,uint16 val1
,uint16 val2 )
{
	//! 54 16
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)54, (byte)16 , val0, val1, val2 );

	if ( ret == 0) 
	{
		MRV_model::setCALIBRATION_DATAUP( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM,ENUM
//! [CHANNUM],Current|Pressure|Position|Cycle,OFF|ON
int MRV::setREPORT_STATE( byte val0
,MRV_REPORT_STATE val1
,MRV_CAN_NETMANAGELED val2 )
{
	//! 55 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)55, (byte)0 , val0, (unsigned char)val1, (unsigned char)val2 );

	if ( ret == 0) 
	{
		MRV_model::setREPORT_STATE( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],Current|Pressure|Position|Cycle
int MRV::getREPORT_STATE( byte val0
,MRV_REPORT_STATE val1, MRV_CAN_NETMANAGELED * val2, bool bQuery )
{
	//! 55 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)55, (byte)1 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = (MRV_CAN_NETMANAGELED)lval0;
	return 0;
}
//! U8,ENUM,U32
//! [CHANNUM],Current|Pressure|Position|Cycle
int MRV::setREPORT_PERIOD( byte val0
,MRV_REPORT_STATE val1
,uint32 val2 )
{
	//! 55 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)55, (byte)2 , val0, (unsigned char)val1, val2 );

	if ( ret == 0) 
	{
		MRV_model::setREPORT_PERIOD( val0, val1, val2 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8,ENUM
//! [CHANNUM],Current|Pressure|Position|Cycle
int MRV::getREPORT_PERIOD( byte val0
,MRV_REPORT_STATE val1, uint32 * val2, bool bQuery )
{
	//! 55 3
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)55, (byte)3 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],Current|Pressure|Position|Cycle
int MRV::getREPORT_DATA( byte val0
,MRV_REPORT_STATE val1, uint32 * val2, bool bQuery )
{
	//! 55 4
	int ret = 0;

	uint32 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)55, (byte)4 , val0, (byte)val1, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val2 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],UART|SWITCH
int MRV::setIOCONFIG_IOFUNCSEL( byte val0
,MRV_IOCONFIG_IOFUNCSEL val1 )
{
	//! 56 0
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)56, (byte)0 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setIOCONFIG_IOFUNCSEL( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getIOCONFIG_IOFUNCSEL( byte val0, MRV_IOCONFIG_IOFUNCSEL * val1, bool bQuery )
{
	//! 56 1
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)56, (byte)1 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_IOCONFIG_IOFUNCSEL)lval0;
	return 0;
}
//! U8,U16
//! [CHANNUM]
int MRV::setIOCONFIG_SWFILTERDELAY( byte val0
,uint16 val1 )
{
	//! 56 2
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)56, (byte)2 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setIOCONFIG_SWFILTERDELAY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getIOCONFIG_SWFILTERDELAY( byte val0, uint16 * val1, bool bQuery )
{
	//! 56 3
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)56, (byte)3 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,U16
//! [CHANNUM]
int MRV::setIOCONFIG_SWFILTERCOUNTER( byte val0
,uint16 val1 )
{
	//! 56 4
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)56, (byte)4 , val0, val1 );

	if ( ret == 0) 
	{
		MRV_model::setIOCONFIG_SWFILTERCOUNTER( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getIOCONFIG_SWFILTERCOUNTER( byte val0, uint16 * val1, bool bQuery )
{
	//! 56 5
	int ret = 0;

	uint16 lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)56, (byte)5 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|RISE|FALL|BOTH
int MRV::setIOCONFIG_TRIGEDGE( byte val0
,MRV_IOCONFIG_TRIGEDGE val1 )
{
	//! 56 6
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)56, (byte)6 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setIOCONFIG_TRIGEDGE( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getIOCONFIG_TRIGEDGE( byte val0, MRV_IOCONFIG_TRIGEDGE * val1, bool bQuery )
{
	//! 56 7
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)56, (byte)7 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_IOCONFIG_TRIGEDGE)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],4800|7200|9600|14400|19200|38400|57600|115200|128000
int MRV::setIOCONFIG_BAUD( byte val0
,MRV_RS232_BAUD val1 )
{
	//! 56 8
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)56, (byte)8 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setIOCONFIG_BAUD( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getIOCONFIG_BAUD( byte val0, MRV_RS232_BAUD * val1, bool bQuery )
{
	//! 56 9
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)56, (byte)9 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_RS232_BAUD)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],8|9
int MRV::setIOCONFIG_WORDLEN( byte val0
,MRV_RS232_WORDLEN val1 )
{
	//! 56 10
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)56, (byte)10 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setIOCONFIG_WORDLEN( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getIOCONFIG_WORDLEN( byte val0, MRV_RS232_WORDLEN * val1, bool bQuery )
{
	//! 56 11
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)56, (byte)11 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_RS232_WORDLEN)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|RTS|CTS|RTS_CTS
int MRV::setIOCONFIG_FLOWCTL( byte val0
,MRV_IOCONFIG_FLOWCTL val1 )
{
	//! 56 12
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)56, (byte)12 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setIOCONFIG_FLOWCTL( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getIOCONFIG_FLOWCTL( byte val0, MRV_IOCONFIG_FLOWCTL * val1, bool bQuery )
{
	//! 56 13
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)56, (byte)13 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_IOCONFIG_FLOWCTL)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],NONE|EVEN|ODD
int MRV::setIOCONFIG_PARITY( byte val0
,MRV_RS232_PARITY val1 )
{
	//! 56 14
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)56, (byte)14 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setIOCONFIG_PARITY( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! U8
//! [CHANNUM]
int MRV::getIOCONFIG_PARITY( byte val0, MRV_RS232_PARITY * val1, bool bQuery )
{
	//! 56 15
	int ret = 0;

	byte lval0 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)56, (byte)15 , val0, &lval0, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val1 = (MRV_RS232_PARITY)lval0;
	return 0;
}
//! U8,ENUM
//! [CHANNUM],1|0.5|2|1.5
int MRV::setIOCONFIG_STOPBIT( byte val0
,MRV_RS232_STOPBIT val1 )
{
	//! 56 16
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)56, (byte)16 , val0, (unsigned char)val1 );

	if ( ret == 0) 
	{
		MRV_model::setIOCONFIG_STOPBIT( val0, val1 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
//! 
//! [CHANNUM]
int MRV::getIOCONFIG_STOPBIT(  byte * val0, MRV_RS232_STOPBIT * val1, bool bQuery )
{
	//! 56 17
	int ret = 0;

	byte lval0 = 0;
	byte lval1 = 0;
	ret = m_pBus->read( DEVICE_RECEIVE_ID, (byte)56, (byte)17 , &lval0, &lval1, bQuery);
	if ( ret != 0){ log_device(); } 
	if ( ret != 0) return ret;
	*val0 = lval0;
	*val1 = (MRV_RS232_STOPBIT)lval1;
	return 0;
}
//! U8
//! [CHANNUM]
int MRV::setIOCONFIG_APPLYPARA( byte val0 )
{
	//! 56 18
	int ret; 
	ret = m_pBus->write( DEVICE_RECEIVE_ID, (byte)56, (byte)18 , val0 );

	if ( ret == 0) 
	{
		MRV_model::setIOCONFIG_APPLYPARA( val0 );
	}
	if ( ret != 0){ log_device(); } 
	return ret;
}
}
