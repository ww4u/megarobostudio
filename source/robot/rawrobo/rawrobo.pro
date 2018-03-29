TEMPLATE=lib
QT       += gui
CONFIG += static

TARGET = ../../../../lib/rawrobo

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
    rawrobo.cpp \
    rawrobo_fsm.cpp \
    rawrobo_state.cpp \
    rawrobo_cond.cpp

HEADERS = \
    rawrobo.h


