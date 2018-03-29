TEMPLATE=lib
QT       += gui
CONFIG += static

INCLUDEPATH += ../../scpi-parser/libscpi/inc
TARGET = ../../../../lib/h2

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    h2.cpp \
    _scpi_callback.cpp \
    _plan.cpp \
    _movement.cpp \
    _download.cpp \
    _state.cpp

HEADERS = \
    h2.h


