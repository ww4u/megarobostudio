TEMPLATE=lib

QT += core gui widgets network

CONFIG += static

TARGET = ../../../lib/device


INCLUDEPATH += ../scpi-parser/libscpi/inc

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = vdevice.cpp \
          devicemodel.cpp \
          ./board/mrq.cpp \
          ./board/_mrq_model.cpp \
          ./board/MRQ_enum.cpp \
          ./board/mrq_model.cpp \
          ./mrq/deviceMRQ.cpp \
          ./mrq/MRQ_scpi_callback.cpp \
          ./mrq/tpvdownloader.cpp \
    vrobot.cpp \
    roboworker.cpp \
    vrobot_serial.cpp \
    vrobot_msg.cpp \
    vrobot_action.cpp \
    vrobot_prop.cpp \
    mrq/devicemrq_transaction.cpp \
    mrq/devicemrq_action.cpp \
    motor/motor.cpp \
    robostate.cpp \
    mrq/devicemrq_state.cpp

HEADERS = vdevice.h \
          devicemodel.h \
          ./board/mrq.h \
          ./board/_mrq_model.h \
          ./board/mrq_model.h \
          ./mrq/deviceMRQ.h \
          ./mrq/tpvdownloader.h \
          ./motor/motor.h \
    vrobot.h \
    roboworker.h \
    mrq/devicemrq_state.h \
    robostate.h \
    mrq/devicemrq_msg.h

