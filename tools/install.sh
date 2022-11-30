#!/bin/sh
## compile
# qmake && make
# cd LCDTester
# qmake && make

KVR=`uname -r`
LARCH=`uname -m`
if [ $LARCH = "aarch64" ]; then
LIBDIR="aarch64-linux-gnu"
else
LIBDIR="arm-linux-gnueabihf"
fi

if [ ! -f hardwaretest -a ! -f LCDTester/LCDTester ]; then
	echo "no binary, compile them first"
	echo "How to:"
	echo "qmake && make"
	echo "cd LCDTester && qmake && make"
	exit 1
fi

## install
if [ -f hardwaretest-app.tar.gz ]; then
	rm hardwaretest-app-*.tar.gz
fi
mkdir -p usr/share/applications
mkdir -p usr/share/pixmaps
mkdir -p usr/bin/
mkdir -p usr/lib/${LIBDIR}/qt5/qml/QtQml
mkdir -p usr/lib/${LIBDIR}/qt5/qml/QtQuick
mkdir -p usr/lib/${LIBDIR}/qt5/qml/QtQuick/Window.2
mkdir -p usr/lib/${LIBDIR}/qt5/qml/QtQuick.2
sudo cp resourcefiles/hardwaretest usr/ -rf
sudo cp hardwaretest usr/hardwaretest/ -rf
sudo cp usr/hardwaretest/hardwaretest.desktop usr/share/applications/
sudo cp usr/hardwaretest/hardwaretest.png usr/share/pixmaps/
sudo cp LCDTester/LCDTester usr/bin/
sudo cp /usr/lib/${LIBDIR}/qt5/qml/builtins.qmltypes usr/lib/${LIBDIR}/qt5/qml/builtins.qmltypes
sudo cp /usr/lib/${LIBDIR}/qt5/qml/QtQml/plugins.qmltypes usr/lib/${LIBDIR}/qt5/qml/QtQml/plugins.qmltypes
sudo cp /usr/lib/${LIBDIR}/qt5/qml/QtQml/qmldir usr/lib/${LIBDIR}/qt5/qml/QtQml/qmldir
sudo cp /usr/lib/${LIBDIR}/qt5/qml/QtQuick/Window.2/libwindowplugin.so usr/lib/${LIBDIR}/qt5/qml/QtQuick/Window.2/libwindowplugin.so
sudo cp /usr/lib/${LIBDIR}/qt5/qml/QtQuick/Window.2/plugins.qmltypes usr/lib/${LIBDIR}/qt5/qml/QtQuick/Window.2/plugins.qmltypes
sudo cp /usr/lib/${LIBDIR}/qt5/qml/QtQuick/Window.2/qmldir usr/lib/${LIBDIR}/qt5/qml/QtQuick/Window.2/qmldir
sudo cp /usr/lib/${LIBDIR}/qt5/qml/QtQuick.2/libqtquick2plugin.so usr/lib/${LIBDIR}/qt5/qml/QtQuick.2/libqtquick2plugin.so
sudo cp /usr/lib/${LIBDIR}/qt5/qml/QtQuick.2/plugins.qmltypes usr/lib/${LIBDIR}/qt5/qml/QtQuick.2/plugins.qmltypes
sudo cp /usr/lib/${LIBDIR}/qt5/qml/QtQuick.2/qmldir usr/lib/${LIBDIR}/qt5/qml/QtQuick.2/qmldir
sudo cp -a /usr/lib/${LIBDIR}/libQt5Qml.so* usr/lib/${LIBDIR}/
sudo cp -a /usr/lib/${LIBDIR}/libQt5Quick.so* usr/lib/${LIBDIR}/
sudo cp -a /usr/lib/${LIBDIR}/libQt5QmlModels.so* usr/lib/${LIBDIR}/
sudo cp -a /usr/lib/${LIBDIR}/libQt5QmlWorkerScript.so* usr/lib/${LIBDIR}/
sudo cp -a /usr/lib/${LIBDIR}/libQt5SerialPort.so* usr/lib/${LIBDIR}/
sync
tar zcvf hardwaretest-app-${KVR}.tar.gz usr
sync
tar zxvf hardwaretest-app-${KVR}.tar.gz -C /
sync
rm usr -rf
echo "Installed."
