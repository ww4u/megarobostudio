TEMPLATE=lib
QT       -= gui
CONFIG += static

TARGET = ../../../../lib/kinematic

DEFINES += EXPORT_LIBRARY

OBJECTS_DIR = ./obj
MOC_DIR = ../obj

SOURCES = \
    kinematic.cpp
	
HEADERS = \
        kinematic.h
