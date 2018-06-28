TEMPLATE=lib

QT += core gui widgets network

CONFIG += static

TARGET = ../../../lib/device


INCLUDEPATH += ../scpi-parser/libscpi/inc

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
          ./board/MRQ.cpp \
          ./board/MRQ_enum.cpp \
          ./board/mrq_model.cpp \
          ./board/mrq_model_serial.cpp \
          ./board/_mrq_model.cpp \
          \
    \
        ./mrv_board/MRV_enum.cpp \
        ./mrv_board/MRV.cpp \
        ./mrv_board/MRV_model.cpp \
        ./mrv_board/_mrv_model.cpp \
        \
    vrobot_serial.cpp \
    vrobot_msg.cpp \
    vrobot_action.cpp \
    vrobot_prop.cpp \
    vrobot_task.cpp \
    vdevice.cpp \
    devicemodel.cpp \
    robostate.cpp \
    vrobot.cpp \
    roboworker.cpp \
    motor/motor.cpp \
    ./mrq/devicemrq_transaction.cpp \
    ./mrq/devicemrq_action.cpp \
    ./mrq/deviceMRQ.cpp \
    ./mrq/MRQ_scpi_callback.cpp \
    ./mrq/tpvdownloader.cpp \
    ./mrq/devicemrq_state.cpp \
    ./mrq/devicemrq_addr.cpp \
    ./mrq/devicemrq_setting.cpp \
    ./mrq/devicemrq_tpv.cpp \
    ./mrq/devicemrq_setting_up.cpp \
    ./mrq/devicemrq_setting_down.cpp \
    ./mrq/devicemrq_assist.cpp \
    ./mrq/devicemrq_task.cpp \
    mrv/devicemrv.cpp \
    phydevice.cpp \
    mrq/datauploader.cpp \
    mrv_board/mrv_model_serial.cpp \
    mrv/mrv_scpi_callback.cpp \
    mrv/devicemrv_setting.cpp \
    mrv/tpdownloader.cpp \
    mrv/devicemrv_tp.cpp




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
    mrq/devicemrq_msg.h \
    mrv/devicemrv.h \
    phydevice.h \
    mrq/datauploader.h \
    mrv/tpdownloader.h

