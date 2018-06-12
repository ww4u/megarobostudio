TEMPLATE=lib
QT       += gui
CONFIG += static

INCLUDEPATH += ../../scpi-parser/libscpi/inc
TARGET = ../../../../lib/injectpump

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    injectpump.cpp \
    _scpi_callback.cpp \
    _plan.cpp \
    _movement.cpp \
    _download.cpp \
    _state.cpp \
    _calibration.cpp \
    _build.cpp \
    injectpump_task.cpp \
    _action.cpp

HEADERS = \
    injectpump.h


