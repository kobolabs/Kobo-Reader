#!/bin/sh

tar zxf ../packages/wpa_supplicant-0.7.1.tar.gz
pushd wpa_supplicant-0.7.1/wpa_supplicant
	cp defconfig .config
popd

patch -p0 < ../patches/wpa_supplicant-logging.patch

pushd wpa_supplicant-0.7.1/wpa_supplicant
	CC=arm-linux-gcc CFLAGS="-I/chroot/usr/include" LDFLAGS="-L/chroot/usr/lib" LIBS="-L/chroot/usr/lib" make DESTDIR=/chroot BINDIR=/bin install
popd


