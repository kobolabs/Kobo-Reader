#!/bin/sh

tar zxf ../packages/libmng-1.0.10.tar.gz
pushd libmng-1.0.10
	cp unmaintained/autogen.sh .
	patch -p0 < ../../patches/libmng-autogen.patch
	./autogen.sh
	CPPFLAGS="-I/chroot/include" LDFLAGS="-L/chroot/lib" ./configure --prefix=/ --host=arm-linux
	make
	make DESTDIR=/chroot install
popd