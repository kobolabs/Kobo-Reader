#!/bin/sh

tar zxf ../packages/attr_2.4.43-1.tar.gz
pushd attr-2.4.43
	LIBTOOL=/chroot/usr/local/bin/arm-linux-libtool CC=arm-linux-gcc ./configure --host=arm-linux-gcc --prefix=/chroot
	make install
	make install-dev
	make install-lib
popd
