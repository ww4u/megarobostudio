TEMPLATE=lib
QT       += gui
CONFIG += static

TARGET = ../../../../lib/igus_delta

INCLUDEPATH += ../../scpi-parser/libscpi/inc

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    igus_delta.cpp \
    _calibration.cpp \
    _plan.cpp \
    _scpi_callback.cpp \
    _download.cpp \
    _movement.cpp \
    _setup.cpp \
    _build.cpp \
    igus_delta_task.cpp

HEADERS = \
    igus_delta.h


