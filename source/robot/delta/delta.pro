TEMPLATE=lib
QT       += gui
CONFIG += static

TARGET = ../../../../lib/delta

INCLUDEPATH += ../../scpi-parser/libscpi/inc

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    delta.cpp \
    _calibration.cpp \
    _plan.cpp \
    _scpi_callback.cpp \
    _download.cpp \
    _movement.cpp \
    _setup.cpp \
    _build.cpp

HEADERS = \
    delta.h


