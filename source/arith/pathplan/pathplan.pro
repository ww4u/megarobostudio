TEMPLATE=lib
QT       -= gui
CONFIG += static


TARGET = ../../../../lib/pathplan

DEFINES += EXPORT_LIBRARY

OBJECTS_DIR = ./obj
MOC_DIR = ../obj

SOURCES = \
    functions.cpp
	
HEADERS = \
    param.h \
        pathplan.h
