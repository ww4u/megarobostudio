QT -= core

#TEMPLATE = app
TEMPLATE= lib

CONFIG += static

TARGET = ../../../../lib/bspcan

#INCLUDEPATH += ../include

#SOURCES += ./bspCan.cpp \
#           main.cpp

SOURCES += ./bspCan.cpp \
    bspcan_packer.cpp

HEADERS += \
    canapi.h
