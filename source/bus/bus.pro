TEMPLATE=lib
#QT       = gui
CONFIG += static

#LIBS += -L$$PWD/../../lib/ -lMegaCanDevice

#QMAKE_LFLAGS += -enable-stdcall-fixup


INCLUDEPATH += ../scpi-parser/libscpi/inc
#INCLUDEPATH += "C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Include"
#INCLUDEPATH += "G:\work\lib\MRGateWay_PC\MegaCanDevice_VC"

TARGET = ../../../lib/bus

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    ibus.cpp \
    canbus.cpp \
    deviceid.cpp \
    receivecache.cpp \
    filebus.cpp \
    canapi.cpp

HEADERS = \
    ibus.h \
    canbus.h \
    deviceid.h \
    receivecache.h \
    filebus.h \
    canapi.h

