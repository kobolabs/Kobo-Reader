#!/bin/sh

tar zxf ../packages/libusb-0.1.12.tar.gz
pushd libusb-0.1.12
	./configure --host=arm-linux --prefix=/
	make
	make DESTDIR=/chroot install
popd
