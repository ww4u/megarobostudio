TEMPLATE=lib
QT       -= gui
CONFIG += static


TARGET = ../../../../lib/kinematic

DEFINES += EXPORT_LIBRARY

OBJECTS_DIR = ./obj
MOC_DIR = ../obj

SOURCES = \
    functions.cpp
	
HEADERS = \
    param.h \
        kinematic.h
