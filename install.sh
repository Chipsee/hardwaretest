#!/bin/sh
## compile
# qmake && make
# cd LCDTester
# qmake && make

if [ ! -f hardwaretest -a ! -f LCDTester/LCDTester ]; then
	echo "no binary, compile them first"
	echo "How to:"
	echo "qmake && make"
	echo "cd LCDTester && qmake && make"
	exit 1
fi

## install
if [ -f hardwaretest-app.tar.gz ]; then
	rm hardwaretest-app.tar.gz
fi
mkdir -p usr/share/applications
mkdir -p usr/share/pixmaps
mkdir -p usr/bin/
mkdir -p usr/lib/arm-linux-gnueabihf/qt5/qml/QtQml
mkdir -p usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick
mkdir -p usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick/Window.2
mkdir -p usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick.2
sudo cp resourcefiles/hardwaretest usr/ -rf
sudo cp hardwaretest usr/hardwaretest/ -rf
sudo cp usr/hardwaretest/hardwaretest.desktop usr/share/applications/
sudo cp usr/hardwaretest/hardwaretest.png usr/share/pixmaps/
sudo cp LCDTester/LCDTester usr/bin/
sudo cp /usr/lib/arm-linux-gnueabihf/qt5/qml/builtins.qmltypes usr/lib/arm-linux-gnueabihf/qt5/qml/builtins.qmltypes
sudo cp /usr/lib/arm-linux-gnueabihf/qt5/qml/QtQml/plugins.qmltypes usr/lib/arm-linux-gnueabihf/qt5/qml/QtQml/plugins.qmltypes
sudo cp /usr/lib/arm-linux-gnueabihf/qt5/qml/QtQml/qmldir usr/lib/arm-linux-gnueabihf/qt5/qml/QtQml/qmldir
sudo cp /usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick/Window.2/libwindowplugin.so usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick/Window.2/libwindowplugin.so
sudo cp /usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick/Window.2/plugins.qmltypes usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick/Window.2/plugins.qmltypes
sudo cp /usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick/Window.2/qmldir usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick/Window.2/qmldir
sudo cp /usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick.2/libqtquick2plugin.so usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick.2/libqtquick2plugin.so
sudo cp /usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick.2/plugins.qmltypes usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick.2/plugins.qmltypes
sudo cp /usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick.2/qmldir usr/lib/arm-linux-gnueabihf/qt5/qml/QtQuick.2/qmldir
sync
tar zcvf hardwaretest-app.tar.gz usr
sync
tar zxvf hardwaretest-app.tar.gz -C /
sync
rm usr -rf
echo "Installed."
