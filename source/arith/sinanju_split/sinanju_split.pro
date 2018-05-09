TEMPLATE=lib
QT       -= gui
CONFIG += static

TARGET = ../../../../lib/sinanju_split

DEFINES += EXPORT_LIBRARY

OBJECTS_DIR = ../obj
MOC_DIR = ../obj

SOURCES = \
    sinanju_split.cpp
	
HEADERS = \
        sinanju_split.h
