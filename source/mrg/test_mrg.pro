QT += core

#TEMPLATE=lib
TEMPLATE=app

#CONFIG += static

TARGET = ../../../lib/mrg

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = main.cpp \
    entity.cpp \
    robot.cpp \
    mrq.cpp \
    mrxt4.cpp \
    operator.cpp \
    operand.cpp \
    link.cpp \
    comoperator.cpp \
    operation.cpp \
    robographmodel.cpp \
    entityfactory.cpp

HEADERS = \ 
    entity.h \
    robot.h \
    mrq.h \
    mrxt4.h \
    operator.h \
    operand.h \
    link.h \
    comoperator.h \
    operation.h \
    robographmodel.h \
    entityfactory.h
    


