TEMPLATE=lib
QT       -= gui
CONFIG += static


TARGET = ../../../../lib/arith_igus

DEFINES += EXPORT_LIBRARY

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    arith_igus.cpp
	
HEADERS = \
        arith_igus.h
