#!/bin/sh

tar zxf ../packages/expat-2.0.1.tar.gz
pushd expat-2.0.1
	./configure --prefix=/ --host=arm-linux --disable-static
	make && make DESTDIR=/chroot install
popd
