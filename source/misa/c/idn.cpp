
#include "stdio.h"

//! misa头文件
//! 文件路径在工程里面设置 INCLUDEPATH
//! 链接需要visa32.lib 需要设置LIB路径
#include "misa.h"


void testIdn()
{
    //! 打开设备
    //! "mrx-t4"是设备在MEGAROBO Studio当前场景中的名称
    ViSession vi = miOpen( "mrx-t4" );

    //! 从设备读取标记
    char recvStr[512];
    robo_getIdn( vi, recvStr );
    printf("idn:%s\n", recvStr );

    //! 关闭设备
    miClose( vi );
}

int main()
{
    testIdn();

    return 0;
}
