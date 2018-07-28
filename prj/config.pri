win32 {
DEFINES += ARCH_32
}

linux {
DEFINES += ARCH_LINUX
}

if ( linux-arm* ){

DEFINES += ARCH_RASPBERRY
DEFINES -= ARCH_LINUX

}
