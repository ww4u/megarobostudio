TEMPLATE=lib
QT       += gui
CONFIG += static

TARGET = ../../../../lib/motors

INCLUDEPATH += ../../scpi-parser/libscpi/inc

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    motors.cpp \
    _movement.cpp \
    _download.cpp \
    _scpi_callback.cpp \
    _plan.cpp \
    _zero.cpp \
    motors_task.cpp \
    _serial.cpp

HEADERS = \
    motors.h


