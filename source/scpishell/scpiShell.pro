#-------------------------------------------------
#
# Project created by QtCreator 2018-01-24T17:14:13
#
#-------------------------------------------------

QT       -= gui

TARGET = ../../../lib/scpishell
TEMPLATE = lib
CONFIG += staticlib

LIBS += -L$$PWD/../../lib
#LIBS += -LG:/work/mc/develope/lib
LIBS += -lscpi
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += EXPORT_LIBRARY
DEFINES += STATIC_LIB

INCLUDEPATH += ../scpi-parser/libscpi/inc

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        scpishell.cpp

HEADERS += \
        scpishell.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
