#!/bin/sh

tar zxf ../packages/dosfstools-3.0.6.tar.gz
pushd dosfstools-3.0.6
	CC=arm-linux-gcc CFLAGS="-D__user= -DBLKSSZGET=_IO\(0x12,104\)" make
	cp mkdosfs /chroot/bin
	cp dosfsck /chroot/bin
popd
