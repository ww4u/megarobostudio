TEMPLATE=lib
QT       += gui
CONFIG += static

INCLUDEPATH += ../../scpi-parser/libscpi/inc
TARGET = ../../../../lib/h2m

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    h2m.cpp \

HEADERS = \
    h2m.h


