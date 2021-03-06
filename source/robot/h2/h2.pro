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
    _state.cpp \
    _calibration.cpp \
    _build.cpp \
    h2_task.cpp \
    _serial.cpp

HEADERS = \
    h2.h


