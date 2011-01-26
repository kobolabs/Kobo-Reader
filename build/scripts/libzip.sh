#!/bin/sh

tar zxf ../packages/libzip-0.9.3.tar.gz
pushd libzip-0.9.3
	CPPFLAGS="-I/chroot/include" LDFLAGS="-L/chroot/lib" LIBS="-lz" ./configure --prefix=/usr --with-zlib=/chroot/lib --host=arm-linux
	make
	make DESTDIR=/chroot install
popd
