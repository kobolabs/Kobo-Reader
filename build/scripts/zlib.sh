#!/bin/sh

tar jxf ../packages/zlib-1.2.4.tar.bz2
pushd zlib-1.2.4
	LD=arm-linux-ld CC=arm-linux-gcc ./configure --prefix=/
	make
	make DESTDIR=/chroot install
popd
