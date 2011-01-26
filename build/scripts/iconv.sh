#!/bin/sh

tar zxf ../packages/libiconv-1.13.1.tar.gz 
pushd libiconv-1.13.1
	./configure --disable-rpath --prefix=/chroot --with-libiconv-prefix=/ --host=arm-linux
	make && make install
popd
