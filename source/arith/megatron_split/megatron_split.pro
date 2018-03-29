TEMPLATE=lib
QT       -= gui
CONFIG += static


TARGET = ../../../../lib/megatron_split

DEFINES += EXPORT_LIBRARY

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    megatron_split.cpp
	
HEADERS = \
        megatron_split.h
