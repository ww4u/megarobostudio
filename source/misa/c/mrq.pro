#TEMPLATE=lib
TEMPLATE=app

# QT += core gui widgets network

CONFIG += static

TARGET = mrq 

win32 {
# visa
INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include"
LIBS += -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc"
LIBS += -lvisa32
DEFINES += NI_VISA
}

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

SOURCES = \
          visa_adapter.c \
                  C_MRQ.c \
                  mrq.c \
          midevice.c \
          megarobo.c \
          sinanju.c \
          h2.c \
          mimgr.c \
          test.cpp
		  
HEADERS = visa_adapter.h \
    megarobo.h \
    sinanju.h \
    h2.h \
    misa.h

