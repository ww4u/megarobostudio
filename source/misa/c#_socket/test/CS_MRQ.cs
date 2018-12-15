using System;
namespace mega_device
{
public class CS_MRQ : MegaRobo
{

    public CS_MRQ()
    {}
    //!ENUM
    //!NONE|CAN|RS232
    public int setLINK_INTFC(  string val0 )
    {
        //!建立连接命令，当使用CAN接口或者RS232接口进行初次连接时，需要发送此命令建立连接
        //!LINK INTFC
        string args;
        args = string.Format("LINK:INTFC {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_LINK_INTFC
    //!val0
    public int getLINK_INTFC(  out string val0 )
    {
        //!
        //!LINK INTFC

        //! init the out
        val0=null;

        string args;
        args = string.Format("LINK:INTFC?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!
    //!
    //!MRQ_LINK_DEVICEINFO,MRQ_LINK_DEVICEINFO_1,uint32
    //!val0,val1,val2
    public int getLINK_DEVICEINFO(  out string val0, out string val1, out UInt32 val2 )
    {
        //!设备信息查询，包括主型号、子型号以及哈希值
        //!LINK DEVICEINFO

        //! init the out
        val0=null;
        val1=null;
        val2=0;

        string args;
        args = string.Format("LINK:DEVICEINFO?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 3 )
        { return -1; } 
        val0 = paras[0];
        val1 = paras[1];
        val2 = Convert.ToUInt32( paras[2] );
        return 0;
    }

    //!ENUM
    //!NORMAL|NONE
    public int setSYSTEM_WORKMODE(  string val0 )
    {
        //!设置工作模式
        //!SYSTEM WORKMODE
        string args;
        args = string.Format("SYSTEM:WORKMODE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_SYSTEM_WORKMODE
    //!val0
    public int getSYSTEM_WORKMODE(  out string val0 )
    {
        //!查询当前工作模式
        //!SYSTEM WORKMODE

        //! init the out
        val0=null;

        string args;
        args = string.Format("SYSTEM:WORKMODE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM
    //!DEFAULT|LAST
    public int setSYSTEM_POWERON(  string val0 )
    {
        //!设置上电状态
        //!SYSTEM POWERON
        string args;
        args = string.Format("SYSTEM:POWERON {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_SYSTEM_POWERON
    //!val0
    public int getSYSTEM_POWERON(  out string val0 )
    {
        //!查询上电状态
        //!SYSTEM POWERON

        //! init the out
        val0=null;

        string args;
        args = string.Format("SYSTEM:POWERON?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!
    //!
    //!byte,char,char,char,char,char
    //!val0,val1,val2,val3,val4,val5
    public int getSYSTEM_SN(  out byte val0, out byte val1, out byte val2, out byte val3, out byte val4, out byte val5 )
    {
        //!查询序列号，序列号长度大于6，所以无法一次发送完，需要多次发送，参数的第一个字节代表SN的段，后5个字节是当前段的数据
        //!SYSTEM SN

        //! init the out
        val0=0;
        val1=0;
        val2=0;
        val3=0;
        val4=0;
        val5=0;

        string args;
        args = string.Format("SYSTEM:SN?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 6 )
        { return -1; } 
        val0 = Convert.ToByte( paras[0] );
        val1 = Convert.ToByte( paras[1] );
        val2 = Convert.ToByte( paras[2] );
        val3 = Convert.ToByte( paras[3] );
        val4 = Convert.ToByte( paras[4] );
        val5 = Convert.ToByte( paras[5] );
        return 0;
    }

    //!
    //!
    //!MRQ_LINK_DEVICEINFO,MRQ_LINK_DEVICEINFO_1
    //!val0,val1
    public int getSYSTEM_TYPE(  out string val0, out string val1 )
    {
        //!查询产品类型，子类型只标注了MRQ的子类型
        //!SYSTEM TYPE

        //! init the out
        val0=null;
        val1=null;

        string args;
        args = string.Format("SYSTEM:TYPE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 2 )
        { return -1; } 
        val0 = paras[0];
        val1 = paras[1];
        return 0;
    }

    //!
    //!
    //!char,char,char
    //!val0,val1,val2
    public int getSYSTEM_SOFTVER(  out byte val0, out byte val1, out byte val2 )
    {
        //!查询系统软件版本，数字
        //!SYSTEM SOFTVER

        //! init the out
        val0=0;
        val1=0;
        val2=0;

        string args;
        args = string.Format("SYSTEM:SOFTVER?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 3 )
        { return -1; } 
        val0 = Convert.ToByte( paras[0] );
        val1 = Convert.ToByte( paras[1] );
        val2 = Convert.ToByte( paras[2] );
        return 0;
    }

    //!
    //!
    //!char,char,char,char,char,char
    //!val0,val1,val2,val3,val4,val5
    public int getSYSTEM_FPGAVER(  out byte val0, out byte val1, out byte val2, out byte val3, out byte val4, out byte val5 )
    {
        //!查询系统FPGA版本，数字
        //!SYSTEM FPGAVER

        //! init the out
        val0=0;
        val1=0;
        val2=0;
        val3=0;
        val4=0;
        val5=0;

        string args;
        args = string.Format("SYSTEM:FPGAVER?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 6 )
        { return -1; } 
        val0 = Convert.ToByte( paras[0] );
        val1 = Convert.ToByte( paras[1] );
        val2 = Convert.ToByte( paras[2] );
        val3 = Convert.ToByte( paras[3] );
        val4 = Convert.ToByte( paras[4] );
        val5 = Convert.ToByte( paras[5] );
        return 0;
    }

    //!
    //!
    //!char,char
    //!val0,val1
    public int getSYSTEM_HARDVER(  out byte val0, out byte val1 )
    {
        //!查询系统硬件版本，数字
        //!SYSTEM HARDVER

        //! init the out
        val0=0;
        val1=0;

        string args;
        args = string.Format("SYSTEM:HARDVER?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 2 )
        { return -1; } 
        val0 = Convert.ToByte( paras[0] );
        val1 = Convert.ToByte( paras[1] );
        return 0;
    }

    //!
    //!
    //!char,char
    //!val0,val1
    public int getSYSTEM_BOOTVER(  out byte val0, out byte val1 )
    {
        //!查询系统BOOT版本，数字
        //!SYSTEM BOOTVER

        //! init the out
        val0=0;
        val1=0;

        string args;
        args = string.Format("SYSTEM:BOOTVER?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 2 )
        { return -1; } 
        val0 = Convert.ToByte( paras[0] );
        val1 = Convert.ToByte( paras[1] );
        return 0;
    }

    //!
    //!
    //!char,char,char,char,char,char
    //!val0,val1,val2,val3,val4,val5
    public int getSYSTEM_EVENTCODE(  out byte val0, out byte val1, out byte val2, out byte val3, out byte val4, out byte val5 )
    {
        //!查询系统错误，错误码
        //!SYSTEM EVENTCODE

        //! init the out
        val0=0;
        val1=0;
        val2=0;
        val3=0;
        val4=0;
        val5=0;

        string args;
        args = string.Format("SYSTEM:EVENTCODE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 6 )
        { return -1; } 
        val0 = Convert.ToByte( paras[0] );
        val1 = Convert.ToByte( paras[1] );
        val2 = Convert.ToByte( paras[2] );
        val3 = Convert.ToByte( paras[3] );
        val4 = Convert.ToByte( paras[4] );
        val5 = Convert.ToByte( paras[5] );
        return 0;
    }

    //!
    //!
    //!uint16
    //!val0
    public int getSYSTEM_DIOSTATE(  out UInt16 val0 )
    {
        //!查询DI端口当前的状态
        //!SYSTEM DIOSTATE

        //! init the out
        val0=0;

        string args;
        args = string.Format("SYSTEM:DIOSTATE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt16( paras[0] );
        return 0;
    }

    //!ENUM
    //!OFF|ON
    public int setSYSTEM_REVMOTION(  string val0 )
    {
        //!设置反向运行状态
        //!SYSTEM REVMOTION
        string args;
        args = string.Format("SYSTEM:REVMOTION {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_SYSTEM_REVMOTION
    //!val0
    public int getSYSTEM_REVMOTION(  out string val0 )
    {
        //!查询反向运行状态
        //!SYSTEM REVMOTION

        //! init the out
        val0=null;

        string args;
        args = string.Format("SYSTEM:REVMOTION?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!U8
    //!
    public int setSYSTEM_FANPARA(  byte val0 )
    {
        //!设置风扇PWM的占空比和频率，占空比数值是百分比乘100，频率单位Hz
        //!SYSTEM FANPARA
        string args;
        args = string.Format("SYSTEM:FANPARA {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!byte
    //!val0
    public int getSYSTEM_FANPARA(  out byte val0 )
    {
        //!查询风扇PWM的占空比和频率
        //!SYSTEM FANPARA

        //! init the out
        val0=0;

        string args;
        args = string.Format("SYSTEM:FANPARA?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!ENUM,U8
    //!BASE|BIGARM|SMALLARM|WRIST
    public int setSYSTEM_ARMLEDPARA(  string val0, byte val1 )
    {
        //!设置机械臂LEDPWM的占空比和频率，占空比数值是百分比乘100，频率单位Hz
        //!SYSTEM ARMLEDPARA
        string args;
        args = string.Format("SYSTEM:ARMLEDPARA {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!BASE|BIGARM|SMALLARM|WRIST
    //!byte
    //!val1
    public int getSYSTEM_ARMLEDPARA(  string val0 ,  out byte val1 )
    {
        //!查询机械臂LEDPWM的占空比和频率
        //!SYSTEM ARMLEDPARA

        //! init the out
        val1=0;

        string args;
        args = string.Format("SYSTEM:ARMLEDPARA? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!ENUM
    //!4800|7200|9600|14400|19200|38400|57600|115200|128000
    public int setRS232_BAUD(  string val0 )
    {
        //!设置波特率，单位bps
        //!RS232 BAUD
        string args;
        args = string.Format("RS232:BAUD {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_RS232_BAUD
    //!val0
    public int getRS232_BAUD(  out string val0 )
    {
        //!查询RS232波特率
        //!RS232 BAUD

        //! init the out
        val0=null;

        string args;
        args = string.Format("RS232:BAUD?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM
    //!8|9
    public int setRS232_WORDLEN(  string val0 )
    {
        //!设置数据长度
        //!RS232 WORDLEN
        string args;
        args = string.Format("RS232:WORDLEN {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_RS232_WORDLEN
    //!val0
    public int getRS232_WORDLEN(  out string val0 )
    {
        //!查询RS232数据长度
        //!RS232 WORDLEN

        //! init the out
        val0=null;

        string args;
        args = string.Format("RS232:WORDLEN?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM
    //!NONE|RTS|CTS|RTS&CTS
    public int setRS232_FLOWCTL(  string val0 )
    {
        //!打开或关闭流控制
        //!RS232 FLOWCTL
        string args;
        args = string.Format("RS232:FLOWCTL {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_RS232_FLOWCTL
    //!val0
    public int getRS232_FLOWCTL(  out string val0 )
    {
        //!查询RS232流控制状态
        //!RS232 FLOWCTL

        //! init the out
        val0=null;

        string args;
        args = string.Format("RS232:FLOWCTL?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM
    //!NONE|EVEN|ODD
    public int setRS232_PARITY(  string val0 )
    {
        //!校验方式
        //!RS232 PARITY
        string args;
        args = string.Format("RS232:PARITY {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_RS232_PARITY
    //!val0
    public int getRS232_PARITY(  out string val0 )
    {
        //!查询RS232校验方式
        //!RS232 PARITY

        //! init the out
        val0=null;

        string args;
        args = string.Format("RS232:PARITY?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM
    //!1|0.5|2|1.5
    public int setRS232_STOPBIT(  string val0 )
    {
        //!设置停止位
        //!RS232 STOPBIT
        string args;
        args = string.Format("RS232:STOPBIT {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_RS232_STOPBIT
    //!val0
    public int getRS232_STOPBIT(  out string val0 )
    {
        //!查询RS232停止位
        //!RS232 STOPBIT

        //! init the out
        val0=null;

        string args;
        args = string.Format("RS232:STOPBIT?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!
    //!
    public int setRS232_APPLYPARA( )
    {
        //!按照之前设置的参数配置接口
        //!RS232 APPLYPARA
        string args;
        args = string.Format("RS232:APPLYPARA"   );
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!A|B
    public int setCAN_TYPE(  string val0 )
    {
        //!设置使用的CAN版本
        //!CAN TYPE
        string args;
        args = string.Format("CAN:TYPE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_CAN_TYPE
    //!val0
    public int getCAN_TYPE(  out string val0 )
    {
        //!查询CAN接口当前的版本
        //!CAN TYPE

        //! init the out
        val0=null;

        string args;
        args = string.Format("CAN:TYPE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM
    //!1000|500|250|125|100|50|20|10
    public int setCAN_BAUD(  string val0 )
    {
        //!设置波特率，单位kBps
        //!CAN BAUD
        string args;
        args = string.Format("CAN:BAUD {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_CAN_BAUD
    //!val0
    public int getCAN_BAUD(  out string val0 )
    {
        //!查询节点的CAN波特率
        //!CAN BAUD

        //! init the out
        val0=null;

        string args;
        args = string.Format("CAN:BAUD?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!U8
    //!
    public int setCAN_GROUP(  byte val0 )
    {
        //!设置分组信息
        //!CAN GROUP
        string args;
        args = string.Format("CAN:GROUP {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!byte
    //!val0
    public int getCAN_GROUP(  out byte val0 )
    {
        //!查询节点的分组信息
        //!CAN GROUP

        //! init the out
        val0=0;

        string args;
        args = string.Format("CAN:GROUP?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!U32
    //!
    public int setCAN_SENDID(  UInt32 val0 )
    {
        //!设置节点的CANID
        //!CAN SENDID
        string args;
        args = string.Format("CAN:SENDID {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!uint32
    //!val0
    public int getCAN_SENDID(  out UInt32 val0 )
    {
        //!查询节点的CANID
        //!CAN SENDID

        //! init the out
        val0=0;

        string args;
        args = string.Format("CAN:SENDID?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U32
    //!
    public int setCAN_RECEIVEID(  UInt32 val0 )
    {
        //!设置接收D，单对单的接收ID
        //!CAN RECEIVEID
        string args;
        args = string.Format("CAN:RECEIVEID {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!uint32
    //!val0
    public int getCAN_RECEIVEID(  out UInt32 val0 )
    {
        //!查询节点的接收ID
        //!CAN RECEIVEID

        //! init the out
        val0=0;

        string args;
        args = string.Format("CAN:RECEIVEID?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U32
    //!
    public int setCAN_GROUPID1(  UInt32 val0 )
    {
        //!设置组ID1，分组后的接收ID1
        //!CAN GROUPID1
        string args;
        args = string.Format("CAN:GROUPID1 {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!uint32
    //!val0
    public int getCAN_GROUPID1(  out UInt32 val0 )
    {
        //!查询节点的组ID1
        //!CAN GROUPID1

        //! init the out
        val0=0;

        string args;
        args = string.Format("CAN:GROUPID1?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U32
    //!
    public int setCAN_GROUPID2(  UInt32 val0 )
    {
        //!设置组ID2，分组后的接收ID2
        //!CAN GROUPID2
        string args;
        args = string.Format("CAN:GROUPID2 {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!uint32
    //!val0
    public int getCAN_GROUPID2(  out UInt32 val0 )
    {
        //!查询节点的组ID2
        //!CAN GROUPID2

        //! init the out
        val0=0;

        string args;
        args = string.Format("CAN:GROUPID2?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U32
    //!
    public int setCAN_BROADCASTID(  UInt32 val0 )
    {
        //!设置广播接收ID
        //!CAN BROADCASTID
        string args;
        args = string.Format("CAN:BROADCASTID {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!uint32
    //!val0
    public int getCAN_BROADCASTID(  out UInt32 val0 )
    {
        //!查询节点的广播ID
        //!CAN BROADCASTID

        //! init the out
        val0=0;

        string args;
        args = string.Format("CAN:BROADCASTID?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!
    //!
    public int setCAN_APPLYPARA( )
    {
        //!按照之前设置的参数配置接口
        //!CAN APPLYPARA
        string args;
        args = string.Format("CAN:APPLYPARA"   );
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!OFF|ON
    public int setCAN_NETMANAGELED(  string val0 )
    {
        //!设置网络管理LED状态
        //!CAN NETMANAGELED
        string args;
        args = string.Format("CAN:NETMANAGELED {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!IDLE|HASH|SIGNATURE
    public int setCAN_NETMANAGESTATE(  string val0 )
    {
        //!设置CAN网络配置状态，IDLE状态下不进行配置，HASH状态下对比哈希值进入状态，SIGNATURE状态下对比电子签名进入配置状态
        //!CAN NETMANAGESTATE
        string args;
        args = string.Format("CAN:NETMANAGESTATE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_CAN_NETMANAGESTATE
    //!val0
    public int getCAN_NETMANAGESTATE(  out string val0 )
    {
        //!查询CAN网络配置状态
        //!CAN NETMANAGESTATE

        //! init the out
        val0=null;

        string args;
        args = string.Format("CAN:NETMANAGESTATE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM,U32
    //!NONE|SENDID|RECEIVEID
    public int setCAN_NETMANAGEID(  string val0, UInt32 val1 )
    {
        //!设置节点的ID
        //!CAN NETMANAGEID
        string args;
        args = string.Format("CAN:NETMANAGEID {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!NONE|SENDID|RECEIVEID
    //!uint32
    //!val1
    public int getCAN_NETMANAGEID(  string val0 ,  out UInt32 val1 )
    {
        //!查询在配置状态下,为节点设置的ID
        //!CAN NETMANAGEID

        //! init the out
        val1=0;

        string args;
        args = string.Format("CAN:NETMANAGEID? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U32
    //!
    public int setCAN_NETMANAGEHASH(  UInt32 val0 )
    {
        //!发送哈希值，使模块进入配置状态(网络状态为HASH时）
        //!CAN NETMANAGEHASH
        string args;
        args = string.Format("CAN:NETMANAGEHASH {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!uint32
    //!val0
    public int getCAN_NETMANAGEHASH(  out UInt32 val0 )
    {
        //!查询网络管理初级识别符(哈希值)
        //!CAN NETMANAGEHASH

        //! init the out
        val0=0;

        string args;
        args = string.Format("CAN:NETMANAGEHASH?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U32
    //!
    public int setCLOCK_FREQUENCY(  UInt32 val0 )
    {
        //!设置时钟频率
        //!CLOCK FREQUENCY
        string args;
        args = string.Format("CLOCK:FREQUENCY {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!uint32
    //!val0
    public int getCLOCK_FREQUENCY(  out UInt32 val0 )
    {
        //!查询时钟频率
        //!CLOCK FREQUENCY

        //! init the out
        val0=0;

        string args;
        args = string.Format("CLOCK:FREQUENCY?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!ENUM
    //!OFF|ON
    public int setCLOCK_SYNCREGISTER(  string val0 )
    {
        //!设置时钟同步寄存器状态，先配置寄存器状态才能进行时钟计数
        //!CLOCK SYNCREGISTER
        string args;
        args = string.Format("CLOCK:SYNCREGISTER {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_SYSTEM_REVMOTION
    //!val0
    public int getCLOCK_SYNCREGISTER(  out string val0 )
    {
        //!查询时钟同步寄存器状态
        //!CLOCK SYNCREGISTER

        //! init the out
        val0=null;

        string args;
        args = string.Format("CLOCK:SYNCREGISTER?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM
    //!RADIOID|RECEIVEID|GROUPID1|GROUPID2
    public int setCLOCK_STARTTYPE(  string val0 )
    {
        //!设置启动类型
        //!CLOCK STARTTYPE
        string args;
        args = string.Format("CLOCK:STARTTYPE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_CLOCK_STARTTYPE
    //!val0
    public int getCLOCK_STARTTYPE(  out string val0 )
    {
        //!查询启动类型
        //!CLOCK STARTTYPE

        //! init the out
        val0=null;

        string args;
        args = string.Format("CLOCK:STARTTYPE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM
    //!END|START
    public int setCLOCK_SYNCSTATE(  string val0 )
    {
        //!设置时钟同步状态，开启或结束时需要先使用配置同步寄存器
        //!CLOCK SYNCSTATE
        string args;
        args = string.Format("CLOCK:SYNCSTATE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_CLOCK_SYNCSTATE
    //!val0
    public int getCLOCK_SYNCSTATE(  out string val0 )
    {
        //!查询时钟同步状态
        //!CLOCK SYNCSTATE

        //! init the out
        val0=null;

        string args;
        args = string.Format("CLOCK:SYNCSTATE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!
    //!
    //!uint32
    //!val0
    public int getCLOCK_COUNT(  out UInt32 val0 )
    {
        //!查询时钟计数值
        //!CLOCK COUNT

        //! init the out
        val0=0;

        string args;
        args = string.Format("CLOCK:COUNT?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],RESET|STOP|RUN|PREPARE|EMERGSTOP,MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    public int setMOTION_SWITCH(  byte val0, string val1, string val2 )
    {
        //!运动切换，使节点进入相应状态：IDLE|CALCEND|STANDBY|RUNNING
        //!MOTION SWITCH
        string args;
        args = string.Format("MOTION:SWITCH {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_MOTION_STATE_2
    //!val2
    public int getMOTION_STATE(  byte val0, string val1 ,  out string val2 )
    {
        //!下位机运动系统状态。上报方式为主动时，由节点发送给上位机或者控制器，上位机或控制器收到需要回复同样的命令，如果没收到回复节点会一直按时间间隔发送，直到回复；上报方式为查询时，不需要带参数，查询后下位机返回状态
        //!MOTION STATE

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTION:STATE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],ACTIVE|QUERY
    public int setMOTION_STATEREPORT(  byte val0, string val1 )
    {
        //!设置MOTION:STATE的上报方式，主动上报还是查询上报
        //!MOTION STATEREPORT
        string args;
        args = string.Format("MOTION:STATEREPORT {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_MOTION_STATEREPORT
    //!val1
    public int getMOTION_STATEREPORT(  byte val0 ,  out string val1 )
    {
        //!查询MOTION:STATE的上报方式
        //!MOTION STATEREPORT

        //! init the out
        val1=null;

        string args;
        args = string.Format("MOTION:STATEREPORT? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],SOFTWARE|DIGITALIO|CAN|ALL
    public int setMOTION_STARTSOURCE(  byte val0, string val1 )
    {
        //!设置启动源
        //!MOTION STARTSOURCE
        string args;
        args = string.Format("MOTION:STARTSOURCE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_MOTION_STARTSOURCE
    //!val1
    public int getMOTION_STARTSOURCE(  byte val0 ,  out string val1 )
    {
        //!查询启动源
        //!MOTION STARTSOURCE

        //! init the out
        val1=null;

        string args;
        args = string.Format("MOTION:STARTSOURCE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],RADIOID|RECEIVEID|GROUPID1|GROUPID2
    public int setMOTION_STARTTYPE(  byte val0, string val1 )
    {
        //!设置启动类型
        //!MOTION STARTTYPE
        string args;
        args = string.Format("MOTION:STARTTYPE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_CLOCK_STARTTYPE
    //!val1
    public int getMOTION_STARTTYPE(  byte val0 ,  out string val1 )
    {
        //!查询启动类型
        //!MOTION STARTTYPE

        //! init the out
        val1=null;

        string args;
        args = string.Format("MOTION:STARTTYPE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,F32
    //![CHANNUM]
    public int setMOTION_MAXSPEED(  byte val0, float val1 )
    {
        //!设置运行的速度上限
        //!MOTION MAXSPEED
        string args;
        args = string.Format("MOTION:MAXSPEED {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!f32
    //!val1
    public int getMOTION_MAXSPEED(  byte val0 ,  out float val1 )
    {
        //!查询运行的速度上限
        //!MOTION MAXSPEED

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:MAXSPEED? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,F32
    //![CHANNUM]
    public int setMOTION_MINSPEED(  byte val0, float val1 )
    {
        //!设置运行的速度下限
        //!MOTION MINSPEED
        string args;
        args = string.Format("MOTION:MINSPEED {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!f32
    //!val1
    public int getMOTION_MINSPEED(  byte val0 ,  out float val1 )
    {
        //!查询运行的速度下限
        //!MOTION MINSPEED

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:MINSPEED? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,F32
    //![CHANNUM]
    public int setMOTION_MAXPOSITION(  byte val0, float val1 )
    {
        //!设置运行的位置上限
        //!MOTION MAXPOSITION
        string args;
        args = string.Format("MOTION:MAXPOSITION {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!f32
    //!val1
    public int getMOTION_MAXPOSITION(  byte val0 ,  out float val1 )
    {
        //!查询运行的位置上限
        //!MOTION MAXPOSITION

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:MAXPOSITION? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,F32
    //![CHANNUM]
    public int setMOTION_MINPOSITION(  byte val0, float val1 )
    {
        //!设置运行的位置下限
        //!MOTION MINPOSITION
        string args;
        args = string.Format("MOTION:MINPOSITION {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!f32
    //!val1
    public int getMOTION_MINPOSITION(  byte val0 ,  out float val1 )
    {
        //!查询运行的位置下限
        //!MOTION MINPOSITION

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:MINPOSITION? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,F32
    //![CHANNUM]
    public int setMOTION_MAXTORQUE(  byte val0, float val1 )
    {
        //!设置运行的力矩上限
        //!MOTION MAXTORQUE
        string args;
        args = string.Format("MOTION:MAXTORQUE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!f32
    //!val1
    public int getMOTION_MAXTORQUE(  byte val0 ,  out float val1 )
    {
        //!查询运行的力矩上限
        //!MOTION MAXTORQUE

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:MAXTORQUE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,F32
    //![CHANNUM]
    public int setMOTION_MINTORQUE(  byte val0, float val1 )
    {
        //!设置运行的力矩下限
        //!MOTION MINTORQUE
        string args;
        args = string.Format("MOTION:MINTORQUE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!f32
    //!val1
    public int getMOTION_MINTORQUE(  byte val0 ,  out float val1 )
    {
        //!查询运行的力矩下限
        //!MOTION MINTORQUE

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:MINTORQUE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,F32
    //![CHANNUM]
    public int setMOTION_MAXACCELERATION(  byte val0, float val1 )
    {
        //!设置运行的加速度上限
        //!MOTION MAXACCELERATION
        string args;
        args = string.Format("MOTION:MAXACCELERATION {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!f32
    //!val1
    public int getMOTION_MAXACCELERATION(  byte val0 ,  out float val1 )
    {
        //!查询运行的加速度上限
        //!MOTION MAXACCELERATION

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:MAXACCELERATION? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,F32
    //![CHANNUM]
    public int setMOTION_MINACCELERATION(  byte val0, float val1 )
    {
        //!设置运行的加速度下限
        //!MOTION MINACCELERATION
        string args;
        args = string.Format("MOTION:MINACCELERATION {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!f32
    //!val1
    public int getMOTION_MINACCELERATION(  byte val0 ,  out float val1 )
    {
        //!查询运行的加速度下限
        //!MOTION MINACCELERATION

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:MINACCELERATION? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,U32
    //![CHANNUM]
    public int setMOTION_ORIGIN(  byte val0, UInt32 val1 )
    {
        //!设置当前位置为原点
        //!MOTION ORIGIN
        string args;
        args = string.Format("MOTION:ORIGIN {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!uint32
    //!val1
    public int getMOTION_ORIGIN(  byte val0 ,  out UInt32 val1 )
    {
        //!查询原点位置
        //!MOTION ORIGIN

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:ORIGIN? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8
    //![CHANNUM]
    public int setMOTION_GOORIGIN(  byte val0 )
    {
        //!回到原点
        //!MOTION GOORIGIN
        string args;
        args = string.Format("MOTION:GOORIGIN {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],OFF|ON
    public int setMOTION_OFFSETSTATE(  byte val0, string val1 )
    {
        //!设置电机未运动时发生位移是否上报
        //!MOTION OFFSETSTATE
        string args;
        args = string.Format("MOTION:OFFSETSTATE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_SYSTEM_REVMOTION
    //!val1
    public int getMOTION_OFFSETSTATE(  byte val0 ,  out string val1 )
    {
        //!查询电机未运动时发生位移是否上报
        //!MOTION OFFSETSTATE

        //! init the out
        val1=null;

        string args;
        args = string.Format("MOTION:OFFSETSTATE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,S32
    //![CHANNUM]
    public int setMOTION_OFFSET(  byte val0, Int32 val1 )
    {
        //!电机未运动时发生的位移
        //!MOTION OFFSET
        string args;
        args = string.Format("MOTION:OFFSET {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!int16
    //!val1
    public int getMOTION_COUNTCIRCLE(  byte val0 ,  out Int16 val1 )
    {
        //!查询电机的圈数统计值，小于0为反转，大于0为正转
        //!MOTION COUNTCIRCLE

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:COUNTCIRCLE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToInt16( paras[0] );
        return 0;
    }

    //!U8
    //![CHANNUM]
    //!int32
    //!val1
    public int getMOTION_ABCOUNT(  byte val0 ,  out Int32 val1 )
    {
        //!查询电机的AB相计数，小于0为反转，大于0为正转
        //!MOTION ABCOUNT

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:ABCOUNT? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToInt32( paras[0] );
        return 0;
    }

    //!U8
    //![CHANNUM]
    public int setMOTION_ABCOUNTCLEAR(  byte val0 )
    {
        //!清零编码器
        //!MOTION ABCOUNTCLEAR
        string args;
        args = string.Format("MOTION:ABCOUNTCLEAR {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!uint32
    //!val1
    public int getMOTION_RUNTIMEL(  byte val0 ,  out UInt32 val1 )
    {
        //!查询电机的最后一次运行的运行时间，低32位，单位ms
        //!MOTION RUNTIMEL

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:RUNTIMEL? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8
    //![CHANNUM]
    //!uint32
    //!val1
    public int getMOTION_RUNTIMEH(  byte val0 ,  out UInt32 val1 )
    {
        //!查询电机的最后一次运行的运行时间，高32位，单位ms
        //!MOTION RUNTIMEH

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTION:RUNTIMEH? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8,ENUM,U8
    //![CHANNUM],GROUP1|GROUP2
    public int setIDENTITY_GROUP(  byte val0, string val1, byte val2 )
    {
        //!设置通道的分组号
        //!IDENTITY GROUP
        string args;
        args = string.Format("IDENTITY:GROUP {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],GROUP1|GROUP2
    //!byte
    //!val2
    public int getIDENTITY_GROUP(  byte val0, string val1 ,  out byte val2 )
    {
        //!查询通道的分组号
        //!IDENTITY GROUP

        //! init the out
        val2=0;

        string args;
        args = string.Format("IDENTITY:GROUP? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!ENUM
    //!OFF|ON
    public int setIDENTITY_DISTDEVICE(  string val0 )
    {
        //!设置设备的识别状态
        //!IDENTITY DISTDEVICE
        string args;
        args = string.Format("IDENTITY:DISTDEVICE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_SYSTEM_REVMOTION
    //!val0
    public int getIDENTITY_DISTDEVICE(  out string val0 )
    {
        //!查询设备的识别状态
        //!IDENTITY DISTDEVICE

        //! init the out
        val0=null;

        string args;
        args = string.Format("IDENTITY:DISTDEVICE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],SINANJU|MEGATRON|GOUF,S1|S2|S3|S4
    public int setIDENTITY_LABEL(  byte val0, string val1, string val2 )
    {
        //!设置通道的标签(后续会做一个标签列表)
        //!IDENTITY LABEL
        string args;
        args = string.Format("IDENTITY:LABEL {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_IDENTITY_LABEL,MRQ_IDENTITY_LABEL_1
    //!val1,val2
    public int getIDENTITY_LABEL(  byte val0 ,  out string val1, out string val2 )
    {
        //!查询通道的标签
        //!IDENTITY LABEL

        //! init the out
        val1=null;
        val2=null;

        string args;
        args = string.Format("IDENTITY:LABEL? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 2 )
        { return -1; } 
        val1 = paras[0];
        val2 = paras[1];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],1.8|0.9|15|7.5
    public int setMOTOR_STEPANGLE(  byte val0, string val1 )
    {
        //!设置电机步距角
        //!MOTOR STEPANGLE
        string args;
        args = string.Format("MOTOR:STEPANGLE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_MOTOR_STEPANGLE
    //!val1
    public int getMOTOR_STEPANGLE(  byte val0 ,  out string val1 )
    {
        //!查询电机步距角
        //!MOTOR STEPANGLE

        //! init the out
        val1=null;

        string args;
        args = string.Format("MOTOR:STEPANGLE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],ROTARY|LINEAR
    public int setMOTOR_TYPE(  byte val0, string val1 )
    {
        //!设置电机运动类型
        //!MOTOR TYPE
        string args;
        args = string.Format("MOTOR:TYPE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_MOTOR_TYPE
    //!val1
    public int getMOTOR_TYPE(  byte val0 ,  out string val1 )
    {
        //!查询电机运动类型
        //!MOTOR TYPE

        //! init the out
        val1=null;

        string args;
        args = string.Format("MOTOR:TYPE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],ANGLE|RADIAN|MILLIMETER
    public int setMOTOR_POSITIONUNIT(  byte val0, string val1 )
    {
        //!设置电机和位置相关参数的单位，包括位置和速度
        //!MOTOR POSITIONUNIT
        string args;
        args = string.Format("MOTOR:POSITIONUNIT {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_MOTOR_POSITIONUNIT
    //!val1
    public int getMOTOR_POSITIONUNIT(  byte val0 ,  out string val1 )
    {
        //!查询电机和位置相关参数的单位，包括位置和速度
        //!MOTOR POSITIONUNIT

        //! init the out
        val1=null;

        string args;
        args = string.Format("MOTOR:POSITIONUNIT? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,U16
    //![CHANNUM]
    public int setMOTOR_GEARRATIONUM(  byte val0, UInt16 val1 )
    {
        //!设置电机减速比分子
        //!MOTOR GEARRATIONUM
        string args;
        args = string.Format("MOTOR:GEARRATIONUM {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!uint16
    //!val1
    public int getMOTOR_GEARRATIONUM(  byte val0 ,  out UInt16 val1 )
    {
        //!查询电机减速比分子
        //!MOTOR GEARRATIONUM

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTOR:GEARRATIONUM? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt16( paras[0] );
        return 0;
    }

    //!U8,U16
    //![CHANNUM]
    public int setMOTOR_GEARRATIODEN(  byte val0, UInt16 val1 )
    {
        //!设置电机减速比分母
        //!MOTOR GEARRATIODEN
        string args;
        args = string.Format("MOTOR:GEARRATIODEN {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!uint16
    //!val1
    public int getMOTOR_GEARRATIODEN(  byte val0 ,  out UInt16 val1 )
    {
        //!查询电机减速比分母
        //!MOTOR GEARRATIODEN

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTOR:GEARRATIODEN? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt16( paras[0] );
        return 0;
    }

    //!U8,F32
    //![CHANNUM]
    public int setMOTOR_LEAD(  byte val0, float val1 )
    {
        //!设置直线运动的导程
        //!MOTOR LEAD
        string args;
        args = string.Format("MOTOR:LEAD {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!f32
    //!val1
    public int getMOTOR_LEAD(  byte val0 ,  out float val1 )
    {
        //!查询直线运动的导程
        //!MOTOR LEAD

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTOR:LEAD? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,F32
    //![CHANNUM]
    public int setMOTOR_PEAKSPEED(  byte val0, float val1 )
    {
        //!设置电机峰值速度
        //!MOTOR PEAKSPEED
        string args;
        args = string.Format("MOTOR:PEAKSPEED {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!f32
    //!val1
    public int getMOTOR_PEAKSPEED(  byte val0 ,  out float val1 )
    {
        //!查询电机峰值速度
        //!MOTOR PEAKSPEED

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTOR:PEAKSPEED? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,F32
    //![CHANNUM]
    public int setMOTOR_PEAKACCELERATION(  byte val0, float val1 )
    {
        //!设置电机峰值加速度
        //!MOTOR PEAKACCELERATION
        string args;
        args = string.Format("MOTOR:PEAKACCELERATION {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!f32
    //!val1
    public int getMOTOR_PEAKACCELERATION(  byte val0 ,  out float val1 )
    {
        //!查询电机峰值加速度
        //!MOTOR PEAKACCELERATION

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTOR:PEAKACCELERATION? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],8|11|14|17|23|24
    public int setMOTOR_SIZE(  byte val0, string val1 )
    {
        //!设置电机尺寸
        //!MOTOR SIZE
        string args;
        args = string.Format("MOTOR:SIZE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_MOTOR_SIZE
    //!val1
    public int getMOTOR_SIZE(  byte val0 ,  out string val1 )
    {
        //!查询电机尺寸
        //!MOTOR SIZE

        //! init the out
        val1=null;

        string args;
        args = string.Format("MOTOR:SIZE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,U8
    //![CHANNUM]
    public int setMOTOR_VOLTAGE(  byte val0, byte val1 )
    {
        //!设置电机工作电压
        //!MOTOR VOLTAGE
        string args;
        args = string.Format("MOTOR:VOLTAGE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!byte
    //!val1
    public int getMOTOR_VOLTAGE(  byte val0 ,  out byte val1 )
    {
        //!查询电机工作电压
        //!MOTOR VOLTAGE

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTOR:VOLTAGE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!U8,U8
    //![CHANNUM]
    public int setMOTOR_CURRENT(  byte val0, byte val1 )
    {
        //!设置电机工作最大电流，单位0.1A
        //!MOTOR CURRENT
        string args;
        args = string.Format("MOTOR:CURRENT {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!byte
    //!val1
    public int getMOTOR_CURRENT(  byte val0 ,  out byte val1 )
    {
        //!查询电机工作最大电流
        //!MOTOR CURRENT

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTOR:CURRENT? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!U8,F32
    //![CHANNUM]
    public int setMOTOR_BACKLASH(  byte val0, float val1 )
    {
        //!设置电机的反向间隙
        //!MOTOR BACKLASH
        string args;
        args = string.Format("MOTOR:BACKLASH {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!f32
    //!val1
    public int getMOTOR_BACKLASH(  byte val0 ,  out float val1 )
    {
        //!查询电机的反向间隙
        //!MOTOR BACKLASH

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTOR:BACKLASH? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,U16
    //![CHANNUM]
    public int setMOTOR_BACKLASHTEST(  byte val0, UInt16 val1 )
    {
        //!开始电机反向间隙测试
        //!MOTOR BACKLASHTEST
        string args;
        args = string.Format("MOTOR:BACKLASHTEST {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!uint16
    //!val1
    public int getMOTOR_BACKLASHTEST(  byte val0 ,  out UInt16 val1 )
    {
        //!查询电机反向间隙测试结果
        //!MOTOR BACKLASHTEST

        //! init the out
        val1=0;

        string args;
        args = string.Format("MOTOR:BACKLASHTEST? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt16( paras[0] );
        return 0;
    }

    //!U8,U32
    //![CHANNUM]
    public int setENCODER_LINENUM(  byte val0, UInt32 val1 )
    {
        //!设置编码器每圈线数(只有3、4轴两个型号支持编码器，并且和TRIGR互斥，只能同时使能一个；6、7、8轴目前不支持编码器)
        //!ENCODER LINENUM
        string args;
        args = string.Format("ENCODER:LINENUM {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!uint32
    //!val1
    public int getENCODER_LINENUM(  byte val0 ,  out UInt32 val1 )
    {
        //!查询编码器每圈线数
        //!ENCODER LINENUM

        //! init the out
        val1=0;

        string args;
        args = string.Format("ENCODER:LINENUM? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],1|3
    public int setENCODER_CHANNELNUM(  byte val0, string val1 )
    {
        //!设置编码器通道数
        //!ENCODER CHANNELNUM
        string args;
        args = string.Format("ENCODER:CHANNELNUM {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_ENCODER_CHANNELNUM
    //!val1
    public int getENCODER_CHANNELNUM(  byte val0 ,  out string val1 )
    {
        //!查询编码器通道数
        //!ENCODER CHANNELNUM

        //! init the out
        val1=null;

        string args;
        args = string.Format("ENCODER:CHANNELNUM? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],INCREMENTAL|ABSOLUTE
    public int setENCODER_TYPE(  byte val0, string val1 )
    {
        //!设置编码器类型
        //!ENCODER TYPE
        string args;
        args = string.Format("ENCODER:TYPE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_ENCODER_TYPE
    //!val1
    public int getENCODER_TYPE(  byte val0 ,  out string val1 )
    {
        //!查询编码器类型
        //!ENCODER TYPE

        //! init the out
        val1=null;

        string args;
        args = string.Format("ENCODER:TYPE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],SINGLE|DOUBLE|QUADRUPLE
    public int setENCODER_MULTIPLE(  byte val0, string val1 )
    {
        //!设置编码器信号的倍乘
        //!ENCODER MULTIPLE
        string args;
        args = string.Format("ENCODER:MULTIPLE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_ENCODER_MULTIPLE
    //!val1
    public int getENCODER_MULTIPLE(  byte val0 ,  out string val1 )
    {
        //!查询编码器信号的倍乘
        //!ENCODER MULTIPLE

        //! init the out
        val1=null;

        string args;
        args = string.Format("ENCODER:MULTIPLE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],NONE|OFF|ON
    public int setENCODER_STATE(  byte val0, string val1 )
    {
        //!设置编码器状态，编码器和DI互斥，DI使能后如果编码器使能则会自动修改为失能，并上传状态；同样的编码器使能时会将DI失能，并上传状态。
        //!ENCODER STATE
        string args;
        args = string.Format("ENCODER:STATE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_ENCODER_STATE
    //!val1
    public int getENCODER_STATE(  byte val0 ,  out string val1 )
    {
        //!查询编码器状态
        //!ENCODER STATE

        //! init the out
        val1=null;

        string args;
        args = string.Format("ENCODER:STATE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,U8
    //![CHANNUM]
    public int setENCODER_FEEDBACKRATIO(  byte val0, byte val1 )
    {
        //!设置LVT模式下编码器反馈比
        //!ENCODER FEEDBACKRATIO
        string args;
        args = string.Format("ENCODER:FEEDBACKRATIO {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!byte
    //!val1
    public int getENCODER_FEEDBACKRATIO(  byte val0 ,  out byte val1 )
    {
        //!查询LVT模式下编码器反馈比
        //!ENCODER FEEDBACKRATIO

        //! init the out
        val1=0;

        string args;
        args = string.Format("ENCODER:FEEDBACKRATIO? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,END|CLEAR
    public int setMOTIONPLAN_PVTCONFIG(  byte val0, string val1, string val2 )
    {
        //!设置当前PVT的配置状态
        //!MOTIONPLAN PVTCONFIG
        string args;
        args = string.Format("MOTIONPLAN:PVTCONFIG {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_MOTIONPLAN_PVTCONFIG_1
    //!val2
    public int getMOTIONPLAN_PVTCONFIG(  byte val0, string val1 ,  out string val2 )
    {
        //!查询当前PVT的配置状态
        //!MOTIONPLAN PVTCONFIG

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTIONPLAN:PVTCONFIG? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,OFF|ON|CALC
    public int setMOTIONPLAN_PRESETSTATE(  byte val0, string val1, string val2 )
    {
        //!设置PRESET执行模式
        //!MOTIONPLAN PRESETSTATE
        string args;
        args = string.Format("MOTIONPLAN:PRESETSTATE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_MOTIONPLAN_PRESETSTATE_1
    //!val2
    public int getMOTIONPLAN_PRESETSTATE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询PRESET状态
        //!MOTIONPLAN PRESETSTATE

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTIONPLAN:PRESETSTATE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,CYCLE|FIFO
    public int setMOTIONPLAN_EXECUTEMODE(  byte val0, string val1, string val2 )
    {
        //!设置PVT执行模式
        //!MOTIONPLAN EXECUTEMODE
        string args;
        args = string.Format("MOTIONPLAN:EXECUTEMODE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_MOTIONPLAN_EXECUTEMODE_1
    //!val2
    public int getMOTIONPLAN_EXECUTEMODE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询PVT执行模式
        //!MOTIONPLAN EXECUTEMODE

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTIONPLAN:EXECUTEMODE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,CUBICPOLY|TRAPEZOID|SCURVE
    public int setMOTIONPLAN_PLANMODE(  byte val0, string val1, string val2 )
    {
        //!设置轨迹规划模式
        //!MOTIONPLAN PLANMODE
        string args;
        args = string.Format("MOTIONPLAN:PLANMODE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_MOTIONPLAN_PLANMODE_1
    //!val2
    public int getMOTIONPLAN_PLANMODE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询轨迹规划模式
        //!MOTIONPLAN PLANMODE

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTIONPLAN:PLANMODE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,PVT|LVT_CORRECT|LVT_NOCORRECT
    public int setMOTIONPLAN_MOTIONMODE(  byte val0, string val1, string val2 )
    {
        //!设置运动模式，PVT方式、LVT不带时间修正方式、LVT带时间修正方式
        //!MOTIONPLAN MOTIONMODE
        string args;
        args = string.Format("MOTIONPLAN:MOTIONMODE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_MOTIONPLAN_MOTIONMODE_1
    //!val2
    public int getMOTIONPLAN_MOTIONMODE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询运动模式
        //!MOTIONPLAN MOTIONMODE

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTIONPLAN:MOTIONMODE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,4|8|16|32
    public int setMOTIONPLAN_MODIFYDUTY(  byte val0, string val1, string val2 )
    {
        //!设置LVT模式下进行时间调整的占比
        //!MOTIONPLAN MODIFYDUTY
        string args;
        args = string.Format("MOTIONPLAN:MODIFYDUTY {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_MOTIONPLAN_MODIFYDUTY_1
    //!val2
    public int getMOTIONPLAN_MODIFYDUTY(  byte val0, string val1 ,  out string val2 )
    {
        //!查询LVT模式下进行时间调整的占比
        //!MOTIONPLAN MODIFYDUTY

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTIONPLAN:MODIFYDUTY? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!uint16
    //!val2
    public int getMOTIONPLAN_REMAINPOINT(  byte val0, string val1 ,  out UInt16 val2 )
    {
        //!查询剩余可写入的点数，如果Buffer满了则返回0
        //!MOTIONPLAN REMAINPOINT

        //! init the out
        val2=0;

        string args;
        args = string.Format("MOTIONPLAN:REMAINPOINT? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToUInt16( paras[0] );
        return 0;
    }

    //!U8,ENUM,U32
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    public int setMOTIONPLAN_CYCLENUM(  byte val0, string val1, UInt32 val2 )
    {
        //!设置循环模式下的循环数
        //!MOTIONPLAN CYCLENUM
        string args;
        args = string.Format("MOTIONPLAN:CYCLENUM {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!uint32
    //!val2
    public int getMOTIONPLAN_CYCLENUM(  byte val0, string val1 ,  out UInt32 val2 )
    {
        //!查询循环模式下的循环数
        //!MOTIONPLAN CYCLENUM

        //! init the out
        val2=0;

        string args;
        args = string.Format("MOTIONPLAN:CYCLENUM? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8,ENUM,U8
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    public int setMOTIONPLAN_WARNPOINT(  byte val0, string val1, byte val2 )
    {
        //!设置FIFO模式下警告点，当剩余的可执行PVT点小于等于WARNPOINT时，发送警告通知
        //!MOTIONPLAN WARNPOINT
        string args;
        args = string.Format("MOTIONPLAN:WARNPOINT {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!byte
    //!val2
    public int getMOTIONPLAN_WARNPOINT(  byte val0, string val1 ,  out byte val2 )
    {
        //!查询FIFO模式下警告点
        //!MOTIONPLAN WARNPOINT

        //! init the out
        val2=0;

        string args;
        args = string.Format("MOTIONPLAN:WARNPOINT? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,STOP|HOLD
    public int setMOTIONPLAN_ENDSTATE(  byte val0, string val1, string val2 )
    {
        //!设置PVT结束状态
        //!MOTIONPLAN ENDSTATE
        string args;
        args = string.Format("MOTIONPLAN:ENDSTATE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_MOTIONPLAN_ENDSTATE_1
    //!val2
    public int getMOTIONPLAN_ENDSTATE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询PVT结束状态
        //!MOTIONPLAN ENDSTATE

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTIONPLAN:ENDSTATE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!uint32
    //!val2
    public int getMOTIONPLAN_BUFFERSIZE(  byte val0, string val1 ,  out UInt32 val2 )
    {
        //!查询主波表Buffer的大小
        //!MOTIONPLAN BUFFERSIZE

        //! init the out
        val2=0;

        string args;
        args = string.Format("MOTIONPLAN:BUFFERSIZE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,IMMEDIATE|DISTANCE|UNTILEND
    public int setMOTIONPLAN_STOPMODE(  byte val0, string val1, string val2 )
    {
        //!设置急停模式
        //!MOTIONPLAN STOPMODE
        string args;
        args = string.Format("MOTIONPLAN:STOPMODE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_MOTIONPLAN_STOPMODE_1
    //!val2
    public int getMOTIONPLAN_STOPMODE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询急停模式
        //!MOTIONPLAN STOPMODE

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTIONPLAN:STOPMODE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,F32
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    public int setMOTIONPLAN_STOPDISTANCE(  byte val0, string val1, float val2 )
    {
        //!设置急停的距离值
        //!MOTIONPLAN STOPDISTANCE
        string args;
        args = string.Format("MOTIONPLAN:STOPDISTANCE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!f32
    //!val2
    public int getMOTIONPLAN_STOPDISTANCE(  byte val0, string val1 ,  out float val2 )
    {
        //!查询急停的距离值
        //!MOTIONPLAN STOPDISTANCE

        //! init the out
        val2=0;

        string args;
        args = string.Format("MOTIONPLAN:STOPDISTANCE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,ENUM,F32
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    public int setMOTIONPLAN_STOPTIME(  byte val0, string val1, float val2 )
    {
        //!设置急停的时间值
        //!MOTIONPLAN STOPTIME
        string args;
        args = string.Format("MOTIONPLAN:STOPTIME {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!f32
    //!val2
    public int getMOTIONPLAN_STOPTIME(  byte val0, string val1 ,  out float val2 )
    {
        //!查询急停的时间值
        //!MOTIONPLAN STOPTIME

        //! init the out
        val2=0;

        string args;
        args = string.Format("MOTIONPLAN:STOPTIME? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,OFF|ON
    public int setMOTIONPLAN_OOSLINESTATE(  byte val0, string val1, string val2 )
    {
        //!设置线间失步告警状态
        //!MOTIONPLAN OOSLINESTATE
        string args;
        args = string.Format("MOTIONPLAN:OOSLINESTATE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_SYSTEM_REVMOTION
    //!val2
    public int getMOTIONPLAN_OOSLINESTATE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询线间失步告警状态
        //!MOTIONPLAN OOSLINESTATE

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTIONPLAN:OOSLINESTATE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,U16
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    public int setMOTIONPLAN_OOSLINEOUTNUM(  byte val0, string val1, UInt16 val2 )
    {
        //!设置线间失步阈值
        //!MOTIONPLAN OOSLINEOUTNUM
        string args;
        args = string.Format("MOTIONPLAN:OOSLINEOUTNUM {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!uint16
    //!val2
    public int getMOTIONPLAN_OOSLINEOUTNUM(  byte val0, string val1 ,  out UInt16 val2 )
    {
        //!查询线间失步阈值
        //!MOTIONPLAN OOSLINEOUTNUM

        //! init the out
        val2=0;

        string args;
        args = string.Format("MOTIONPLAN:OOSLINEOUTNUM? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToUInt16( paras[0] );
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,NONE|ALARM|STOP|ALARM&STOP
    public int setMOTIONPLAN_OOSLINERESPONSE(  byte val0, string val1, string val2 )
    {
        //!设置当步数偏差超过LOSTNUM后的响应
        //!MOTIONPLAN OOSLINERESPONSE
        string args;
        args = string.Format("MOTIONPLAN:OOSLINERESPONSE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
    //!val2
    public int getMOTIONPLAN_OOSLINERESPONSE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询失步响应
        //!MOTIONPLAN OOSLINERESPONSE

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTIONPLAN:OOSLINERESPONSE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,OFF|ON
    public int setMOTIONPLAN_OOSTOTALSTATE(  byte val0, string val1, string val2 )
    {
        //!设置总的失步告警状态
        //!MOTIONPLAN OOSTOTALSTATE
        string args;
        args = string.Format("MOTIONPLAN:OOSTOTALSTATE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_SYSTEM_REVMOTION
    //!val2
    public int getMOTIONPLAN_OOSTOTALSTATE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询总的失步告警状态
        //!MOTIONPLAN OOSTOTALSTATE

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTIONPLAN:OOSTOTALSTATE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,U32
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    public int setMOTIONPLAN_OOSTOTALOUTNUM(  byte val0, string val1, UInt32 val2 )
    {
        //!设置总的失步阈值
        //!MOTIONPLAN OOSTOTALOUTNUM
        string args;
        args = string.Format("MOTIONPLAN:OOSTOTALOUTNUM {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!uint32
    //!val2
    public int getMOTIONPLAN_OOSTOTALOUTNUM(  byte val0, string val1 ,  out UInt32 val2 )
    {
        //!查询总的失步阈值
        //!MOTIONPLAN OOSTOTALOUTNUM

        //! init the out
        val2=0;

        string args;
        args = string.Format("MOTIONPLAN:OOSTOTALOUTNUM? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,NONE|ALARM|STOP|ALARM&STOP
    public int setMOTIONPLAN_OOSTOTALRESPONSE(  byte val0, string val1, string val2 )
    {
        //!设置当步数偏差超过LOSTNUM后的响应
        //!MOTIONPLAN OOSTOTALRESPONSE
        string args;
        args = string.Format("MOTIONPLAN:OOSTOTALRESPONSE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    //!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
    //!val2
    public int getMOTIONPLAN_OOSTOTALRESPONSE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询失步响应
        //!MOTIONPLAN OOSTOTALRESPONSE

        //! init the out
        val2=null;

        string args;
        args = string.Format("MOTIONPLAN:OOSTOTALRESPONSE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,U8,F32
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
    public int setPOSITION(  byte val0, string val1, byte val2, float val3 )
    {
        //!设置PVT点的位置值
        //!POSITION 
        string args;
        args = string.Format("POSITION {0},{1},{2},{3}" , val0,val1,val2,val3);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM,U8
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
    //!f32
    //!val3
    public int getPOSITION(  byte val0, string val1, byte val2 ,  out float val3 )
    {
        //!查询PVT点的位置值
        //!POSITION 

        //! init the out
        val3=0;

        string args;
        args = string.Format("POSITION? {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val3 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,ENUM,U8,F32
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
    public int setVELOCITY(  byte val0, string val1, byte val2, float val3 )
    {
        //!设置PVT点的速度值
        //!VELOCITY 
        string args;
        args = string.Format("VELOCITY {0},{1},{2},{3}" , val0,val1,val2,val3);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM,U8
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
    //!f32
    //!val3
    public int getVELOCITY(  byte val0, string val1, byte val2 ,  out float val3 )
    {
        //!查询PVT点的速度值
        //!VELOCITY 

        //! init the out
        val3=0;

        string args;
        args = string.Format("VELOCITY? {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val3 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,ENUM,U8,F32
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
    public int setTIME(  byte val0, string val1, byte val2, float val3 )
    {
        //!设置PVT点的时间值
        //!TIME 
        string args;
        args = string.Format("TIME {0},{1},{2},{3}" , val0,val1,val2,val3);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM,U8
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
    //!f32
    //!val3
    public int getTIME(  byte val0, string val1, byte val2 ,  out float val3 )
    {
        //!查询PVT点的时间值
        //!TIME 

        //! init the out
        val3=0;

        string args;
        args = string.Format("TIME? {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val3 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,ENUM,U8,U16,U16
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
    public int setTIMESCALE(  byte val0, string val1, byte val2, UInt16 val3, UInt16 val4 )
    {
        //!设置S曲线的加减速占比，两段一起，千分之
        //!TIMESCALE 
        string args;
        args = string.Format("TIMESCALE {0},{1},{2},{3},{4}" , val0,val1,val2,val3,val4);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM,U8
    //![CHANNUM],MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8,[INDEX]
    //!uint16,uint16
    //!val3,val4
    public int getTIMESCALE(  byte val0, string val1, byte val2 ,  out UInt16 val3, out UInt16 val4 )
    {
        //!查询S曲线的加减速占比，两段一起，千分之
        //!TIMESCALE 

        //! init the out
        val3=0;
        val4=0;

        string args;
        args = string.Format("TIMESCALE? {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 2 )
        { return -1; } 
        val3 = Convert.ToUInt16( paras[0] );
        val4 = Convert.ToUInt16( paras[1] );
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC,OFF|ON
    public int setREPORT_STATE(  byte val0, string val1, string val2 )
    {
        //!打开或关闭上报功能，编号从0到4对应失步数、步数、速度、力矩和温度
        //!REPORT STATE
        string args;
        args = string.Format("REPORT:STATE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC
    //!MRQ_SYSTEM_REVMOTION
    //!val2
    public int getREPORT_STATE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询上报状态
        //!REPORT STATE

        //! init the out
        val2=null;

        string args;
        args = string.Format("REPORT:STATE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,U32
    //![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC
    public int setREPORT_PERIOD(  byte val0, string val1, UInt32 val2 )
    {
        //!设置上报周期，单位ms
        //!REPORT PERIOD
        string args;
        args = string.Format("REPORT:PERIOD {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC
    //!uint32
    //!val2
    public int getREPORT_PERIOD(  byte val0, string val1 ,  out UInt32 val2 )
    {
        //!查询上报周期
        //!REPORT PERIOD

        //! init the out
        val2=0;

        string args;
        args = string.Format("REPORT:PERIOD? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],TORQUE|CYCLE|SGALL|SGSE|DIST|ABSENC
    //!uint32
    //!val2
    public int getREPORT_DATA(  byte val0, string val1 ,  out UInt32 val2 )
    {
        //!上报数据，上报打开时下位机按照上报周期主动上报；上位机也可以主动查询
        //!REPORT DATA

        //! init the out
        val2=0;

        string args;
        args = string.Format("REPORT:DATA? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],PATTERN|LEVEL
    public int setTRIGGER_MODE(  byte val0, string val1 )
    {
        //!设置触发类型
        //!TRIGGER MODE
        string args;
        args = string.Format("TRIGGER:MODE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_TRIGGER_MODE
    //!val1
    public int getTRIGGER_MODE(  byte val0 ,  out string val1 )
    {
        //!查询触发类型
        //!TRIGGER MODE

        //! init the out
        val1=null;

        string args;
        args = string.Format("TRIGGER:MODE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],OFF|ON
    public int setTRIGGER_PATTSTATE(  byte val0, string val1 )
    {
        //!打开或关闭码型触发
        //!TRIGGER PATTSTATE
        string args;
        args = string.Format("TRIGGER:PATTSTATE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_SYSTEM_REVMOTION
    //!val1
    public int getTRIGGER_PATTSTATE(  byte val0 ,  out string val1 )
    {
        //!查询码型触发的状态
        //!TRIGGER PATTSTATE

        //! init the out
        val1=null;

        string args;
        args = string.Format("TRIGGER:PATTSTATE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],X|L|R|F|H,X|L|R|F|H
    public int setTRIGGER_PATTERN(  byte val0, string val1, string val2 )
    {
        //!设置触发输入的码型
        //!TRIGGER PATTERN
        string args;
        args = string.Format("TRIGGER:PATTERN {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_TRIGGER_PATTERN,MRQ_TRIGGER_PATTERN
    //!val1,val2
    public int getTRIGGER_PATTERN(  byte val0 ,  out string val1, out string val2 )
    {
        //!查询触发输入的码型
        //!TRIGGER PATTERN

        //! init the out
        val1=null;
        val2=null;

        string args;
        args = string.Format("TRIGGER:PATTERN? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 2 )
        { return -1; } 
        val1 = paras[0];
        val2 = paras[1];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],NONE|ALARM|STOP|ALARM&STOP
    public int setTRIGGER_PATTRESP(  byte val0, string val1 )
    {
        //!设置码型触发的响应
        //!TRIGGER PATTRESP
        string args;
        args = string.Format("TRIGGER:PATTRESP {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
    //!val1
    public int getTRIGGER_PATTRESP(  byte val0 ,  out string val1 )
    {
        //!查询码型触发的响应
        //!TRIGGER PATTRESP

        //! init the out
        val1=null;

        string args;
        args = string.Format("TRIGGER:PATTRESP? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],CONTINUED|INTERVAL|SINGLE
    public int setTRIGGER_PATTSMODE(  byte val0, string val1 )
    {
        //!设置码型触发的采样模式
        //!TRIGGER PATTSMODE
        string args;
        args = string.Format("TRIGGER:PATTSMODE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_TRIGGER_PATTSMODE
    //!val1
    public int getTRIGGER_PATTSMODE(  byte val0 ,  out string val1 )
    {
        //!查询码型触发的采样模式
        //!TRIGGER PATTSMODE

        //! init the out
        val1=null;

        string args;
        args = string.Format("TRIGGER:PATTSMODE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,U32
    //![CHANNUM]
    public int setTRIGGER_PATTSPERIOD(  byte val0, UInt32 val1 )
    {
        //!设置码型触发的采样周期
        //!TRIGGER PATTSPERIOD
        string args;
        args = string.Format("TRIGGER:PATTSPERIOD {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!uint32
    //!val1
    public int getTRIGGER_PATTSPERIOD(  byte val0 ,  out UInt32 val1 )
    {
        //!查询码型触发的采样周期
        //!TRIGGER PATTSPERIOD

        //! init the out
        val1=0;

        string args;
        args = string.Format("TRIGGER:PATTSPERIOD? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,OFF|ON
    public int setTRIGGER_LEVELSTATE(  byte val0, string val1, string val2 )
    {
        //!打开或关闭电平触发(3,4轴型号中TRIGR和编码器互斥，使用编码器时无法使用TRIGR,反之亦然)
        //!TRIGGER LEVELSTATE
        string args;
        args = string.Format("TRIGGER:LEVELSTATE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,OFF|ON
    //!MRQ_SYSTEM_REVMOTION
    //!val2
    public int getTRIGGER_LEVELSTATE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询电平触发的状态
        //!TRIGGER LEVELSTATE

        //! init the out
        val2=null;

        string args;
        args = string.Format("TRIGGER:LEVELSTATE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,RESERVE|LOW|RISE|FALL|HIGH
    public int setTRIGGER_LEVELTYPE(  byte val0, string val1, string val2 )
    {
        //!设置触发电平类型
        //!TRIGGER LEVELTYPE
        string args;
        args = string.Format("TRIGGER:LEVELTYPE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
    //!MRQ_TRIGGER_LEVELTYPE_1
    //!val2
    public int getTRIGGER_LEVELTYPE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询触发电平类型
        //!TRIGGER LEVELTYPE

        //! init the out
        val2=null;

        string args;
        args = string.Format("TRIGGER:LEVELTYPE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,NONE|ALARM|STOP|ALARM&STOP|RUN
    public int setTRIGGER_LEVELRESP(  byte val0, string val1, string val2 )
    {
        //!设置电平触发的响应
        //!TRIGGER LEVELRESP
        string args;
        args = string.Format("TRIGGER:LEVELRESP {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
    //!MRQ_TRIGGER_LEVELRESP_1
    //!val2
    public int getTRIGGER_LEVELRESP(  byte val0, string val1 ,  out string val2 )
    {
        //!查询电平触发的响应
        //!TRIGGER LEVELRESP

        //! init the out
        val2=null;

        string args;
        args = string.Format("TRIGGER:LEVELRESP? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,CONTINUED|INTERVAL|SINGLE
    public int setTRIGGER_LEVELSMODE(  byte val0, string val1, string val2 )
    {
        //!设置电平触发的采样模式
        //!TRIGGER LEVELSMODE
        string args;
        args = string.Format("TRIGGER:LEVELSMODE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
    //!MRQ_TRIGGER_PATTSMODE
    //!val2
    public int getTRIGGER_LEVELSMODE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询电平触发的采样模式
        //!TRIGGER LEVELSMODE

        //! init the out
        val2=null;

        string args;
        args = string.Format("TRIGGER:LEVELSMODE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8,ENUM,F32
    //![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
    public int setTRIGGER_LEVELSPERIOD(  byte val0, string val1, float val2 )
    {
        //!设置电平触发的采样周期
        //!TRIGGER LEVELSPERIOD
        string args;
        args = string.Format("TRIGGER:LEVELSPERIOD {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
    //!f32
    //!val2
    public int getTRIGGER_LEVELSPERIOD(  byte val0, string val1 ,  out float val2 )
    {
        //!查询电平触发的采样周期
        //!TRIGGER LEVELSPERIOD

        //! init the out
        val2=0;

        string args;
        args = string.Format("TRIGGER:LEVELSPERIOD? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5,MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
    public int setTRIGGER_LEVELRUNWAVE(  byte val0, string val1, string val2 )
    {
        //!设置电平触发响应为RUN时执行的波表
        //!TRIGGER LEVELRUNWAVE
        string args;
        args = string.Format("TRIGGER:LEVELRUNWAVE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],TRIG1|TRIG2|TRIG3|TRIG4|TRIG5
    //!MRQ_MOTION_SWITCH_1
    //!val2
    public int getTRIGGER_LEVELRUNWAVE(  byte val0, string val1 ,  out string val2 )
    {
        //!查询电平触发响应为RUN时执行的波表
        //!TRIGGER LEVELRUNWAVE

        //! init the out
        val2=null;

        string args;
        args = string.Format("TRIGGER:LEVELRUNWAVE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_DRIVER_TYPE
    //!val1
    public int getDRIVER_TYPE(  byte val0 ,  out string val1 )
    {
        //!查询驱动板类型，其实可以通过型号进行区分
        //!DRIVER TYPE

        //! init the out
        val1=null;

        string args;
        args = string.Format("DRIVER:TYPE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],R0|R1|R2
    //!uint32
    //!val2
    public int getDRIVER_STATEREG(  byte val0, string val1 ,  out UInt32 val2 )
    {
        //!获取TMC芯片状态寄存器信息(数据源为TMC三个状态寄存器)
        //!DRIVER STATEREG

        //! init the out
        val2=0;

        string args;
        args = string.Format("DRIVER:STATEREG? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8,U8
    //![CHANNUM]
    public int setDRIVER_CURRENT(  byte val0, byte val1 )
    {
        //!设置驱动工作最大电流，单位0.1A
        //!DRIVER CURRENT
        string args;
        args = string.Format("DRIVER:CURRENT {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!byte
    //!val1
    public int getDRIVER_CURRENT(  byte val0 ,  out byte val1 )
    {
        //!查询驱动工作最大电流
        //!DRIVER CURRENT

        //! init the out
        val1=0;

        string args;
        args = string.Format("DRIVER:CURRENT? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],256|128|64|32|16|8|4|2|1
    public int setDRIVER_MICROSTEPS(  byte val0, string val1 )
    {
        //!设置电机微步数
        //!DRIVER MICROSTEPS
        string args;
        args = string.Format("DRIVER:MICROSTEPS {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_DRIVER_MICROSTEPS
    //!val1
    public int getDRIVER_MICROSTEPS(  byte val0 ,  out string val1 )
    {
        //!查询电机微步数
        //!DRIVER MICROSTEPS

        //! init the out
        val1=null;

        string args;
        args = string.Format("DRIVER:MICROSTEPS? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],OFF|ON
    public int setDRIVER_STATE(  byte val0, string val1 )
    {
        //!设置驱动开关状态
        //!DRIVER STATE
        string args;
        args = string.Format("DRIVER:STATE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_SYSTEM_REVMOTION
    //!val1
    public int getDRIVER_STATE(  byte val0 ,  out string val1 )
    {
        //!查询驱动开关状态
        //!DRIVER STATE

        //! init the out
        val1=null;

        string args;
        args = string.Format("DRIVER:STATE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM,S8
    //![CHANNUM],UPLIMIT|DOWNLIMIT
    public int setDRIVER_SGLIMIT(  byte val0, string val1, byte val2 )
    {
        //!设置能效曲线SG上下限，超限后报警
        //!DRIVER SGLIMIT
        string args;
        args = string.Format("DRIVER:SGLIMIT {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM],UPLIMIT|DOWNLIMIT
    //!MRQ_DRIVER_SGLIMIT,int8
    //!val1,val2
    public int getDRIVER_SGLIMIT(  byte val0 ,  out string val1, out byte val2 )
    {
        //!查询能效曲线SG上下限
        //!DRIVER SGLIMIT

        //! init the out
        val1=null;
        val2=0;

        string args;
        args = string.Format("DRIVER:SGLIMIT? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 2 )
        { return -1; } 
        val1 = paras[0];
        val2 = Convert.ToByte( paras[1] );
        return 0;
    }

    //!U8,ENUM,S16
    //![CHANNUM],SG0|SGT|SEMAX|SEMIN
    public int setDRIVER_SGPARASET(  byte val0, string val1, Int16 val2 )
    {
        //!设置SG相关值
        //!DRIVER SGPARASET
        string args;
        args = string.Format("DRIVER:SGPARASET {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],SG0|SGT|SEMAX|SEMIN
    //!int16
    //!val2
    public int getDRIVER_SGPARASET(  byte val0, string val1 ,  out Int16 val2 )
    {
        //!查询SG相关值
        //!DRIVER SGPARASET

        //! init the out
        val2=0;

        string args;
        args = string.Format("DRIVER:SGPARASET? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToInt16( paras[0] );
        return 0;
    }

    //!U8,U8
    //![CHANNUM]
    public int setDRIVER_IDLECURRENT(  byte val0, byte val1 )
    {
        //!设置空闲电流，单位0.1A
        //!DRIVER IDLECURRENT
        string args;
        args = string.Format("DRIVER:IDLECURRENT {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!byte
    //!val1
    public int getDRIVER_IDLECURRENT(  byte val0 ,  out byte val1 )
    {
        //!查询空闲电流
        //!DRIVER IDLECURRENT

        //! init the out
        val1=0;

        string args;
        args = string.Format("DRIVER:IDLECURRENT? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!U8,U32
    //![CHANNUM]
    public int setDRIVER_SWITCHTIME(  byte val0, UInt32 val1 )
    {
        //!从工作电流切换到空闲电流的时间，单位ms
        //!DRIVER SWITCHTIME
        string args;
        args = string.Format("DRIVER:SWITCHTIME {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!uint32
    //!val1
    public int getDRIVER_SWITCHTIME(  byte val0 ,  out UInt32 val1 )
    {
        //!查询切换时间
        //!DRIVER SWITCHTIME

        //! init the out
        val1=0;

        string args;
        args = string.Format("DRIVER:SWITCHTIME? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],1/2|1/4
    public int setDRIVER_MINICURRRATIO(  byte val0, string val1 )
    {
        //!设置运行过程中的最低电流比
        //!DRIVER MINICURRRATIO
        string args;
        args = string.Format("DRIVER:MINICURRRATIO {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_DRIVER_MINICURRRATIO
    //!val1
    public int getDRIVER_MINICURRRATIO(  byte val0 ,  out string val1 )
    {
        //!查询运行过程中的最低电流比
        //!DRIVER MINICURRRATIO

        //! init the out
        val1=null;

        string args;
        args = string.Format("DRIVER:MINICURRRATIO? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM,U32
    //![CHANNUM],DRVCTRL|CHOPCONF|SMARTEN|SGCSCONF|DRVCONF
    public int setDRIVER_REGCONFIG(  byte val0, string val1, UInt32 val2 )
    {
        //!配置驱动寄存器
        //!DRIVER REGCONFIG
        string args;
        args = string.Format("DRIVER:REGCONFIG {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8,ENUM
    //![CHANNUM],DRVCTRL|CHOPCONF|SMARTEN|SGCSCONF|DRVCONF
    //!uint32
    //!val2
    public int getDRIVER_REGCONFIG(  byte val0, string val1 ,  out UInt32 val2 )
    {
        //!查询驱动寄存器的配置
        //!DRIVER REGCONFIG

        //! init the out
        val2=0;

        string args;
        args = string.Format("DRIVER:REGCONFIG? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!ENUM,ENUM
    //!DO1|DO2|DO3|DO4,DISABLE|ENABLE
    public int setDIGITALOUTPUT_STATE(  string val0, string val1 )
    {
        //!打开或关闭触发输出
        //!DIGITALOUTPUT STATE
        string args;
        args = string.Format("DIGITALOUTPUT:STATE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!DO1|DO2|DO3|DO4
    //!MRQ_DIGITALOUTPUT_STATE_1
    //!val1
    public int getDIGITALOUTPUT_STATE(  string val0 ,  out string val1 )
    {
        //!查询触发输出的状态
        //!DIGITALOUTPUT STATE

        //! init the out
        val1=null;

        string args;
        args = string.Format("DIGITALOUTPUT:STATE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,ENUM
    //!DO1|DO2|DO3|DO4,LEVEL|NONE
    public int setDIGITALOUTPUT_SIGNAL(  string val0, string val1 )
    {
        //!设置触发输出的输出信号类型
        //!DIGITALOUTPUT SIGNAL
        string args;
        args = string.Format("DIGITALOUTPUT:SIGNAL {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!DO1|DO2|DO3|DO4
    //!MRQ_DIGITALOUTPUT_SIGNAL_1
    //!val1
    public int getDIGITALOUTPUT_SIGNAL(  string val0 ,  out string val1 )
    {
        //!查询触发输出的输出信号类型
        //!DIGITALOUTPUT SIGNAL

        //! init the out
        val1=null;

        string args;
        args = string.Format("DIGITALOUTPUT:SIGNAL? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,ENUM
    //!DO1|DO2|DO3|DO4,POSITIVE|NEGATIVE
    public int setDIGITALOUTPUT_POLARITY(  string val0, string val1 )
    {
        //!设置触发输出的输出信号极性
        //!DIGITALOUTPUT POLARITY
        string args;
        args = string.Format("DIGITALOUTPUT:POLARITY {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!DO1|DO2|DO3|DO4
    //!MRQ_DIGITALOUTPUT_POLARITY_1
    //!val1
    public int getDIGITALOUTPUT_POLARITY(  string val0 ,  out string val1 )
    {
        //!查询触发输出的输出信号极性
        //!DIGITALOUTPUT POLARITY

        //! init the out
        val1=null;

        string args;
        args = string.Format("DIGITALOUTPUT:POLARITY? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,U8
    //!DO1|DO2|DO3|DO4,[CHANNUM]
    public int setDIGITALOUTPUT_SOURCE(  string val0, byte val1 )
    {
        //!设置隔离输出的控制源(不包括所有通道：128和分组号)
        //!DIGITALOUTPUT SOURCE
        string args;
        args = string.Format("DIGITALOUTPUT:SOURCE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!DO1|DO2|DO3|DO4
    //!byte
    //!val1
    public int getDIGITALOUTPUT_SOURCE(  string val0 ,  out byte val1 )
    {
        //!查询隔离输出的控制源
        //!DIGITALOUTPUT SOURCE

        //! init the out
        val1=0;

        string args;
        args = string.Format("DIGITALOUTPUT:SOURCE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!ENUM,ENUM
    //!DO1|DO2|DO3|DO4,AUTO|NONE
    public int setDIGITALOUTPUT_CONDITION(  string val0, string val1 )
    {
        //!设置触发输出的触发条件
        //!DIGITALOUTPUT CONDITION
        string args;
        args = string.Format("DIGITALOUTPUT:CONDITION {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!DO1|DO2|DO3|DO4
    //!MRQ_DIGITALOUTPUT_CONDITION_1
    //!val1
    public int getDIGITALOUTPUT_CONDITION(  string val0 ,  out string val1 )
    {
        //!查询触发输出的触发条件
        //!DIGITALOUTPUT CONDITION

        //! init the out
        val1=null;

        string args;
        args = string.Format("DIGITALOUTPUT:CONDITION? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,U32
    //!DO1|DO2|DO3|DO4
    public int setDIGITALOUTPUT_PERIOD(  string val0, UInt32 val1 )
    {
        //!设置触发输出的输出方波的周期
        //!DIGITALOUTPUT PERIOD
        string args;
        args = string.Format("DIGITALOUTPUT:PERIOD {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!DO1|DO2|DO3|DO4
    //!uint32
    //!val1
    public int getDIGITALOUTPUT_PERIOD(  string val0 ,  out UInt32 val1 )
    {
        //!查询触发输出的输出方波的周期
        //!DIGITALOUTPUT PERIOD

        //! init the out
        val1=0;

        string args;
        args = string.Format("DIGITALOUTPUT:PERIOD? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!ENUM,U16
    //!DO1|DO2|DO3|DO4
    public int setDIGITALOUTPUT_DUTY(  string val0, UInt16 val1 )
    {
        //!设置触发输出的输出方波的占空比
        //!DIGITALOUTPUT DUTY
        string args;
        args = string.Format("DIGITALOUTPUT:DUTY {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!DO1|DO2|DO3|DO4
    //!uint16
    //!val1
    public int getDIGITALOUTPUT_DUTY(  string val0 ,  out UInt16 val1 )
    {
        //!查询触发输出的输出方波的占空比
        //!DIGITALOUTPUT DUTY

        //! init the out
        val1=0;

        string args;
        args = string.Format("DIGITALOUTPUT:DUTY? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt16( paras[0] );
        return 0;
    }

    //!ENUM,ENUM
    //!YO1|YO2,DISABLE|ENABLE
    public int setISOLATOROUTPUT_STATE(  string val0, string val1 )
    {
        //!打开或关闭隔离输出
        //!ISOLATOROUTPUT STATE
        string args;
        args = string.Format("ISOLATOROUTPUT:STATE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!YO1|YO2
    //!MRQ_DIGITALOUTPUT_STATE_1
    //!val1
    public int getISOLATOROUTPUT_STATE(  string val0 ,  out string val1 )
    {
        //!查询隔离输出的状态
        //!ISOLATOROUTPUT STATE

        //! init the out
        val1=null;

        string args;
        args = string.Format("ISOLATOROUTPUT:STATE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,U8
    //!YO1|YO2,[CHANNUM]
    public int setISOLATOROUTPUT_SOURCE(  string val0, byte val1 )
    {
        //!设置隔离输出的控制源(不包括所有通道：128和分组号)
        //!ISOLATOROUTPUT SOURCE
        string args;
        args = string.Format("ISOLATOROUTPUT:SOURCE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!YO1|YO2
    //!byte
    //!val1
    public int getISOLATOROUTPUT_SOURCE(  string val0 ,  out byte val1 )
    {
        //!查询隔离输出的控制源
        //!ISOLATOROUTPUT SOURCE

        //! init the out
        val1=0;

        string args;
        args = string.Format("ISOLATOROUTPUT:SOURCE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!ENUM,ENUM
    //!YO1|YO2,AUTO|NONE
    public int setISOLATOROUTPUT_CONDITION(  string val0, string val1 )
    {
        //!设置隔离输出的触发条件
        //!ISOLATOROUTPUT CONDITION
        string args;
        args = string.Format("ISOLATOROUTPUT:CONDITION {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!YO1|YO2
    //!MRQ_DIGITALOUTPUT_CONDITION_1
    //!val1
    public int getISOLATOROUTPUT_CONDITION(  string val0 ,  out string val1 )
    {
        //!查询隔离输出的触发条件
        //!ISOLATOROUTPUT CONDITION

        //! init the out
        val1=null;

        string args;
        args = string.Format("ISOLATOROUTPUT:CONDITION? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,ENUM
    //!YO1|YO2,TURNON|CUTOFF
    public int setISOLATOROUTPUT_RESPONSE(  string val0, string val1 )
    {
        //!设置隔离输出的响应方式
        //!ISOLATOROUTPUT RESPONSE
        string args;
        args = string.Format("ISOLATOROUTPUT:RESPONSE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!YO1|YO2
    //!MRQ_ISOLATOROUTPUT_RESPONSE_1
    //!val1
    public int getISOLATOROUTPUT_RESPONSE(  string val0 ,  out string val1 )
    {
        //!查询隔离输出的响应方式
        //!ISOLATOROUTPUT RESPONSE

        //! init the out
        val1=null;

        string args;
        args = string.Format("ISOLATOROUTPUT:RESPONSE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,ENUM
    //!UART1|UART2,4800|7200|9600|14400|19200|38400|57600|115200|128000|2000000|2500000
    public int setSENSORUART_BAUD(  string val0, string val1 )
    {
        //!设置波特率，单位bps
        //!SENSORUART BAUD
        string args;
        args = string.Format("SENSORUART:BAUD {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!UART1|UART2
    //!MRQ_SENSORUART_BAUD_1
    //!val1
    public int getSENSORUART_BAUD(  string val0 ,  out string val1 )
    {
        //!查询RS232波特率
        //!SENSORUART BAUD

        //! init the out
        val1=null;

        string args;
        args = string.Format("SENSORUART:BAUD? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,ENUM
    //!UART1|UART2,8|9
    public int setSENSORUART_WORDLEN(  string val0, string val1 )
    {
        //!设置数据长度
        //!SENSORUART WORDLEN
        string args;
        args = string.Format("SENSORUART:WORDLEN {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!UART1|UART2
    //!MRQ_RS232_WORDLEN
    //!val1
    public int getSENSORUART_WORDLEN(  string val0 ,  out string val1 )
    {
        //!查询RS232数据长度
        //!SENSORUART WORDLEN

        //! init the out
        val1=null;

        string args;
        args = string.Format("SENSORUART:WORDLEN? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,ENUM
    //!UART1|UART2,NONE|RTS|CTS|RTS&CTS
    public int setSENSORUART_FLOWCTL(  string val0, string val1 )
    {
        //!打开或关闭流控制
        //!SENSORUART FLOWCTL
        string args;
        args = string.Format("SENSORUART:FLOWCTL {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!UART1|UART2
    //!MRQ_RS232_FLOWCTL
    //!val1
    public int getSENSORUART_FLOWCTL(  string val0 ,  out string val1 )
    {
        //!查询RS232流控制状态
        //!SENSORUART FLOWCTL

        //! init the out
        val1=null;

        string args;
        args = string.Format("SENSORUART:FLOWCTL? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,ENUM
    //!UART1|UART2,NONE|EVEN|ODD
    public int setSENSORUART_PARITY(  string val0, string val1 )
    {
        //!校验方式
        //!SENSORUART PARITY
        string args;
        args = string.Format("SENSORUART:PARITY {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!UART1|UART2
    //!MRQ_RS232_PARITY
    //!val1
    public int getSENSORUART_PARITY(  string val0 ,  out string val1 )
    {
        //!查询RS232校验方式
        //!SENSORUART PARITY

        //! init the out
        val1=null;

        string args;
        args = string.Format("SENSORUART:PARITY? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,ENUM
    //!UART1|UART2,1|0.5|2|1.5
    public int setSENSORUART_STOPBIT(  string val0, string val1 )
    {
        //!设置停止位
        //!SENSORUART STOPBIT
        string args;
        args = string.Format("SENSORUART:STOPBIT {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!UART1|UART2
    //!MRQ_RS232_STOPBIT
    //!val1
    public int getSENSORUART_STOPBIT(  string val0 ,  out string val1 )
    {
        //!查询RS232停止位
        //!SENSORUART STOPBIT

        //! init the out
        val1=null;

        string args;
        args = string.Format("SENSORUART:STOPBIT? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM
    //!UART1|UART2
    public int setSENSORUART_APPLYPARA(  string val0 )
    {
        //!按照之前设置的参数配置接口
        //!SENSORUART APPLYPARA
        string args;
        args = string.Format("SENSORUART:APPLYPARA {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM,ENUM,ENUM
    //!UART1|UART2,S1|S2|S3|S4,OFF|ON
    public int setSENSORUART_STATE(  string val0, string val1, string val2 )
    {
        //!开启或关闭传感器UART
        //!SENSORUART STATE
        string args;
        args = string.Format("SENSORUART:STATE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM,ENUM
    //!UART1|UART2,S1|S2|S3|S4
    //!MRQ_SYSTEM_REVMOTION
    //!val2
    public int getSENSORUART_STATE(  string val0, string val1 ,  out string val2 )
    {
        //!查询传感器UART状态
        //!SENSORUART STATE

        //! init the out
        val2=null;

        string args;
        args = string.Format("SENSORUART:STATE? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = paras[0];
        return 0;
    }

    //!ENUM,ENUM,U8
    //!UART1|UART2,S1|S2|S3|S4
    public int setSENSORUART_SOF(  string val0, string val1, byte val2 )
    {
        //!设置数据帧头
        //!SENSORUART SOF
        string args;
        args = string.Format("SENSORUART:SOF {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM,ENUM
    //!UART1|UART2,S1|S2|S3|S4
    //!byte
    //!val2
    public int getSENSORUART_SOF(  string val0, string val1 ,  out byte val2 )
    {
        //!查询数据帧头
        //!SENSORUART SOF

        //! init the out
        val2=0;

        string args;
        args = string.Format("SENSORUART:SOF? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!ENUM,ENUM,U8
    //!UART1|UART2,S1|S2|S3|S4
    public int setSENSORUART_FRAMELEN(  string val0, string val1, byte val2 )
    {
        //!设置帧长度
        //!SENSORUART FRAMELEN
        string args;
        args = string.Format("SENSORUART:FRAMELEN {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM,ENUM
    //!UART1|UART2,S1|S2|S3|S4
    //!byte
    //!val2
    public int getSENSORUART_FRAMELEN(  string val0, string val1 ,  out byte val2 )
    {
        //!查询帧长度
        //!SENSORUART FRAMELEN

        //! init the out
        val2=0;

        string args;
        args = string.Format("SENSORUART:FRAMELEN? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!ENUM,ENUM,U8
    //!UART1|UART2,S1|S2|S3|S4
    public int setSENSORUART_RECEIVENUM(  string val0, string val1, byte val2 )
    {
        //!设置周期内接收的帧数
        //!SENSORUART RECEIVENUM
        string args;
        args = string.Format("SENSORUART:RECEIVENUM {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM,ENUM
    //!UART1|UART2,S1|S2|S3|S4
    //!byte
    //!val2
    public int getSENSORUART_RECEIVENUM(  string val0, string val1 ,  out byte val2 )
    {
        //!查询一个周期内接收的帧数
        //!SENSORUART RECEIVENUM

        //! init the out
        val2=0;

        string args;
        args = string.Format("SENSORUART:RECEIVENUM? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!ENUM,ENUM,U32
    //!UART1|UART2,S1|S2|S3|S4
    public int setSENSORUART_SWITCHTIME(  string val0, string val1, UInt32 val2 )
    {
        //!设置切换周期
        //!SENSORUART SWITCHTIME
        string args;
        args = string.Format("SENSORUART:SWITCHTIME {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM,ENUM
    //!UART1|UART2,S1|S2|S3|S4
    //!uint32
    //!val2
    public int getSENSORUART_SWITCHTIME(  string val0, string val1 ,  out UInt32 val2 )
    {
        //!查询切换周期
        //!SENSORUART SWITCHTIME

        //! init the out
        val2=0;

        string args;
        args = string.Format("SENSORUART:SWITCHTIME? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val2 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!ENUM,ENUM
    //!UART1|UART2,S1|S2|S3|S4
    //!byte,char,char,char
    //!val2,val3,val4,val5
    public int getSENSORUART_DATA(  string val0, string val1 ,  out byte val2, out byte val3, out byte val4, out byte val5 )
    {
        //!查询数据
        //!SENSORUART DATA

        //! init the out
        val2=0;
        val3=0;
        val4=0;
        val5=0;

        string args;
        args = string.Format("SENSORUART:DATA? {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 4 )
        { return -1; } 
        val2 = Convert.ToByte( paras[0] );
        val3 = Convert.ToByte( paras[1] );
        val4 = Convert.ToByte( paras[2] );
        val5 = Convert.ToByte( paras[3] );
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],OFF|ON
    public int setTUNING_STATE(  byte val0, string val1 )
    {
        //!设置驱动开关状态
        //!TUNING STATE
        string args;
        args = string.Format("TUNING:STATE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_SYSTEM_REVMOTION
    //!val1
    public int getTUNING_STATE(  byte val0 ,  out string val1 )
    {
        //!查询驱动开关状态
        //!TUNING STATE

        //! init the out
        val1=null;

        string args;
        args = string.Format("TUNING:STATE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],1/2|1/4
    public int setTUNING_MINICURRRATIO(  byte val0, string val1 )
    {
        //!设置运行过程中的最低电流比
        //!TUNING MINICURRRATIO
        string args;
        args = string.Format("TUNING:MINICURRRATIO {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_DRIVER_MINICURRRATIO
    //!val1
    public int getTUNING_MINICURRRATIO(  byte val0 ,  out string val1 )
    {
        //!查询运行过程中的最低电流比
        //!TUNING MINICURRRATIO

        //! init the out
        val1=null;

        string args;
        args = string.Format("TUNING:MINICURRRATIO? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,U16,U16
    //![CHANNUM]
    public int setTUNING_ENERGYEFFIC(  byte val0, UInt16 val1, UInt16 val2 )
    {
        //!设置运行过程中能效的上下限，上下限一起设置，上限在前，下限在后，千分之
        //!TUNING ENERGYEFFIC
        string args;
        args = string.Format("TUNING:ENERGYEFFIC {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!uint16,uint16
    //!val1,val2
    public int getTUNING_ENERGYEFFIC(  byte val0 ,  out UInt16 val1, out UInt16 val2 )
    {
        //!查询运行过程中能效的上下限
        //!TUNING ENERGYEFFIC

        //! init the out
        val1=0;
        val2=0;

        string args;
        args = string.Format("TUNING:ENERGYEFFIC? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 2 )
        { return -1; } 
        val1 = Convert.ToUInt16( paras[0] );
        val2 = Convert.ToUInt16( paras[1] );
        return 0;
    }

    //!U8,ENUM,ENUM
    //![CHANNUM],1|2|4|8,32|8|2|1
    public int setTUNING_CURRREGULATE(  byte val0, string val1, string val2 )
    {
        //!设置电流调节的速度，上升速度和下降速度
        //!TUNING CURRREGULATE
        string args;
        args = string.Format("TUNING:CURRREGULATE {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //!
    //!MRQ_TUNING_CURRREGULATE,MRQ_TUNING_CURRREGULATE_1
    //!val1,val2
    public int getTUNING_CURRREGULATE(  byte val0 ,  out string val1, out string val2 )
    {
        //!查询电流调节的速度，上升速度和下降速度
        //!TUNING CURRREGULATE

        //! init the out
        val1=null;
        val2=null;

        string args;
        args = string.Format("TUNING:CURRREGULATE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 2 )
        { return -1; } 
        val1 = paras[0];
        val2 = paras[1];
        return 0;
    }

    //!ENUM
    //!OFF|ON
    public int setISOLATORIN_STATE(  string val0 )
    {
        //!打开或关闭隔离输入
        //!ISOLATORIN STATE
        string args;
        args = string.Format("ISOLATORIN:STATE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_SYSTEM_REVMOTION
    //!val0
    public int getISOLATORIN_STATE(  out string val0 )
    {
        //!查询隔离输入的状态
        //!ISOLATORIN STATE

        //! init the out
        val0=null;

        string args;
        args = string.Format("ISOLATORIN:STATE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM
    //!RESERVE|LOW|RISE|FALL|HIGH
    public int setISOLATORIN_TYPE(  string val0 )
    {
        //!设置隔离输入电平类型
        //!ISOLATORIN TYPE
        string args;
        args = string.Format("ISOLATORIN:TYPE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_TRIGGER_LEVELTYPE_1
    //!val0
    public int getISOLATORIN_TYPE(  out string val0 )
    {
        //!查询隔离输入电平类型
        //!ISOLATORIN TYPE

        //! init the out
        val0=null;

        string args;
        args = string.Format("ISOLATORIN:TYPE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM
    //!NONE|ALARM|STOP|ALARM&STOP
    public int setISOLATORIN_RESPONSE(  string val0 )
    {
        //!设置隔离输入的响应
        //!ISOLATORIN RESPONSE
        string args;
        args = string.Format("ISOLATORIN:RESPONSE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
    //!val0
    public int getISOLATORIN_RESPONSE(  out string val0 )
    {
        //!查询隔离输入的响应
        //!ISOLATORIN RESPONSE

        //! init the out
        val0=null;

        string args;
        args = string.Format("ISOLATORIN:RESPONSE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!U8
    //![CHANNUM]
    public int setISOLATORIN_RESPCHAN(  byte val0 )
    {
        //!设置隔离输入的响应通道(不包括所有通道号：128和分组号)
        //!ISOLATORIN RESPCHAN
        string args;
        args = string.Format("ISOLATORIN:RESPCHAN {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!byte
    //!val0
    public int getISOLATORIN_RESPCHAN(  out byte val0 )
    {
        //!查询隔离输入的响应通道
        //!ISOLATORIN RESPCHAN

        //! init the out
        val0=0;

        string args;
        args = string.Format("ISOLATORIN:RESPCHAN?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!ENUM
    //!CONTINUED|INTERVAL|SINGLE
    public int setISOLATORIN_SMODE(  string val0 )
    {
        //!设置隔离输入的采样模式
        //!ISOLATORIN SMODE
        string args;
        args = string.Format("ISOLATORIN:SMODE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_TRIGGER_PATTSMODE
    //!val0
    public int getISOLATORIN_SMODE(  out string val0 )
    {
        //!查询隔离输入的采样模式
        //!ISOLATORIN SMODE

        //! init the out
        val0=null;

        string args;
        args = string.Format("ISOLATORIN:SMODE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!U32
    //!
    public int setISOLATORIN_SPERIOD(  UInt32 val0 )
    {
        //!设置隔离输入的采样周期，单位ms，最小值100
        //!ISOLATORIN SPERIOD
        string args;
        args = string.Format("ISOLATORIN:SPERIOD {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!uint32
    //!val0
    public int getISOLATORIN_SPERIOD(  out UInt32 val0 )
    {
        //!查询隔离输入的采样周期
        //!ISOLATORIN SPERIOD

        //! init the out
        val0=0;

        string args;
        args = string.Format("ISOLATORIN:SPERIOD?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!ENUM,ENUM
    //!S1|S2|S3|S4,OFF|ON
    public int setABSENCALARM_STATE(  string val0, string val1 )
    {
        //!设置绝对值编码器报警状态
        //!ABSENCALARM STATE
        string args;
        args = string.Format("ABSENCALARM:STATE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!S1|S2|S3|S4
    //!MRQ_SYSTEM_REVMOTION
    //!val1
    public int getABSENCALARM_STATE(  string val0 ,  out string val1 )
    {
        //!查询编码器报警状态
        //!ABSENCALARM STATE

        //! init the out
        val1=null;

        string args;
        args = string.Format("ABSENCALARM:STATE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,U32
    //!S1|S2|S3|S4
    public int setABSENCALARM_UPLIMIT(  string val0, UInt32 val1 )
    {
        //!设置编码器角度上限
        //!ABSENCALARM UPLIMIT
        string args;
        args = string.Format("ABSENCALARM:UPLIMIT {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!S1|S2|S3|S4
    //!uint32
    //!val1
    public int getABSENCALARM_UPLIMIT(  string val0 ,  out UInt32 val1 )
    {
        //!查询编码器角度上限
        //!ABSENCALARM UPLIMIT

        //! init the out
        val1=0;

        string args;
        args = string.Format("ABSENCALARM:UPLIMIT? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!ENUM,U32
    //!S1|S2|S3|S4
    public int setABSENCALARM_DOWNLIMIT(  string val0, UInt32 val1 )
    {
        //!设置编码器角度下限
        //!ABSENCALARM DOWNLIMIT
        string args;
        args = string.Format("ABSENCALARM:DOWNLIMIT {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!S1|S2|S3|S4
    //!uint32
    //!val1
    public int getABSENCALARM_DOWNLIMIT(  string val0 ,  out UInt32 val1 )
    {
        //!查询编码器角度下限
        //!ABSENCALARM DOWNLIMIT

        //! init the out
        val1=0;

        string args;
        args = string.Format("ABSENCALARM:DOWNLIMIT? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!ENUM,U32
    //!S1|S2|S3|S4
    public int setABSENCALARM_ZEROVALUE(  string val0, UInt32 val1 )
    {
        //!设置编码器零位的值
        //!ABSENCALARM ZEROVALUE
        string args;
        args = string.Format("ABSENCALARM:ZEROVALUE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!S1|S2|S3|S4
    //!uint32
    //!val1
    public int getABSENCALARM_ZEROVALUE(  string val0 ,  out UInt32 val1 )
    {
        //!查询编码器零位的值
        //!ABSENCALARM ZEROVALUE

        //! init the out
        val1=0;

        string args;
        args = string.Format("ABSENCALARM:ZEROVALUE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!ENUM,ENUM
    //!S1|S2|S3|S4,NONE|EXIS
    public int setABSENCALARM_ZEROPOSITION(  string val0, string val1 )
    {
        //!设置上下限之间是否存在零点位置
        //!ABSENCALARM ZEROPOSITION
        string args;
        args = string.Format("ABSENCALARM:ZEROPOSITION {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!S1|S2|S3|S4
    //!MRQ_ABSENCALARM_ZEROPOSITION_1
    //!val1
    public int getABSENCALARM_ZEROPOSITION(  string val0 ,  out string val1 )
    {
        //!查询上下限之间是否存在零点位置
        //!ABSENCALARM ZEROPOSITION

        //! init the out
        val1=null;

        string args;
        args = string.Format("ABSENCALARM:ZEROPOSITION? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM
    //!NONE|ALARM|STOP|ALARM&STOP
    public int setABSENCALARM_RESPONSE(  string val0 )
    {
        //!设置触发上下限后的响应
        //!ABSENCALARM RESPONSE
        string args;
        args = string.Format("ABSENCALARM:RESPONSE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
    //!val0
    public int getABSENCALARM_RESPONSE(  out string val0 )
    {
        //!查询响应
        //!ABSENCALARM RESPONSE

        //! init the out
        val0=null;

        string args;
        args = string.Format("ABSENCALARM:RESPONSE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM,ENUM
    //!S1|S2|S3|S4,OFF|ON
    public int setDISTANCEALARM_STATE(  string val0, string val1 )
    {
        //!设置测距报警的状态
        //!DISTANCEALARM STATE
        string args;
        args = string.Format("DISTANCEALARM:STATE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!S1|S2|S3|S4
    //!MRQ_SYSTEM_REVMOTION
    //!val1
    public int getDISTANCEALARM_STATE(  string val0 ,  out string val1 )
    {
        //!查询测距报警的状态
        //!DISTANCEALARM STATE

        //! init the out
        val1=null;

        string args;
        args = string.Format("DISTANCEALARM:STATE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!ENUM,U16
    //!S1|S2|S3|S4
    public int setDISTANCEALARM_ALARM1DIST(  string val0, UInt16 val1 )
    {
        //!机械臂测距传感器1级警报触发距离，当检测到距离小于此值时开始通过总线发送警报
        //!DISTANCEALARM ALARM1DIST
        string args;
        args = string.Format("DISTANCEALARM:ALARM1DIST {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!S1|S2|S3|S4
    //!uint16
    //!val1
    public int getDISTANCEALARM_ALARM1DIST(  string val0 ,  out UInt16 val1 )
    {
        //!查询1级警报的触发距离
        //!DISTANCEALARM ALARM1DIST

        //! init the out
        val1=0;

        string args;
        args = string.Format("DISTANCEALARM:ALARM1DIST? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt16( paras[0] );
        return 0;
    }

    //!ENUM,U16
    //!S1|S2|S3|S4
    public int setDISTANCEALARM_ALARM2DIST(  string val0, UInt16 val1 )
    {
        //!设置2级警报的触发距离，当检测到距离小于此值时开始减速停止并发送警报
        //!DISTANCEALARM ALARM2DIST
        string args;
        args = string.Format("DISTANCEALARM:ALARM2DIST {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!S1|S2|S3|S4
    //!uint16
    //!val1
    public int getDISTANCEALARM_ALARM2DIST(  string val0 ,  out UInt16 val1 )
    {
        //!查询2级警报的触发距离
        //!DISTANCEALARM ALARM2DIST

        //! init the out
        val1=0;

        string args;
        args = string.Format("DISTANCEALARM:ALARM2DIST? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt16( paras[0] );
        return 0;
    }

    //!ENUM,U16
    //!S1|S2|S3|S4
    public int setDISTANCEALARM_ALARM3DIST(  string val0, UInt16 val1 )
    {
        //!设置3级警报的触发距离，当检测到距离小于此值时立即停止并发送警报
        //!DISTANCEALARM ALARM3DIST
        string args;
        args = string.Format("DISTANCEALARM:ALARM3DIST {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!ENUM
    //!S1|S2|S3|S4
    //!uint16
    //!val1
    public int getDISTANCEALARM_ALARM3DIST(  string val0 ,  out UInt16 val1 )
    {
        //!查询3级警报的触发距离
        //!DISTANCEALARM ALARM3DIST

        //! init the out
        val1=0;

        string args;
        args = string.Format("DISTANCEALARM:ALARM3DIST? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt16( paras[0] );
        return 0;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_NEWDRIVER_TYPE
    //!val1
    public int getNEWDRIVER_TYPE(  byte val0 ,  out string val1 )
    {
        //!查询驱动板类型
        //!NEWDRIVER TYPE

        //! init the out
        val1=null;

        string args;
        args = string.Format("NEWDRIVER:TYPE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8
    //!
    public int setNEWDRIVER_CURRENT(  byte val0 )
    {
        //!设置驱动工作最大电流，单位0.1A
        //!NEWDRIVER CURRENT
        string args;
        args = string.Format("NEWDRIVER:CURRENT {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!byte
    //!val0
    public int getNEWDRIVER_CURRENT(  out byte val0 )
    {
        //!查询驱动工作最大电流
        //!NEWDRIVER CURRENT

        //! init the out
        val0=0;

        string args;
        args = string.Format("NEWDRIVER:CURRENT?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!ENUM
    //!RESERVE|RESERVE1|RESERVE2|32|16|8|4|2|1
    public int setNEWDRIVER_MICROSTEPS(  string val0 )
    {
        //!设置电机微步数
        //!NEWDRIVER MICROSTEPS
        string args;
        args = string.Format("NEWDRIVER:MICROSTEPS {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_NEWDRIVER_MICROSTEPS
    //!val0
    public int getNEWDRIVER_MICROSTEPS(  out string val0 )
    {
        //!查询电机微步数
        //!NEWDRIVER MICROSTEPS

        //! init the out
        val0=null;

        string args;
        args = string.Format("NEWDRIVER:MICROSTEPS?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],OFF|ON
    public int setNEWDRIVER_STATE(  byte val0, string val1 )
    {
        //!设置驱动开关状态
        //!NEWDRIVER STATE
        string args;
        args = string.Format("NEWDRIVER:STATE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_SYSTEM_REVMOTION
    //!val1
    public int getNEWDRIVER_STATE(  byte val0 ,  out string val1 )
    {
        //!查询驱动开关状态
        //!NEWDRIVER STATE

        //! init the out
        val1=null;

        string args;
        args = string.Format("NEWDRIVER:STATE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,ENUM
    //![CHANNUM],END|START
    public int setPDM_SAMPLESTATE(  byte val0, string val1 )
    {
        //!
        //!PDM SAMPLESTATE
        string args;
        args = string.Format("PDM:SAMPLESTATE {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!MRQ_CLOCK_SYNCSTATE
    //!val1
    public int getPDM_SAMPLESTATE(  byte val0 ,  out string val1 )
    {
        //!
        //!PDM SAMPLESTATE

        //! init the out
        val1=null;

        string args;
        args = string.Format("PDM:SAMPLESTATE? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = paras[0];
        return 0;
    }

    //!U8,U8
    //![CHANNUM]
    public int setPDM_ENCDIV(  byte val0, byte val1 )
    {
        //!
        //!PDM ENCDIV
        string args;
        args = string.Format("PDM:ENCDIV {0},{1}" , val0,val1);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!U8
    //![CHANNUM]
    //!byte
    //!val1
    public int getPDM_ENCDIV(  byte val0 ,  out byte val1 )
    {
        //!
        //!PDM ENCDIV

        //! init the out
        val1=0;

        string args;
        args = string.Format("PDM:ENCDIV? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!U8
    //![CHANNUM]
    //!uint16
    //!val1
    public int getPDM_MICSTEPCOUNT(  byte val0 ,  out UInt16 val1 )
    {
        //!MICSTEPDATA?U8,U16,U16
        //!PDM MICSTEPCOUNT

        //! init the out
        val1=0;

        string args;
        args = string.Format("PDM:MICSTEPCOUNT? {0}" , val0);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val1 = Convert.ToUInt16( paras[0] );
        return 0;
    }

    //!U8,U16,U16
    //!
    //!byte
    //!val3
    public int getPDM_MICSTEPDATA(  byte val0, UInt16 val1, UInt16 val2 ,  out byte val3 )
    {
        //!
        //!PDM MICSTEPDATA

        //! init the out
        val3=0;

        string args;
        args = string.Format("PDM:MICSTEPDATA? {0},{1},{2}" , val0,val1,val2);
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val3 = Convert.ToByte( paras[0] );
        return 0;
    }

    //!ENUM
    //!OFF|ON
    public int setOTP_STATE(  string val0 )
    {
        //!打开或关闭OTP功能(Overtemperatureprotection)
        //!OTP STATE
        string args;
        args = string.Format("OTP:STATE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_SYSTEM_REVMOTION
    //!val0
    public int getOTP_STATE(  out string val0 )
    {
        //!查询OTP状态
        //!OTP STATE

        //! init the out
        val0=null;

        string args;
        args = string.Format("OTP:STATE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!U16
    //!
    public int setOTP_THRESHOLD(  UInt16 val0 )
    {
        //!设置过温保护的阈值，电机温度大于等于此值后执行响应动作
        //!OTP THRESHOLD
        string args;
        args = string.Format("OTP:THRESHOLD {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!uint16
    //!val0
    public int getOTP_THRESHOLD(  out UInt16 val0 )
    {
        //!查询过温保护阈值
        //!OTP THRESHOLD

        //! init the out
        val0=0;

        string args;
        args = string.Format("OTP:THRESHOLD?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt16( paras[0] );
        return 0;
    }

    //!ENUM
    //!NONE|ALARM|STOP|ALARM&STOP
    public int setOTP_RESPONSE(  string val0 )
    {
        //!发生过温保护响应
        //!OTP RESPONSE
        string args;
        args = string.Format("OTP:RESPONSE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
    //!val0
    public int getOTP_RESPONSE(  out string val0 )
    {
        //!查询过温保护响应
        //!OTP RESPONSE

        //! init the out
        val0=null;

        string args;
        args = string.Format("OTP:RESPONSE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!U32
    //!
    public int setOTP_PERIOD(  UInt32 val0 )
    {
        //!设置上报周期，单位ms
        //!OTP PERIOD
        string args;
        args = string.Format("OTP:PERIOD {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!uint32
    //!val0
    public int getOTP_PERIOD(  out UInt32 val0 )
    {
        //!查询上报周期
        //!OTP PERIOD

        //! init the out
        val0=0;

        string args;
        args = string.Format("OTP:PERIOD?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!
    //!
    //!uint32
    //!val0
    public int getOTP_DATA(  out UInt32 val0 )
    {
        //!上报数据，上报打开时下位机按照上报周期主动上报；上位机也可以主动查询
        //!OTP DATA

        //! init the out
        val0=0;

        string args;
        args = string.Format("OTP:DATA?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToUInt32( paras[0] );
        return 0;
    }

    //!ENUM
    //!OFF|ON
    public int setANALOGIN_STATE(  string val0 )
    {
        //!打开或关闭模拟传感器
        //!ANALOGIN STATE
        string args;
        args = string.Format("ANALOGIN:STATE {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_SYSTEM_REVMOTION
    //!val0
    public int getANALOGIN_STATE(  out string val0 )
    {
        //!查询模拟传感器的状态
        //!ANALOGIN STATE

        //! init the out
        val0=null;

        string args;
        args = string.Format("ANALOGIN:STATE?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!F32
    //!
    public int setANALOGIN_THRESHOLDH(  float val0 )
    {
        //!设置传感器触发电压阈值高
        //!ANALOGIN THRESHOLDH
        string args;
        args = string.Format("ANALOGIN:THRESHOLDH {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!f32
    //!val0
    public int getANALOGIN_THRESHOLDH(  out float val0 )
    {
        //!查询传感器触发电压阈值高
        //!ANALOGIN THRESHOLDH

        //! init the out
        val0=0;

        string args;
        args = string.Format("ANALOGIN:THRESHOLDH?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!F32
    //!
    public int setANALOGIN_THRESHOLDL(  float val0 )
    {
        //!设置传感器触发电压阈值低
        //!ANALOGIN THRESHOLDL
        string args;
        args = string.Format("ANALOGIN:THRESHOLDL {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!f32
    //!val0
    public int getANALOGIN_THRESHOLDL(  out float val0 )
    {
        //!查询传感器触发电压阈值低
        //!ANALOGIN THRESHOLDL

        //! init the out
        val0=0;

        string args;
        args = string.Format("ANALOGIN:THRESHOLDL?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = Convert.ToSingle( paras[0] );
        return 0;
    }

    //!ENUM
    //!NONE|ALARM|STOP|ALARM&STOP
    public int setANALOGIN_RESPONSEH(  string val0 )
    {
        //!设置传感器电压阈值高的触发响应
        //!ANALOGIN RESPONSEH
        string args;
        args = string.Format("ANALOGIN:RESPONSEH {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
    //!val0
    public int getANALOGIN_RESPONSEH(  out string val0 )
    {
        //!查询传感器电压阈值高的触发响应
        //!ANALOGIN RESPONSEH

        //! init the out
        val0=null;

        string args;
        args = string.Format("ANALOGIN:RESPONSEH?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

    //!ENUM
    //!NONE|ALARM|STOP|ALARM&STOP
    public int setANALOGIN_RESPONSEL(  string val0 )
    {
        //!设置传感器电压阈值低的触发响应
        //!ANALOGIN RESPONSEL
        string args;
        args = string.Format("ANALOGIN:RESPONSEL {0}" , val0);
        int ret;
        ret = miSend( args );
        return ret;
    }

    //!
    //!
    //!MRQ_MOTIONPLAN_OOSLINERESPONSE_1
    //!val0
    public int getANALOGIN_RESPONSEL(  out string val0 )
    {
        //!查询传感器电压阈值低的触发响应
        //!ANALOGIN RESPONSEL

        //! init the out
        val0=null;

        string args;
        args = string.Format("ANALOGIN:RESPONSEL?"   );
        int ret;
        ret = miSend( args );
        if ( ret != 0 ){ return ret; }
        ret = miWaitRead();
        if ( ret != 0 ){ return ret; }

        string strRecv;
        int retCount;
        ret = miRecv( out strRecv, out retCount);
        if ( ret != 0 ){ return ret; }
        if ( retCount < 1 ){ return -1; }
        string [] paras=strRecv.Split(',');
        if ( paras.Length < 1 )
        { return -1; } 
        val0 = paras[0];
        return 0;
    }

}
}
