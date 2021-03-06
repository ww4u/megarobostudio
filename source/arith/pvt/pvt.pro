TEMPLATE=lib
QT       += gui
CONFIG += static


TARGET = ../../../../lib/pvt

win32 {
INCLUDEPATH += "D:/Program Files (x86)/IAR Systems/Embedded Workbench 7.5/arm/CMSIS/Include"
}
unix{
INCLUDEPATH += "/home/w/work/arm/include"
}

macx{
INCLUDEPATH += "/home/w/work/megarobostudio/arm/include"
}

INCLUDEPATH += ./GelgoogProject/APP/Sources/Common
INCLUDEPATH += ./GelgoogProject/APP/Sources
INCLUDEPATH += ./GelgoogProject/BSP/BSP

INCLUDEPATH += ./GelgoogProject/BSP/BSP
INCLUDEPATH += ./GelgoogProject/BSP/HardwareStdlib/CMSIS
INCLUDEPATH += ./GelgoogProject/BSP/HardwareStdlib/Source/STM32F4xx_StdPeriph_Driver/inc

DEFINES += EXPORT_LIBRARY

DEFINES += ARM_MATH_CM4
DEFINES +=  USE_STDPERIPH_DRIVER STM32F40_41xxx
#STM32F427_437xx
OBJECTS_DIR = ../obj
MOC_DIR = ../obj

SOURCES = \
    ./GelgoogProject/PVT/Sources/pvtAlgorithm.c \
        ./GelgoogProject/PVT/Sources/pvtCubicSolution.c \
        ./GelgoogProject/PVT/Sources/pvtScurveSolution.c \
        ./GelgoogProject/PVT/Sources/pvtTrapezSolution.c \
#        ./GelgoogProject/PVT/Sources/pvtUniformSolution.c \
        pvt.cpp
		
HEADERS = \
        pvt.h


