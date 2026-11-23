# Introduction
One Qt program which is used by Chipsee to test Chipsee products and one Demo to use hardware resources.

# Required Packages
```
sudo apt install qt5-default  // some platforms doesn't have this package, ignore it
sudo apt install git make g++ libqt5serialport5-dev libqt5serialbus5-dev libqt5serialbus5-plugins iproute2 qtdeclarative5-dev qml-module-qtquick2 qml-module-qtquick-controls qtmultimedia5-dev qtconnectivity5-dev gpiod libgpiod-dev qml-module-qtquick-controls2
```

# Build system DDR limited
Need at least 4GB DDR to build this source.

You can use swap if your hardware's DDR is less than 4GB.

For Industril Pi products, you can setting `CONF_SWAPSIZE=2048` in `/etc/dphys-swapfile` and reboot

# Supported boards
This program only support the following Chipsee products now:
 - [IMX6QDL](https://chipsee.com/product-category/ipc/arm/?filter_cpu=nxp-imx6q&query_type_cpu=or)
 - [IMX6UL](https://chipsee.com/product-category/ipc/arm/?query_type_cpu=or&filter_cpu=nxp-imx6ul)
 - [IMX8MP](https://chipsee.com/product-category/ipc/arm-nxp/?filter_cpu=arm-nxp-i-mx8mp&query_type_cpu=or)
 - [Industrial-pi](https://chipsee.com/product-category/ipc/arm-raspberry-pi/)
 - [AM335X](https://chipsee.com/product-category/ipc/arm/?filter_cpu=ti-am3354&query_type_cpu=or)
 - [PX30](https://chipsee.com/product-category/ipc/arm/?filter_cpu=rockchip-px30&query_type_cpu=or)
 - [RK3399](https://chipsee.com/product-category/ipc/arm/?query_type_cpu=or&filter_cpu=rockchip-rk3399)
 - [RK3568](https://chipsee.com/product-category/ipc/arm-rockchip/?filter_cpu=arm-rockchip-rk3568&query_type_cpu=or)
 - [RK3588](https://chipsee.com/product-category/ipc/arm-rockchip/?filter_cpu=rockchip-rk3588&query_type_cpu=or)

 - NVIDIA Tegra

# Manual CAN test on Linux

Run hardwaretest with permission to configure CAN network interfaces. Close any
other CAN test first and use an interface that is down. Click **Refresh** after
adding a USB CAN adapter, select the interface and bitrates, then click **Open**.
Classic CAN defaults to 500000 bit/s. CAN FD defaults to 500000 bit/s for
arbitration and 2000000 bit/s for data, with bitrate switching enabled on TX.
Set the same bitrates on the peer. The default TX frame uses ID `5A1` and
payload `11 22 33 44 55 66 77 88`. Click **Close** to release the interface.

On NVIDIA Tegra, run from the desktop session with sudo so CAN, RTC, LED and
backlight controls can access their Linux interfaces. Audio uses GStreamer
(`gst-play-1.0`, `pipewiresink`) and the desktop user's default PipeWire sink.
Volume changes use `wpctl`. Install the package or its media files before running
the audio test. The I²C dialog lists exposed adapters. The buzzer control is
enabled when the board exposes `/dev/buzzer`.

# Build deb packages
```
sudo apt-get update
sudo apt install debhelper
mkdir hardwaretest-packages
cd hardwaretest-packages
git clone https://github.com/Chipsee/hardwaretest.git
cd hardwaretest
dpkg-buildpackage -us -uc
```
Then, you can get hardwaretest_\<VERSION\>-1_arm64.deb in the hardwaretest-packages directory

# Release notes
**V5.10:**

- Add function to check RK3588 CPU
- Add hasbl function to control the backlight feature
- Disable GPIO on CS12720RA5050P
- Add 4G CAT1 GPS support.
- Enable 4G CAT1 dialog automaticly when detected it.
- Added Deb build script.
- Added checkRelay function.

**V5.9:**

- Fix gpiod detect function
- Use thread to record and play audio on STMP25 board
- Added recorderworker class.
- fix gpiocontroller compatible issue
- Added CS12800RA5101A product support
- Use weston user to run pipewire command on stmp25 platform yocto system
- Added Autotest support for STM32MP25 platform product.
- Change RK3588 to use thread record and play audio

**V5.8:**

- Add CS10600RA5070P support
- Add CS12800RA5101P support
- Support to test CM5 products
- Program can enter to manufecture autotest mode by rename program name to hardwaretest_auto 
- Add flash device size for CM5 products features.
- Fix save/restore ui geometry bugs.
- Add RK3588 Ubuntu20.04 support
- add cm5 133h 150h 156h products ep support
- fix 133h 150h 156h button group
- Added STMP25 product support


**V5.7:**

- Add CS12800RA4101AV4 support
- Add R2P Product support
- Add AutoTest support for IMX8MP products
- Add CS12800_RK3588_101 board and machine support
- Bug fix: BOX product doesn't have backlight


**V5.6:**

- Add RK3588 070 BOX product support


**V5.5:**

- Add CS19108RA4156P products support.


**V5.1:**

- Add RK3568 Platform products support.
- Fix CAN bugs for Industrial-pi platform.


**V5.0:**

- Add PX30 product support.
- Add RK3399 product support.
- Other bug fix.


**V4.2:**

- Fix GPS bugs.


**V4.1:**

- First Record.
- Add Release notes.

