#!/bin/sh

tar jxf ../packages/busybox-1.17.1.tar.bz2
patch -p0 < ../patches/busybox-1.17.1.patch
pushd busybox-1.17.1
	make defconfig
	make CROSS_COMPILE=arm-linux- install
	cp -a _install/* /chroot
popd
