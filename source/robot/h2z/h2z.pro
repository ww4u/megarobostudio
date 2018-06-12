TEMPLATE=lib
QT       += gui
CONFIG += static

INCLUDEPATH += ../../scpi-parser/libscpi/inc
TARGET = ../../../../lib/h2z

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    h2z.cpp \
    _scpi_callback.cpp \
    _plan.cpp \
    _movement.cpp \
    _download.cpp \
    _state.cpp \
    _calibration.cpp \
    _build.cpp \
    h2z_task.cpp

HEADERS = \
    h2z.h


