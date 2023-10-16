#-------------------------------------------------
#
# Project created by QtCreator 2016-10-27T07:05:05
#
#-------------------------------------------------

QT       += core gui serialport network bluetooth qml quick multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hardwaretest
TEMPLATE = app

# The path of target platform
target.path = /usr/bin
INSTALLS += target

LIBS += -lgpiod

#include(qextserialport/src/qextserialport.pri)

SOURCES += main.cpp\
    gpiocontroller.cpp \
        mainwindow.cpp \
    timedialog.cpp \
    slavethread.cpp \
    device.cpp \
    service.cpp \
    lcdtester.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    gpiocontroller.h \
    timedialog.h \
    slavethread.h \
    utils.h \
    version.h \
    device.h \
    service.h \
    lcdtester.h

FORMS    += mainwindow.ui \
    timedialog.ui \
    device.ui \
    service.ui \
    lcdtester.ui

RESOURCES += \
    hardwaretext_resource.qrc
