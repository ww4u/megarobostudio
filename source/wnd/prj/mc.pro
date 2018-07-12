#-------------------------------------------------
#
# Project created by QtCreator 2017-12-20T08:26:00
#
#-------------------------------------------------

QT       += core gui charts sql network
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "MegaRobo Studio"
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VERSION = 0.1.0.8
#WINRT_MANIFEST.name = "Mega Robo Studio"

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

INCLUDEPATH += $$PWD/../ui

INCLUDEPATH += $$PWD/../../scpi-parser/libscpi/inc
INCLUDEPATH += $$PWD/../../widget

## for pvt path
#INCLUDEPATH += "D:/Program Files (x86)/IAR Systems/Embedded Workbench 7.5/arm/CMSIS/Include"
#INCLUDEPATH += $$PWD/../../arith/pvt/GelgoogProject/APP/Sources/Common
#INCLUDEPATH += $$PWD/../../arith/pvt/GelgoogProject/APP/Sources
#INCLUDEPATH += $$PWD/../../arith/pvt/GelgoogProject/BSP/BSP

#INCLUDEPATH += $$PWD/../../arith/pvt/GelgoogProject/BSP/BSP
#INCLUDEPATH += $$PWD/../../arith/pvt/GelgoogProject/BSP/HardwareStdlib/CMSIS
#INCLUDEPATH += $$PWD/../../arith/pvt/GelgoogProject/BSP/HardwareStdlib/Source/STM32F4xx_StdPeriph_Driver/inc


DEFINES += ARM_MATH_CM4
DEFINES += STM32F427_437xx USE_STDPERIPH_DRIVER

DEPENDPATH += $$PWD/../../../lib

# link libs
LIBS += -L$$PWD/../../../lib/
LIBS += -linst -lscpishell -lscpi
LIBS += -linstserver
LIBS += -linstclient

LIBS += -lrobotfact
LIBS += -lmegatron -lsinanju -ldelta -lh2 -lh2z -linjectpump
LIBS += -lmotor
#-lslide
LIBS += -lquebeley -lgeogoog -lgeo8 -lgeo10 -lgeo51
LIBS += -lgouf
LIBS += -lrawrobo

LIBS += -ldevice -lbus
LIBS += -lpara

win32 {
# visa
INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include"
LIBS += -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc"
LIBS += -lvisa32
DEFINES += NI_VISA
DEFINES += ARCH_32
}


# arith
LIBS += -lsinanju_split -lmegatron_split -lh2_split -larith_delta
LIBS += -lpathplan
LIBS += -lpvt

SOURCES += \
        ../main.cpp \
\
#        ../test/pvt_test.cpp \
#        ../test/kinematic_test.cpp \
#        ../test/robo_test.cpp \
#        ../test/megatron_split.cpp\
#    ../test/delta_test.cpp \
#        ../test/dataset_test.cpp \
#        ../test/h2_split.cpp \
\
\
        ../ui/mainwindow.cpp \
        ../ui/mrqinfo.cpp \
        ../ui/mrqproperty.cpp \
        ../ui/mrqaxes.cpp \
        ../ui/mrqio.cpp \
        ../ui/mrqsensor.cpp \
        ../ui/mrqsys.cpp \
    ../ui/devicemgr.cpp \
    ../ui/robomgr.cpp \
    ../ui/mrqaxes2.cpp \
    ../ui/scriptmgr.cpp \
    ../ui/roboinfo.cpp \
    ../ui/roboprop.cpp \
    ../ui/cfgtab.cpp \
    ../ui/roboaxes.cpp \
    ../../model/mcmodel.cpp \
    ../../model/tpvitem.cpp \
    ../../model/tpvgroup.cpp \
    ../../model/mcmodelobj.cpp \
    ../ui/aboutdlg.cpp \
    ../ui/logout.cpp \
    ../ui/pvtedit.cpp \
    ../ui/mainwindow_menu_slot.cpp \
    ../ui/statebar.cpp \
#    ../ui/runtool.cpp \
    ../../intf/iserial.cpp \
    ../../intf/imcstream.cpp \
    ../ui/tpvplot.cpp \
    ../ui/mrqview.cpp \
    ../ui/motionedit.cpp \
    ../ui/syspref.cpp \
    ../ui/mainwindow_sys.cpp \
    ../ui/modelview.cpp \
    ../../model/modelsyspref.cpp \
    ../../model/modelrobotmgr.cpp \
    ../ui/axesknob.cpp \
    ../../app/appwidget.cpp \
    ../../app/dpcobj.cpp \
    ../../model/scriptfile.cpp \
    ../../model/scriptgroup.cpp \
    ../../model/scriptmodel.cpp \
    ../ui/tableedit.cpp \
    ../ui/querymgr.cpp \
    ../ui/queryview.cpp \
    ../../com/comassist.cpp \
    ../../model/spycfgmodel.cpp \
    ../../model/spyitemmodel.cpp \
    ../../model/comboxdelegate.cpp \
    ../../com/drivercontroller.cpp \
    ../../model/checkdelegate.cpp \
#    ../../app/tpvdownloader.cpp \
    ../../app/sampleproxy.cpp \
    ../../app/samplemgr.cpp \
    ../../app/samplethread.cpp \
    ../../app/interrupthandler.cpp \
    ../ui/eventviewer.cpp \
    ../../model/dbmeta.cpp \
    ../ui/roboscene.cpp \
    ../../widget/codeeditor.cpp \
    ../../widget/highlighter.cpp \
    ../../widget/flowlayout.cpp \
    ../ui/scripteditor.cpp \
    ../ui/deviceconsole.cpp \
    ../ui/dlgwndlist.cpp \
    ../../widget/scenewidget.cpp \
    ../../model/roboscenemodel.cpp \
    ../ui/axesconnection.cpp \
    ../ui/roboconnection.cpp \
    ../../model/mcconnection.cpp \
    ../../com/robomsg.cpp \
    ../../app/robonetthread.cpp \
    ../ui/mainwindow_scene.cpp \
    ../../com/robomsgqueue.cpp \
    ../../com/robomsgthread.cpp \
    ../../app/systimer.cpp \
    ../ui/devicemgr_test.cpp \
    ../ui/progressgroup.cpp \
    ../ui/progresselement.cpp \
    ../ui/robohand.cpp \
    ../../model/handactionmodel.cpp \
    ../ui/motionedit_compile.cpp \
    ../ui/motionedit_download.cpp \
    ../../model/eventactionmodel.cpp \
    ../ui/warnprompt.cpp \
    ../ui/mainwindow_exception.cpp \
    ../../model/megatablemodel.cpp \
    ../ui/motionedit_graph.cpp \
    ../../widget/valuecombobox.cpp \
    ../ui/pvtpref.cpp \
    ../ui/motionpref.cpp \
    ../ui/anglemonitor.cpp \
    ../ui/anglewidget.cpp \
    ../ui/motorstate.cpp \
    ../ui/motormonitor.cpp \
    ../ui/mainwindow_progress.cpp \
    ../ui/deltapref.cpp \
    ../ui/megatronpref.cpp \
    ../ui/robocompref.cpp \
    ../ui/sinanjupref.cpp \
    ../ui/mrqdcaxes.cpp \
    ../ui/robodesc.cpp \
    ../ui/dlgview.cpp \
    ../ui/mrqalarm.cpp \
    ../ui/encoderalarmpage.cpp \
    ../ui/distancealarmpage.cpp \
    ../ui/robojoint.cpp \
    ../ui/mrquart.cpp \
    ../ui/mrqsubuart.cpp \
    ../ui/mrqsensorpage.cpp \
    ../ui/axnpref.cpp \
    ../ui/mrqtrigchan.cpp \
    ../ui/mrqtrigpage.cpp \
    ../ui/mrqaxesplan.cpp \
    ../../widget/megamessagebox.cpp \
    ../ui/mrqaxespage.cpp \
    ../ui/roboconfig.cpp \
    ../ui/sinanjuconfig.cpp \
    ../ui/deltaconfig.cpp \
    ../ui/testprop.cpp \
    ../../model/megatronmotiongroup.cpp \
    ../../model/megatronmotionitem.cpp \
    ../../model/sinanjumotionitem.cpp \
    ../../model/sinanjumotiongroup.cpp \
    ../ui/motionwizard.cpp \
    ../../model/h2motionitem.cpp \
    ../../model/h2motiongroup.cpp \
    ../ui/motionedit_slot.cpp \
    ../ui/h2pref.cpp \
    ../ui/statediagram.cpp \
    ../ui/h2config.cpp \
    ../../model/deltamotiongroup.cpp \
    ../ui/mrqin.cpp \
    ../ui/mrqyo.cpp \
    ../ui/mrqdo.cpp \
    ../ui/mrqxi.cpp \
    ../ui/mrqai.cpp \
    ../ui/lrbutton.cpp \
    ../../model/h2zmotiongroup.cpp \
    ../../model/h2zmotionitem.cpp \
    ../ui/quicktool.cpp \
    ../ui/ippref.cpp \
    ../ui/motionedit_post.cpp \
    ../ui/t4panel.cpp \
    ../../com/basetype.cpp \
    ../../model/ipmotiongroup.cpp \
    ../ui/ipconfig.cpp \
    ../ui/mrvvertical.cpp \
    ../ui/mrvrecord.cpp \
    ../ui/mrvthreshold.cpp \
    ../ui/mrvio.cpp \
    ../ui/mrvswitch.cpp \
    ../ui/mrvch.cpp \
    ../ui/mrvprop.cpp \
    ../ui/mrvinfo.cpp \
    ../ui/mrvsystem.cpp \
    ../ui/h2zpanel.cpp \
    ../ui/h2panel.cpp \
    ../ui/robopanel.cpp \
    ../../app/comthread.cpp \
    ../ui/mrvview.cpp \
    ../ui/mrxview.cpp \
    ../../model/tpgroup.cpp \
    ../../model/tpitem.cpp \
    ../ui/tpedit.cpp \
    ../ui/motorpref.cpp \
    ../ui/mainwindow_shell.cpp \
    ../../com/rpcthread.cpp

HEADERS += \
        ../ui/mainwindow.h \
        ../ui/mrqinfo.h \
        ../ui/mrqproperty.h \
        ../ui/mrqaxes.h \
        ../ui/mrqio.h \
        ../ui/mrqsensor.h \
        ../ui/mrqsys.h \
    ../ui/devicemgr.h \
    ../ui/robomgr.h \
    ../ui/mrqaxes2.h \
    ../ui/scriptmgr.h \
    ../ui/roboinfo.h \
    ../ui/roboprop.h \
    ../ui/cfgtab.h \
    ../ui/roboaxes.h \
    ../ui/aboutdlg.h \
    ../ui/logout.h \
    ../ui/pvtedit.h \
    ../ui/statebar.h \
#    ../ui/runtool.h \
    ../../model/tpvitem.h \
    ../../model/tpvgroup.h \
    ../../model/mcmodelobj.h \
    ../../model/mcmodel.h \
    ../../intf/imcstream.h \
    ../../intf/iserial.h \
    ../ui/tpvplot.h \
    ../ui/mrqview.h \
    ../ui/motionedit.h \
    ../ui/syspref.h \
    ../ui/modelview.h \
    ../../model/modelsyspref.h \
    ../../model/modelrobotmgr.h \
    ../ui/axesknob.h \
    ../../app/appwidget.h \
    ../../app/dpcobj.h \
    ../../model/scriptfile.h \
    ../../model/scriptgroup.h \
    ../../model/scriptmodel.h \
    ../ui/tableedit.h \
    ../ui/querymgr.h \
    ../ui/queryview.h \
    ../../com/comassist.h \
    ../../model/spycfgmodel.h \
    ../../model/spyitemmodel.h \
    ../../model/comboxdelegate.h \
    ../../com/drivercontroller.h \
    ../../model/checkdelegate.h \
#    ../../app/tpvdownloader.h \
    ../../app/sampleproxy.h \
    ../../app/samplemgr.h \
    ../../app/samplethread.h \
    ../../app/interrupthandler.h \
    ../ui/eventviewer.h \
    ../../model/dbmeta.h \
    ../ui/roboscene.h \
    ../../widget/codeeditor.h \
    ../../widget/highlighter.h \
    ../../widget/flowlayout.h \
    ../ui/scripteditor.h \
    ../ui/deviceconsole.h \
    ../ui/dlgwndlist.h \
    ../../widget/scenewidget.h \
    ../../model/roboscenemodel.h \
    ../ui/axesconnection.h \
    ../ui/roboconnection.h \
    ../../model/mcconnection.h \
    ../../com/robomsg.h \
    ../../app/robonetthread.h \
    ../../com/robomsgqueue.h \
    ../../com/robomsgthread.h \
    ../../app/systimer.h \
    ../ui/progressgroup.h \
    ../ui/progresselement.h \
    ../ui/robohand.h \
    ../../arith/megaarith.h \
    ../../model/handactionmodel.h \
    ../../model/eventactionmodel.h \
    ../ui/warnprompt.h \
    ../../model/megatablemodel.h \
    ../../widget/valuecombobox.h \
    ../ui/pvtpref.h \
    ../ui/motionpref.h \
    ../ui/anglemonitor.h \
    ../ui/anglewidget.h \
    ../ui/motorstate.h \
    ../ui/motormonitor.h \
    ../ui/deltapref.h \
    ../ui/megatronpref.h \
    ../ui/robocompref.h \
    ../ui/sinanjupref.h \
    ../ui/mrqdcaxes.h \
    ../ui/robodesc.h \
    ../ui/dlgview.h \
    ../ui/mrqalarm.h \
    ../ui/encoderalarmpage.h \
    ../ui/distancealarmpage.h \
    ../ui/robojoint.h \
    ../ui/mrquart.h \
    ../ui/mrqsubuart.h \
    ../ui/mrqsensorpage.h \
    ../ui/axnpref.h \
    ../ui/mrqtrigchan.h \
    ../ui/mrqtrigpage.h \
    ../ui/mrqaxesplan.h \
    ../../widget/megamessagebox.h \
    ../ui/mrqaxespage.h \
    ../ui/roboconfig.h \
    ../ui/sinanjuconfig.h \
    ../ui/deltaconfig.h \
    ../ui/testprop.h \
    ../../model/megatronmotiongroup.h \
    ../../model/megatronmotionitem.h \
    ../../model/sinanjumotionitem.h \
    ../../model/sinanjumotiongroup.h \
    ../ui/motionwizard.h \
    ../../model/h2motionitem.h \
    ../../model/h2motiongroup.h \
    ../ui/h2pref.h \
    ../ui/statediagram.h \
    ../ui/h2config.h \
    ../../model/deltamotiongroup.h \
    ../ui/mrqin.h \
    ../ui/mrqyo.h \
    ../ui/mrqdo.h \
    ../ui/mrqxi.h \
    ../ui/mrqai.h \
    ../ui/lrbutton.h \
    ../../model/h2zmotiongroup.h \
    ../../model/h2zmotionitem.h \
    ../ui/quicktool.h \
    ../ui/ippref.h \
    ../ui/t4panel.h \
    ../../model/ipmotiongroup.h \
    ../ui/ipconfig.h \
    ../ui/mrvvertical.h \
    ../ui/mrvrecord.h \
    ../ui/mrvthreshold.h \
    ../ui/mrvio.h \
    ../ui/mrvswitch.h \
    ../ui/mrvch.h \
    ../ui/mrvprop.h \
    ../ui/mrvinfo.h \
    ../ui/mrvsystem.h \
    ../ui/h2zpanel.h \
    ../ui/h2panel.h \
    ../ui/robopanel.h \
    ../../app/comthread.h \
    ../ui/mrvview.h \
    ../ui/mrxview.h \
    ../../model/tpgroup.h \
    ../../model/tpitem.h \
    ../ui/tpedit.h \
    ../ui/motorpref.h \
    ../../com/rpcthread.h

FORMS += \
        ../ui/mainwindow.ui \
        ../ui/mrqinfo.ui \
        ../ui/mrqproperty.ui \
        ../ui/mrqaxes.ui \
        ../ui/mrqio.ui \
        ../ui/mrqsensor.ui \
        ../ui/mrqsys.ui \
    ../ui/devicemgr.ui \
    ../ui/robomgr.ui \
    ../ui/mrqaxes2.ui \
    ../ui/scriptmgr.ui \
    ../ui/roboaxes.ui \
    ../ui/roboinfo.ui \
    ../ui/cfgtab.ui \
    ../ui/aboutdlg.ui \
    ../ui/logout.ui \
    ../ui/pvtedit.ui \
    ../ui/motionedit.ui \
    ../ui/statebar.ui \
#    ../ui/runtool.ui \
    ../ui/tpvplot.ui \
    ../ui/syspref.ui \
    ../ui/axesknob.ui \
    ../ui/querymgr.ui \
    ../ui/queryview.ui \
    ../ui/eventviewer.ui \
    ../ui/roboscene.ui \
    ../ui/scripteditor.ui \
    ../ui/deviceconsole.ui \
    ../ui/dlgwndlist.ui \
    ../../widget/scenewidget.ui \
    ../ui/axesconnection.ui \
    ../ui/roboconnection.ui \
    ../ui/progressgroup.ui \
    ../ui/progresselement.ui \
    ../ui/robohand.ui \
    ../ui/warnprompt.ui \
    ../ui/pvtpref.ui \
    ../ui/motionpref.ui \
    ../ui/anglemonitor.ui \
    ../ui/anglewidget.ui \
    ../ui/motorstate.ui \
    ../ui/motormonitor.ui \
    ../ui/deltapref.ui \
    ../ui/megatronpref.ui \
    ../ui/robocompref.ui \
    ../ui/mrqdcaxes.ui \
    ../ui/robodesc.ui \
    ../ui/mrqalarm.ui \
    ../ui/encoderalarmpage.ui \
    ../ui/distancealarmpage.ui \
    ../ui/robojoint.ui \
    ../ui/sinanjupref.ui \
    ../ui/mrquart.ui \
    ../ui/mrqsubuart.ui \
    ../ui/mrqsensorpage.ui \
    ../ui/axnpref.ui \
    ../ui/mrqtrigchan.ui \
    ../ui/mrqtrigpage.ui \
    ../ui/mrqaxesplan.ui \
    ../ui/mrqaxespage.ui \
    ../ui/roboconfig.ui \
    ../ui/sinanjuconfig.ui \
    ../ui/deltaconfig.ui \
    ../ui/testprop.ui \
    ../ui/motionwizard.ui \
    ../ui/h2pref.ui \
    ../ui/statediagram.ui \
    ../ui/h2config.ui \
    ../ui/mrqin.ui \
    ../ui/mrqyo.ui \
    ../ui/mrqdo.ui \
    ../ui/mrqxi.ui \
    ../ui/mrqai.ui \
    ../ui/lrbutton.ui \
    ../ui/quicktool.ui \
    ../ui/ippref.ui \
    ../ui/t4panel.ui \
    ../ui/ipconfig.ui \
    ../ui/mrvvertical.ui \
    ../ui/mrvrecord.ui \
    ../ui/mrvthreshold.ui \
    ../ui/mrvio.ui \
    ../ui/mrvinfo.ui \
    ../ui/mrvswitch.ui \
    ../ui/mrvch.ui \
    ../ui/mrvsystem.ui \
    ../ui/h2zpanel.ui \
    ../ui/h2panel.ui \
    ../ui/tpedit.ui \
    ../ui/motorpref.ui

RESOURCES = ../res/res.qrc

RC_ICONS = ../res/image/logo/megarobo.ico

TRANSLATIONS = ../../translate/megarobostudio_zh_CN.ts \
                ../../translate/megarobostudio_zh_TW.ts
DISTFILES += \
    ../../../installer/doc/readme.txt


