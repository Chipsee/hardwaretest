#!/bin/sh

#echo "Install dependency packages"
#sudo apt update
#sudo apt install git qt5-default make g++ libqt5serialport5-dev qtdeclarative5-dev qml-module-qtquick2 qml-module-qtquick-controls

check_commands () {
  for COMMAND in git qmake g++; do
    if ! command -v $COMMAND > /dev/null; then
      FAIL_REASON="$COMMAND not found, run apt-get install $COMMAND"
      echo $FAIL_REASON
      return 1
    fi
  done
  return 0
}

if ! check_commands; then
  exit 1;
fi

head=`git rev-parse --verify --short HEAD 2>/dev/null`
LAST_GIT_VERSION="+git$head"

git checkout version.h
sed -i "s/LAST_GIT_HASH/$LAST_GIT_VERSION/g" version.h

sleep 1

qmake -o Makefile hardwaretest.pro
make -j4
cp resourcefiles/hardwaretest /usr/ -rf
cp hardwaretest /usr/hardwaretest/
ln -sf /usr/hardwaretest/hardwaretest /usr/bin/hardwaretest
cp /usr/hardwaretest/hardwaretest.desktop /usr/share/applications/
cp /usr/hardwaretest/hardwaretest.png /usr/share/pixmaps/

cd LCDTester
qmake -o Makefile LCDTester.pro
make -j4
cp LCDTester /usr/hardwaretest/
ln -sf /usr/hardwaretest/LCDTester /usr/bin/LCDTester
cd ..
sync
