# Introduction
One Qt program which is used by Chipsee to test Chipsee products and one Demo to use hardware resources.

# Required Packages
```
sudo apt install qt5-default  // some platforms doesn't have this package, ignore it
sudo apt install git make g++ libqt5serialport5-dev qtdeclarative5-dev qml-module-qtquick2 qml-module-qtquick-controls qtmultimedia5-dev qtconnectivity5-dev gpiod libgpiod-dev qml-module-qtquick-controls2
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

# Release notes
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

