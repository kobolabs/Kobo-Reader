#!/bin/sh

tar zxf ../packages/libtool-2.2.8.tar.gz
pushd libtool-2.2.8
	./configure --host=arm-linux --program-prefix=arm-linux-
	make
	make DESTDIR=/chroot install
popd
