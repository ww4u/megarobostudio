TEMPLATE=lib
QT       -= gui
CONFIG += static


TARGET = ../../../../lib/h2_split

DEFINES += EXPORT_LIBRARY

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    h2_split.cpp
	
HEADERS = \
        h2_split.h
