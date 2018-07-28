QT += network
TEMPLATE=lib
CONFIG += static

TARGET = ../../../lib/inst

INCLUDEPATH += ../scpi-parser/libscpi/inc

include(../../prj/config.pri)

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = instmgr.cpp \
    vrobolist.cpp

HEADERS = instmgr.h \
    vrobolist.h

