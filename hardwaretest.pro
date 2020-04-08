#-------------------------------------------------
#
# Project created by QtCreator 2016-10-27T07:05:05
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hardwaretest
TEMPLATE = app

# The path of target platform
target.path = /usr/bin
INSTALLS += target

include(qextserialport/src/qextserialport.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    timedialog.cpp \
    slavethread.cpp

HEADERS  += mainwindow.h \
    timedialog.h \
    slavethread.h

FORMS    += mainwindow.ui \
    timedialog.ui

RESOURCES += \
    hardwaretext_resource.qrc
