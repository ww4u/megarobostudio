win32 {
DEFINES += ARCH_32

INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include"
LIBS += -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc"
LIBS += -lvisa32
DEFINES += NI_VISA

}

linux {
DEFINES += ARCH_LINUX
}

if ( linux-arm* ){

DEFINES += ARCH_RASPBERRY
DEFINES -= ARCH_LINUX

}
