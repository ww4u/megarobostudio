TEMPLATE = subdirs

SUBDIRS = ../source/device/device.pro \
          ../source/bus/bus.pro \
          ../source/inst/inst.pro \
          ../source/scpi-parser/scpilib.pro \
    ../source/scpishell/scpishell.pro \
    ../source/instServer/instServer.pro \
    ../source/instClient/instClient.pro \
    ../source/arith/arith.pro \
    ../source/robot/robot.pro \
    ../source/para/para.pro \
    ../source/mrp/mrp.pro \
    ../source/mrg/mrg.pro

contains( ARCH_HOST, raspberry ) {
    SUBDIRS += ../source/bus/bspcan/bspcan.pro
}



#TRANSLATIONS = megarobostudio_lib_zh.ts
