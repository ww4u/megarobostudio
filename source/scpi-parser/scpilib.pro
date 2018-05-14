TEMPLATE = lib

QT -= gui
CONFIG += static

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

TARGET = ../../../lib/scpi

INCLUDEPATH += libscpi/inc

SOURCES = \
                libscpi/src/error.c \
                libscpi/src/expression.c \
                libscpi/src/fifo.c \
                libscpi/src/ieee488.c \
                libscpi/src/lexer.c \
                libscpi/src/minimal.c \
                libscpi/src/parser.c \
                libscpi/src/utils.c \
                libscpi/src/units.c


HEADERS = \
                libscpi/src/fifo_private.h \
                libscpi/src/lexer_private.h \
                libscpi/src/parser_private.h \
                libscpi/src/utils_private.h
		
