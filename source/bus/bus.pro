TEMPLATE=lib
#QT       = gui
CONFIG += static
QT += network

#LIBS += -L$$PWD/../../lib/ -lMegaCanDevice

#QMAKE_LFLAGS += -enable-stdcall-fixup

include(../../prj/config.pri)

INCLUDEPATH += ../scpi-parser/libscpi/inc

TARGET = ../../../lib/bus

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    ibus.cpp \
    canbus.cpp \
    deviceid.cpp \
    receivecache.cpp \
    filebus.cpp \
    canapi.cpp \
    rs232bus.cpp \
    rs232_adapt.cpp

HEADERS = \
    ibus.h \
    canbus.h \
    deviceid.h \
    receivecache.h \
    filebus.h \
    canapi.h \
    rs232bus.h

