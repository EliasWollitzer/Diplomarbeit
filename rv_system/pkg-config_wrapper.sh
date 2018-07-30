#!/bin/sh
PKG_CONFIG_SYSROOT_DIR=/opt/gelin2-17.11-imx6/host/arm-buildroot-linux-gnueabihf/sysroot
export PKG_CONFIG_SYSROOT_DIR
PKG_CONFIG_LIBDIR=/opt/gelin2-17.11-imx6/host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib/pkgconfig
export PKG_CONFIG_LIBDIR
exec pkg-config "$@"
