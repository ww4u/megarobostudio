TEMPLATE=lib
QT       += gui
CONFIG += static

INCLUDEPATH += ../../scpi-parser/libscpi/inc
TARGET = ../../../../lib/megatron

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    megatron.cpp \
    _scpi_callback.cpp \
    _plan.cpp \
    _movement.cpp \
    _download.cpp \
    _state.cpp \
    _calibration.cpp \
    _setup.cpp \
    _build.cpp

HEADERS = \
    megatron.h


