#!/bin/sh

tar zxf ../packages/libpng-1.2.43.tar.gz
pushd libpng-1.2.43
	CPPFLAGS="-I/chroot/include" LDFLAGS="-L/chroot/lib" ./configure --with-libpng-compat --prefix=/ --host=arm-linux
	make
	make DESTDIR=/chroot install
popd
