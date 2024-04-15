# Introduction
One Qt program which is used by Chipsee to test hardware.

# Required Packages
sudo apt install git qt5-default make g++ libqt5serialport5-dev qtdeclarative5-dev qml-module-qtquick2 qml-module-qtquick-controls qtmultimedia5-dev qtconnectivity5-dev gpiod libgpiod-dev

# Supported boards
This program only supports Chipsee follow platform products now:
 - [IMX6QDL](https://chipsee.com/product-category/ipc/arm/?filter_cpu=nxp-imx6q&query_type_cpu=or)
 - [IMX6UL](https://chipsee.com/product-category/ipc/arm/?query_type_cpu=or&filter_cpu=nxp-imx6ul)
 - [Industrial-pi](https://chipsee.com/product-category/ipc/arm-raspberry-pi/)
 - [AM335X](https://chipsee.com/product-category/ipc/arm/?filter_cpu=ti-am3354&query_type_cpu=or)
 - [PX30](https://chipsee.com/product-category/ipc/arm/?filter_cpu=rockchip-px30&query_type_cpu=or)
 - [RK3399](https://chipsee.com/product-category/ipc/arm/?query_type_cpu=or&filter_cpu=rockchip-rk3399)
 - RK3568

# Release notes
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

