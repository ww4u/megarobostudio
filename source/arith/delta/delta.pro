TEMPLATE=lib
QT       -= gui
CONFIG += static


TARGET = ../../../../lib/arith_delta

DEFINES += EXPORT_LIBRARY

INCLUDEPATH += ./header

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    arith_delta.cpp \
    deltadatacache.cpp
	
HEADERS = \
    arith_delta.h \
    deltadatacache.h

