#!/bin/sh

tar zxf ../packages/lcms-1.18a.tar.gz
pushd lcms-1.18
	CPPFLAGS="-I/chroot/include" LDFLAGS="-L/chroot/lib" ./configure --prefix=/ --host=arm-linux
	make
	make DESTDIR=/chroot install
popd