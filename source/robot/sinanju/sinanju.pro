TEMPLATE=lib
QT       += gui
CONFIG += static

INCLUDEPATH += ../../scpi-parser/libscpi/inc
TARGET = ../../../../lib/sinanju

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    sinanju.cpp \
    _scpi_callback.cpp \
    _plan.cpp \
    _movement.cpp \
    _download.cpp \
    _state.cpp \
    _calibration.cpp \
    _setup.cpp

HEADERS = \
    sinanju.h



