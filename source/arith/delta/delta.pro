TEMPLATE=lib
QT       -= gui
CONFIG += static


TARGET = ../../../../lib/arith_delta

DEFINES += EXPORT_LIBRARY

INCLUDEPATH += ./header

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    ./source/angleOperate.c \
        ./source/calAcceleration.c \
        ./source/calPosition.c \
        ./source/calVelocity.c \
        ./source/configure.c \
        ./source/dllMain.c \
        ./source/oneFreedom.c \
        test.c \
    arith_delta.cpp \
    deltadatacache.cpp
	
HEADERS = \
       ./header/angleOperate.h \
           ./header/calAcceleration.h \
           ./header/calPosition.h \
           ./header/calVelocity.h \
           ./header/configure.h \
           ./header/dllMain.h \
           ./header/oneFreedom.h \
    arith_delta.h \
    deltadatacache.h

