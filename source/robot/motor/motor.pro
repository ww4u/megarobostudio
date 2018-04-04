TEMPLATE=lib
QT       += gui
CONFIG += static

TARGET = ../../../../lib/motor

INCLUDEPATH += ../../scpi-parser/libscpi/inc

OBJECTS_DIR = ../obj
MOC_DIR = ../obj

SOURCES = \
    motor.cpp \
    _movement.cpp \
    _download.cpp \
    _scpi_callback.cpp \
    _plan.cpp

HEADERS = \
    motor.h


