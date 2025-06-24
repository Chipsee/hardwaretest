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

# Get libgpiod version
LIBGPIOD_VERSION = $$system(pkg-config --modversion libgpiod)

message(LIBGPIOD_VERSION = $$LIBGPIOD_VERSION)

equals(find(LIBGPIOD_VERSION, "2."), -1){
    message("Using libgpiod v1")
    DEFINES += USE_GPIOD_V1
} else {
    message("Using libgpiod v2")
    DEFINES += USE_GPIOD_V2
}

message(DEFINES = $$DEFINES)

SOURCES += main.cpp\
    epflash.cpp \
    gpiocontroller.cpp \
        mainwindow.cpp \
    timedialog.cpp \
    slavethread.cpp \
    device.cpp \
    service.cpp \
    lcdtester.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    epflash.h \
    gpiocontroller.h \
    timedialog.h \
    slavethread.h \
    utils.h \
    version.h \
    device.h \
    service.h \
    lcdtester.h

FORMS    += mainwindow.ui \
    epflash.ui \
    timedialog.ui \
    device.ui \
    service.ui \
    lcdtester.ui

RESOURCES += \
    hardwaretext_resource.qrc
