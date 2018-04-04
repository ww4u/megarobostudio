TEMPLATE=lib
QT       -= gui
CONFIG += static


TARGET = ../../../../lib/arith_delta

DEFINES += EXPORT_LIBRARY

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    angleOperate.c \
	calAcceleration.c \
	calPosition.c \
	calVelocity.c \
	configure.c \
        dllMain.c \
        test.c
	
HEADERS = \
       angleOperate.h \
	   calAcceleration.h \
	   calPosition.h \
	   calVelocity.h \
	   configure.h \
	   dllMain.h 

